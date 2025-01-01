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

#include <nlohmann/json.hpp>

struct ClientSession 
{
	std::shared_ptr<asio::ip::tcp::socket> socket;
	std::string username;

	bool tryToSendMessage(const std::string& message)
	{
		try 
		{
			asio::write(*this->socket, asio::buffer(message));
			return true;
		}
		catch (const std::exception& e) 
		{
			std::cerr << "ERROR: Failed to send message to client '" + this->username + "'\nMessage Reads: " + e.what() + '\n';
			return false;
		}
	}
};

enum Commands
{
	NONE,
	DM,
};

class Server 
{
public:

	Server(asio::io_context& context, uint16_t port, size_t maxClients = 10);

	void start();

	void broadcastMessage(const std::string& message, std::shared_ptr<asio::ip::tcp::socket> excludeSocket = nullptr);

	void shutdown();

private:

	void acceptConnections();

	void readClientMessage(std::shared_ptr<ClientSession> client);

	void removeClient(std::shared_ptr<ClientSession> client);

	void handleMessage(const nlohmann::json& jsonMessage, const std::shared_ptr<ClientSession>& client);

private:	

	asio::io_context& m_context;
	asio::ip::tcp::acceptor m_acceptor;
	std::vector<std::shared_ptr<ClientSession>> m_clients;
	std::mutex m_clientsMutex;
	size_t m_maxClients;
	bool m_running;
};
