#include "server.h"

#include "Logging.h"

#include <nlohmann/json.hpp>

Server::Server(asio::io_context& context, uint16_t port, size_t maxClients)
	:m_context(context), m_acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), m_maxClients(maxClients), m_running(false)
{
}

void Server::start()
{
	Log::Info("Server has started!!");

	m_running = true;
	acceptConnections();
}

void Server::broadcastMessage(const std::string& message, std::shared_ptr<asio::ip::tcp::socket> excludeSocket)
{
	std::lock_guard<std::mutex> lock(m_clientsMutex);

	for (auto& client : m_clients)
	{
		if (excludeSocket && client->socket == excludeSocket) continue;

		// Here we try to send the message to the client, and if it fails, and error message will be printed and they'll be removed.
		if (!client->tryToSendMessage(message))
		{
			removeClient(client);
		}
	}

}			

void Server::shutdown()
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

void Server::acceptConnections()
{
	auto newSocket = std::make_shared<asio::ip::tcp::socket>(m_acceptor.get_executor());

	m_acceptor.async_accept(*newSocket, [this, newSocket](std::error_code ec)
			{
				if (ec)
				{
					Log::Error("Accpet error: " + ec.message());
					return;
				}

				{
					std::lock_guard<std::mutex> lock(m_clientsMutex);
					if (m_clients.size() >= m_maxClients)
					{
						Log::Warning("Max Clients Reached, rejection new connections...");

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

				Log::Info("Client '" + clientIP + "' connected!");

				readClientMessage(newClient);

				acceptConnections();
			});
}

void Server::readClientMessage(std::shared_ptr<ClientSession> client)
{
	auto buffer = std::make_shared<std::array<char, 1024>>();

	client->socket->async_read_some(asio::buffer(*buffer),
			[this, client, buffer](std::error_code ec, std::size_t length)
			{
				if (ec)
				{
					Log::Info("Client disconnected");
					removeClient(client);
					return;
				}

				std::string serializedMessage(buffer->data(), length);

				if (!serializedMessage.empty() && serializedMessage.find_first_not_of(" \n\r\t") != std::string::npos)
				{

					nlohmann::json jsonMessage; 

					try 
					{

						jsonMessage = nlohmann::json::parse(serializedMessage);
						

					}
					catch (const std::exception& e)
					{

						std::string errorMessage = "Couldn't parse message: ";
						errorMessage += e.what();
						Log::Error(errorMessage);
					}

					// This might replace everything else so be prepared my young paduon
					handleMessage(jsonMessage, client);

				}

				
				
				readClientMessage(client);

			});
}

void Server::removeClient(std::shared_ptr<ClientSession> client)
{
	std::string clientUsername = client->username;

	if (!client->username.empty())
	{
		broadcastMessage("'" + clientUsername + "' has disconnected!!", client->socket);
	}

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

	Log::Info("Client '" + clientUsername + "' removed.");

}

void Server::handleMessage(const nlohmann::json& jsonMessage, const std::shared_ptr<ClientSession>& client)
{
	std::string message = jsonMessage["message"];
	std::string username = jsonMessage["username"];

	if (message.rfind("/dm ", 0) == 0)
	{
		std::istringstream iss(message);
		std::string command, recipient, dmMessage;
		iss >> command >> recipient;
		std::getline(iss, dmMessage);

		if (!dmMessage.empty() && dmMessage[0] == ' ')
		{
			dmMessage.erase(0, 1);
		}

		std::lock_guard<std::mutex> lock(m_clientsMutex);

		auto it = std::find_if(m_clients.begin(), m_clients.end(),
				[&recipient](const std::shared_ptr<ClientSession>& client)
				{
					return client->username == recipient;
				});

		if (it != m_clients.end())
		{
			std::string fullMessage = "[DM From '" + client->username + "']: " + dmMessage;
			(*it)->tryToSendMessage(fullMessage);
			Log::Info("Dm sent from '" + client->username + "' to '" + (*it)->username + "' [Message]: " + dmMessage); 
		}
		else
		{
			client->tryToSendMessage("User '" + recipient + "' not found.");
			Log::Info("Dm failed to send to '" + recipient + "' from '" + (*it)->username + "' because they were not found!");
		}


	}
	else
	{
		if (client->username.empty())
		{
			client->username = username;
			Log::Info("New Client Username: " + client->username);
			broadcastMessage("'" + client->username + "' has joined!!");
		}
		else
		{
			std::string fullMessage = client->username + ": " + message;
			Log::Info("Received: " + fullMessage);
			broadcastMessage(fullMessage, client->socket);
		}
	}
}

