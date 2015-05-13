#include <stdio.h>
#include <stdlib.h>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
	int port = 1000;

	if (argc > 1)
		port = atoi(argv[1]);

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket sock(io_service);

}