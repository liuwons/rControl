#include <stdio.h>
#include <stdlib.h>

#include "ScreenClient.h"
#include "Buffer.h"

int main(int argc, char** argv)
{
	int port = 1000;

	if (argc > 1)
		port = atoi(argv[1]);

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endp(boost::asio::ip::address::from_string("127.0.0.1"), port);

    boost::shared_ptr<rc::Buffer> buf(new rc::Buffer(5 * 1024 * 1024));
    boost::shared_ptr<ScreenClient> client(new ScreenClient(io_service, endp, buf));
    client->start();

    io_service.run();

    return 0;
}