#pragma once

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <WinSock2.h>
	#include <WS2tcpip.h>
#endif

#define ASIO_STANDALONE
#include <asio.hpp>

#include <string>
#include <memory>
#include <vector>
#include <mutex>
#include <iostream>

struct ClientSession 
{
	std::shared_ptr<asio::ip::tcp::socket> socket;
	std::string username;
};

class Server 
{
public:

	Server(asio::io_context& context, uint16_t port, size_t maxClients = 10)
		:m_context(context), m_acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), m_maxClients(maxClients), m_running(false)
	{
	}

	void start()
	{
		m_running = true;
		acceptConnections();
	}

	void broadcastMessage(const std::string& message, std::shared_ptr<asio::ip::tcp::socket> excludeSocket = nullptr)
	{
		std::lock_guard<std::mutex> lock(m_clientsMutex);

		for (auto& client : m_clients)
		{
			if (excludeSocket && client->socket == excludeSocket) continue;

			try 
			{
				asio::write(*client->socket, asio::buffer(message));	
			}
			catch (const std::exception& e) 
			{
				std::cerr << "Broadcast error: " << e.what() << '\n';
				removeClient(client);
			}
		}

	}

	void shutdown()
	{
		m_running = false;

		std::lock_guard<std::mutex> lock(m_clientsMutex);
		for (auto& client : m_clients)
		{
			if (client->socket->is_open())
			{
				client->socket->close();
			}
		}
		m_clients.clear();


		m_acceptor.close();
	}

private:

	void acceptConnections()
	{
		auto newSocket = std::make_shared<asio::ip::tcp::socket>(m_acceptor.get_executor());

		m_acceptor.async_accept(*newSocket, [this, newSocket](std::error_code ec)
				{
					if (ec)
					{
						std::cerr << "Accept error: " << ec.message() << '\n';
						return;
					}

					{
						std::lock_guard<std::mutex> lock(m_clientsMutex);
						if (m_clients.size() >= m_maxClients)
						{
							std::cout << "Warning: Max clients reached, rejecting new connection...\n";
							newSocket->close();

							acceptConnections();
							return;
						}
					}

					auto newClient = std::make_shared<ClientSession>();
					newClient->socket = newSocket;

					{
						std::lock_guard<std::mutex> lock(m_clientsMutex);
						m_clients.push_back(newClient);
					}

					std::string clientIP = newClient->socket->remote_endpoint().address().to_string();

					std::cout << "Client '" << clientIP << "' connected!\n";

					readClientMessage(newClient);

					acceptConnections();
				});
	}

	void readClientMessage(std::shared_ptr<ClientSession> client)
	{
		auto buffer = std::make_shared<std::array<char, 1024>>();

		client->socket->async_read_some(asio::buffer(*buffer),
				[this, client, buffer](std::error_code ec, std::size_t length)
				{
					if (ec)
					{
						std::cout << "Client disconnected.\n";
						removeClient(client);
						return;
					}

					std::string message(buffer->data(), length);

					if (client->username.empty())
					{
						client->username = message;
						std::cout << "New Client Username: " << client->username << '\n'; 
					}
					else
					{
						std::string fullMessage = client->username + ": " + message;
						std::cout << "Received: " << fullMessage << '\n';
						broadcastMessage(fullMessage, client->socket);
					}
					
					readClientMessage(client);

				});
	}

	void removeClient(std::shared_ptr<ClientSession> client)
	{
		std::lock_guard<std::mutex> lock(m_clientsMutex);

		if (client->socket->is_open())
		{
			client->socket->close();
		}

		auto it = std::find(m_clients.begin(), m_clients.end(), client);
		if (it != m_clients.end())
		{
			m_clients.erase(it);
		}

		std::cout << "Client removed.\n";

	}


private:	

	asio::io_context& m_context;
	asio::ip::tcp::acceptor m_acceptor;
	std::vector<std::shared_ptr<ClientSession>> m_clients;
	std::mutex m_clientsMutex;
	size_t m_maxClients;
	bool m_running;
};

class Client
{
public:

	Client(asio::io_context& context, const std::string& serverAddress, uint16_t port, const std::string& username)
		:m_context(context), m_serverAddress(serverAddress), m_port(port), m_username(username), m_connected(false)
	{
		m_socket = std::make_unique<asio::ip::tcp::socket>(m_context);
	}

	void connect()
	{
		try 
		{
			asio::ip::tcp::endpoint endpoint(asio::ip::make_address(m_serverAddress), m_port);
			m_socket->connect(endpoint);
			m_connected = true;


			sendMessage(m_username);

			startReading();
		}
		catch (const std::exception& e) 
		{
			m_connected = false;
			std::cerr << "Connection error: " << e.what() << '\n';
		}
	}

	void sendMessage(const std::string& message)
	{
		if (!m_connected)
		{
			std::cerr << "Cannot send message - not connected\n";
			return;
		}

		try 
		{
			asio::write(*m_socket, asio::buffer(message));	
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Send message error: " << e.what() << '\n';
			m_connected = false;
		}
	}

	void startReading()
	{
		std::thread([this]()
				{
					readFromServer();
				}).detach();
	}

	std::string getUsername() const
	{
		return m_username;
	}

	bool isConnected() const
	{
		return m_connected;
	}

private:

	void readFromServer()
	{
		try 
		{
			while (m_connected)
			{
				char buffer[1024];
				std::size_t length = m_socket->read_some(asio::buffer(buffer));

				if (length > 0)
				{
					std::string message(buffer, length);
					std::cout << "[Server]: " << message << '\n';
				}
			}
		}
		catch (const std::exception& e) 
		{
			std::cerr << "Read error: " << e.what() << '\n';
			m_connected = false;
		}
	}

private:

	asio::io_context& m_context;

	std::unique_ptr<asio::ip::tcp::socket> m_socket;

	std::string m_username;

	std::string m_serverAddress;
	uint16_t m_port;

	bool m_connected;

};
