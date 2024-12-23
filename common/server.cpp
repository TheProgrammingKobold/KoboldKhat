#include "server.h"

Server::Server(asio::io_context& context, uint16_t port, size_t maxClients)
	:m_context(context), m_acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), m_maxClients(maxClients), m_running(false)
{
}

void Server::start()
{
	m_running = true;
	acceptConnections();
}

void Server::broadcastMessage(const std::string& message, std::shared_ptr<asio::ip::tcp::socket> excludeSocket)
{
	std::lock_guard<std::mutex> lock(m_clientsMutex);

	for (auto& client : m_clients)
	{
		if (excludeSocket && client->socket == excludeSocket) continue;

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

void Server::readClientMessage(std::shared_ptr<ClientSession> client)
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
					broadcastMessage("'" + client->username + "' has joined!!");
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

void Server::removeClient(std::shared_ptr<ClientSession> client)
{
	if (!client->username.empty())
	{
		broadcastMessage("'" + client->username + "' has disconnected!!", client->socket);
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

	std::cout << "Client removed.\n";

}