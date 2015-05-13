#include <iostream>
#include <string>
#include <deque>
#include <vector>
using std::vector;

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;
using boost::shared_ptr;


class ScreenClient
{
public:
    ScreenClient(shared_ptr<tcp::socket> sock, int id);
    void asyn_send(const_buffer& buf);
private:
    shared_ptr<tcp::socket> m_socket;
    int m_id;
};

class ScreenClientCluster
{
public:
    ScreenClientCluster();
    void add_client(shared_ptr<ScreenClient> client);
    void asyn_send(const_buffer& buf);
private:
    vector<shared_ptr<ScreenClient>> m_clients;
};

class ScreenServer
{
public:
    ScreenServer(io_service &iosev);

    void start_listen();
    void start_send_screen();

    void accept_handler(shared_ptr<tcp::socket> psocket, error_code ec);
    void write_handler(boost::shared_ptr<std::string> pstr, error_code ec, size_t bytes_transferred);

    void asyn_send(const_buffer& buf);

private:
    io_service &m_iosev;
    tcp::acceptor m_acceptor;
    shared_ptr<ScreenClientCluster> cluster;
};