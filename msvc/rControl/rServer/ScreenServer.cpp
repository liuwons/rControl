#include "ScreenServer.h"

ScreenClient::ScreenClient(shared_ptr<tcp::socket> sock) :m_socket(sock)
{

}

ScreenClient::~ScreenClient()
{
    printf("~ScreenClient\n");
}


void ScreenClient::asyn_deliver(mutable_buffer buf)
{
    printf("ScreenClient::asyn_deliver\n");
    boost::asio::async_write(*m_socket, buffer(buf), boost::bind(&ScreenClient::write_handler, this, _1));
}

void ScreenClient::write_handler(const error_code& ec)
{
	if (ec)
		std::cout << "send failed" << std::endl;
	else
		std::cout << "send succeed" << std::endl;
}

ScreenServer::ScreenServer(io_service &iosev) :m_iosev(iosev), m_acceptor(iosev, tcp::endpoint(tcp::v4(), 1000))
{
}

void ScreenServer::start_listen()
{
    shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    m_acceptor.async_accept(*psocket, boost::bind(&ScreenServer::accept_handler, this, psocket, _1));
}

void ScreenServer::asyn_deliver(mutable_buffer buf)
{
	for (set<shared_ptr<ScreenClient>>::iterator iter = m_clients.begin(); iter != m_clients.end(); iter++)
	{
		(*iter)->asyn_deliver(buf);
	}
}


void ScreenServer::accept_handler(shared_ptr<tcp::socket> psocket, error_code ec)
{
    if (ec)
        return;

    start_listen();
    std::cout << "accpet connect from:" << psocket->remote_endpoint().address() << std::endl;

    shared_ptr<ScreenClient> client(new ScreenClient(psocket));
	m_clients.insert(client);
}
