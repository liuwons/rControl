#include <iostream>
#include <string>
#include <set>
using std::set;

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
using namespace boost::asio;
using ip::tcp;
using boost::system::error_code;
using boost::shared_ptr;

class ScreenClient:public boost::enable_shared_from_this<ScreenClient>
{
public:
    ScreenClient(shared_ptr<tcp::socket> sock);
	virtual ~ScreenClient();
	void asyn_deliver(mutable_buffer buf);
    void write_handler(const error_code& ec);
    shared_ptr<tcp::socket> m_socket;
    int m_id;
};


class ScreenServer
{
public:
    ScreenServer(io_service &iosev);

    void start_listen();
    void accept_handler(shared_ptr<tcp::socket> psocket, error_code ec);

    void asyn_deliver(mutable_buffer buf);

private:
    io_service &m_iosev;
    tcp::acceptor m_acceptor;
	set<shared_ptr<ScreenClient>> m_clients;
};