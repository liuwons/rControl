
#include <stdio.h>
#include <stdlib.h>

#include <boost/thread.hpp>

#include "ScreenClient.h"
#include "Buffer.h"
#include "decode.h"

int main(int argc, char** argv)
{
	int port = 1000;

	if (argc > 1)
		port = atoi(argv[1]);

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endp(boost::asio::ip::address::from_string("127.0.0.1"), port);

    boost::shared_ptr<rc::DataBuffer> buf(new rc::DataBuffer(5 * 1024 * 1024));
    //boost::shared_ptr<ScreenClient> client(new ScreenClient(io_service, endp, buf));

	boost::shared_ptr<rc::Decode> decode(new rc::Decode(buf));

	//printf("starting client\n");
	//client->start();

	//printf("starting io_service\n");
	//boost::thread([&io_service]{io_service.run(); });

	printf("starting decode\n");
	decode->start();
	Sleep(1000000);


    return 0;
}