#include "client.h"

#include <nlohmann/json.hpp>
#include <iostream>

Client::Client(asio::io_context& context, const std::string& serverAddress, uint16_t port, const std::string& username)
	:m_context(context), m_serverAddress(serverAddress), m_port(port), m_username(username), m_connected(false)
{
	m_socket = std::make_unique<asio::ip::tcp::socket>(m_context);
}

void Client::connect()
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

void Client::sendMessage(const std::string& message)
{
	if (!m_connected)
	{
		std::cerr << "Cannot send message - not connected\n";
		return;
	}

	try 
	{
		nlohmann::json jsonMessage;
		jsonMessage["username"] = m_username;
		jsonMessage["message"] = message;

		std::string serializedMessage = jsonMessage.dump();

		asio::write(*m_socket, asio::buffer(serializedMessage));	
	}
	catch (const std::exception& e) 
	{
		std::cerr << "Send message error: " << e.what() << '\n';
		m_connected = false;
	}
}

void Client::startReading()
{
	std::thread([this]()
			{
				readFromServer();
			}).detach();
}

std::string Client::getUsername() const
{
	return m_username;
}

bool Client::isConnected() const
{
	return m_connected;
}

void Client::readFromServer()
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

				if (m_messageQueue.size() > 250)
				{
					std::cerr << "[WARNING]: Max messages reached in queue. Ignoring new messages until queue is cleared!\n";
				}
				else
				{
					m_messageQueue.push_back(message);
				}

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
