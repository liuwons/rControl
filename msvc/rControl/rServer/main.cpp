#include "ScreenServer.h"

#include <boost/thread.hpp>

#include "encode.h"

using namespace rs;

void screen_proc_data_callback(void* encoded_data, int encoded_data_len, void* pUser)
{
    ScreenServer* s = (ScreenServer*)pUser;

    char ch[10];
    sprintf_s(ch, "%d", encoded_data_len);
    s->asyn_deliver(boost::asio::buffer(ch, 10));
    s->asyn_deliver(boost::asio::buffer(encoded_data, encoded_data_len));
}

void screen_func(shared_ptr<ScreenServer> screen_server)
{
    encode_screen(3600 * 24, screen_proc_data_callback, screen_server.get());
}

int main(int argc, char** argv)
{
	boost::asio::io_service io_service;

    boost::shared_ptr<ScreenServer> screen_server_ptr(new ScreenServer(io_service));
	screen_server_ptr->start_listen();

    boost::thread th_io_service([&io_service]{io_service.run(); });
    boost::thread th_screen(screen_func, screen_server_ptr);

    screen_func(screen_server_ptr);

	return 0;
}