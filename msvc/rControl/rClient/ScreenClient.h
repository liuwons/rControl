#ifndef RC_SCREENCLIENT_H
#define RC_SCREENCLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_array.hpp>

#include "Buffer.h"

class ScreenClient
{
public:
    ScreenClient(boost::asio::io_service& ioserv, boost::asio::ip::tcp::endpoint endp, boost::shared_ptr<rc::Buffer> buf);
    void start();
    void handle_connect(const boost::system::error_code& error);
    void handle_read_len(const boost::system::error_code& error);
    void handle_read_data(const boost::system::error_code& error);
    ~ScreenClient();

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    boost::asio::ip::tcp::endpoint endp_;
    char len_buf[10];
    boost::shared_array<char> buf_;
    int data_len;
    boost::shared_ptr<rc::Buffer> recv_buffer_;
};


#endif