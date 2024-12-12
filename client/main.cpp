#include "networking.h"

int main()
{
	std::string username;
	std::getline(std::cin, username);

	try 
	{
		asio::io_context context;

		Client client(context, "127.0.0.1", 12345, username);

		client.connect();


		std::string message;
		
		while (client.isConnected())
		{
			std::getline(std::cin, message);
			if (message == "/quit") break;
			client.sendMessage(message);
		}
		
	}
	catch (const std::exception& e) 
	{
		std::cerr << "ERROR: " << e.what() << '\n';
	}
	return 0;
}
