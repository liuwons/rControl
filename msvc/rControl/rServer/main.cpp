#include "ScreenServer.h"

#include <thread>

int main(int argc, char** argv)
{
	boost::asio::io_service io_service;
	ScreenServer server(io_service);
	server.start_listen();

    std::thread th([&io_service]{io_service.run(); });

    char line[10];
    while (std::cin.getline(line, 10))
    {
        const char* l = line;
        server.asyn_deliver(boost::asio::mutable_buffer(line, 10));
        memset(line, 0, 10);
    }

	return 0;
}