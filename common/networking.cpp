#include "networking.h"

#include <iostream>
#include <asio.hpp>

void test()
{
	asio::io_context context;
	std::cout << "Hello I am networking!\n";
}
