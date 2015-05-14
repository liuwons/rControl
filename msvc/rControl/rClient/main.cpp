#include <stdio.h>
#include <stdlib.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_array.hpp>


class Client
{
public:
    Client(boost::asio::io_service& ioserv, boost::asio::ip::tcp::endpoint endp) : sock_(new boost::asio::ip::tcp::socket(ioserv)), endp_(endp), buf_(new char[1000000])
    {
        
    }

    void start()
    {
        sock_->async_connect(endp_, boost::bind(&Client::handle_connect, this, _1));
    }
    void handle_connect(const boost::system::error_code& error)
    {
        if (!error)
        {
            printf("connect succeed\n");
        }
        else
        {
            printf("connect error\n");
        }

        boost::asio::async_read(*sock_, boost::asio::buffer(len_buf, 10), boost::bind(&Client::handle_read_len, this, _1));
    }

    void handle_read_len(const boost::system::error_code& error)
    {
        if (!error)
        {
            printf("read len succeed:%s\n", len_buf);
        }
        else
        {
            printf("read len error\n");
            exit(0);
        }

        int data_len = atoi(len_buf);

        boost::asio::async_read(*sock_, boost::asio::buffer(buf_.get(), data_len), boost::bind(&Client::handle_read_len, this, _1, data_len));
    }

    void handle_read_data(const boost::system::error_code& error, int data_len)
    {
        if (!error)
        {
            printf("read data succeed, len:%d\n", data_len);
        }
        else
        {
            printf("read len error\n");
            exit(0);
        }

        boost::asio::async_read(*sock_, boost::asio::buffer(len_buf, 10), boost::bind(&Client::handle_read_len, this, _1));
    }


    ~Client()
    {
        printf("delete client\n");
    }

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    boost::asio::ip::tcp::endpoint endp_;
    char len_buf[10];
    boost::shared_array<char> buf_;
};



int main(int argc, char** argv)
{
	int port = 1000;

	if (argc > 1)
		port = atoi(argv[1]);

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endp(boost::asio::ip::address::from_string("127.0.0.1"), port);

    boost::shared_ptr<Client> client(new Client(io_service, endp));
    client->start();

    io_service.run();

    return 0;
}