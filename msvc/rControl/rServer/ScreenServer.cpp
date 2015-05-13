#include "ScreenServer.h"

ScreenClient::ScreenClient(shared_ptr<tcp::socket> sock, int id) :m_socket(sock), m_id(id)
{

}

void ScreenClient::asyn_send(const_buffer& buf)
{

}

ScreenServer::ScreenServer(io_service &iosev) :m_iosev(iosev), m_acceptor(iosev, tcp::endpoint(tcp::v4(), 1000))
{
}

void ScreenServer::start_listen()
{
    shared_ptr<tcp::socket> psocket(new tcp::socket(m_iosev));
    m_acceptor.async_accept(*psocket, boost::bind(&ScreenServer::accept_handler, this, psocket, _1));
}


void ScreenServer::start_send_screen()
{

    boost::shared_ptr<std::string> pstr(new std::string("hello async world!"));
    psocket->async_write_some(buffer(*pstr), boost::bind(&ScreenServer::write_handler, this, pstr, _1, _2));
}

void ScreenServer::accept_handler(shared_ptr<tcp::socket> psocket, error_code ec)
{
    if (ec)
        return;

    start_listen();
    std::cout << "accpet connect from:" << psocket->remote_endpoint().address() << std::endl;

    shared_ptr<ScreenClient> client(new ScreenClient(psocket));
    m_clients.push_back(client);
}

void ScreenServer::write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred)
{
    if (ec)
        std::cout << "send failed" << std::endl;
    else
        std::cout << *pstr << "send succeed" << std::endl;
}