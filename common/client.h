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

class Client
{
public:

	Client(asio::io_context& context, const std::string& serverAddress, uint16_t port, const std::string& username);

	void connect();

	void sendMessage(const std::string& message);

	void startReading();

	std::string getUsername() const;

	bool isConnected() const;

private:

	void readFromServer();

private:

	asio::io_context& m_context;

	std::unique_ptr<asio::ip::tcp::socket> m_socket;

	std::string m_username;

	std::string m_serverAddress;
	uint16_t m_port;

	bool m_connected;

};
