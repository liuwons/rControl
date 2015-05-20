#ifndef RC_SCREENCLIENT_H
#define RC_SCREENCLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/shared_array.hpp>

#include "Buffer.h"
#include "struct.h"

class ScreenClient
{
public:
    ScreenClient(boost::asio::io_service& ioserv, boost::asio::ip::tcp::endpoint endp, boost::shared_ptr<rc::DataBuffer> buf);
    void start();
    void handle_connect(const boost::system::error_code& error);
    void handle_read_init_info(const boost::system::error_code& error);
    void handle_read_len(const boost::system::error_code& error);
    void handle_read_data(const boost::system::error_code& error);
    ~ScreenClient();

    boost::function<void (const boost::system::error_code&)> on_connected;
    boost::function<void (int)> on_recved_data;
    boost::function<void (InitInfo&)> on_recved_init_info;

private:
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    boost::asio::ip::tcp::endpoint endp_;
    char tmp_buf[2048];
    int data_len;
    boost::shared_array<char> buf_;
    boost::shared_ptr<rc::DataBuffer> recv_buffer_;
};


#endif
