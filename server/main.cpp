#include "networking.h"

int main()
{
	try 
	{
		asio::io_context context;


		Server server(context, 12345);

		server.start();

		context.run();
	}
	catch (const std::exception& e) 
	{
		std::cerr << "Server error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
