#include "ScreenServer.h"

int main(int argc, char** argv)
{
	boost::asio::io_service io_service;
	ScreenServer server(io_service);
	server.start_listen();
	io_service.run();

	return 0;
}