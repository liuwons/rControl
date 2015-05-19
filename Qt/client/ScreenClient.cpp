#include "ScreenClient.h"


ScreenClient::ScreenClient(boost::asio::io_service& ioserv, boost::asio::ip::tcp::endpoint endp, boost::shared_ptr<rc::DataBuffer> buf) : sock_(new boost::asio::ip::tcp::socket(ioserv)), endp_(endp), buf_(new char[1000000]), recv_buffer_(buf)
{

}


void ScreenClient::start()
{
    sock_->async_connect(endp_, boost::bind(&ScreenClient::handle_connect, this, _1));
}


void ScreenClient::handle_connect(const boost::system::error_code& error)
{
    if (!error)
    {
        printf("connect succeed\n");
    }
    else
    {
        printf("connect error\n");
    }

    boost::asio::async_read(*sock_, boost::asio::buffer(tmp_buf, InitInfo::struct_size), boost::bind(&ScreenClient::handle_read_init_info, this, _1));
}

void ScreenClient::handle_read_init_info(const boost::system::error_code& error)
{
    if(error)
    {
        printf("read init info error\n");
        return;
    }
    else
    {

    }

    InitInfo info;
    info.parse(tmp_buf);
    if(!info.valid)
    {
        printf("parse init info failed\n");
        return;
    }
    on_recved_init_info(info);
    boost::asio::async_read(*sock_, boost::asio::buffer(tmp_buf, 10), boost::bind(&ScreenClient::handle_read_len, this, _1));
}

void ScreenClient::handle_read_len(const boost::system::error_code& error)
{
    if (!error)
    {
        //printf("read len succeed:%s\n", len_buf);
    }
    else
    {
        printf("read len error\n");
        exit(0);
    }

    data_len = atoi(tmp_buf);

    boost::asio::async_read(*sock_, boost::asio::buffer(buf_.get(), data_len), boost::bind(&ScreenClient::handle_read_data, this, _1));
}

void ScreenClient::handle_read_data(const boost::system::error_code& error)
{
    if (!error)
    {
        printf("recv:%d\n", data_len);
		FILE* f = fopen("test.avi", "ab");
		fwrite(buf_.get(), 1, data_len, f);
        fclose(f);

        if (recv_buffer_->write(buf_.get(), data_len))
        {
            printf("write buffer succeed:%d\n", data_len);
        }
        else
        {
            printf("write buffer failed\n");
        }
    }
    else
    {
        printf("read data error\n");
        exit(0);
    }

    boost::asio::async_read(*sock_, boost::asio::buffer(tmp_buf, 10), boost::bind(&ScreenClient::handle_read_len, this, _1));
}


ScreenClient::~ScreenClient()
{
    printf("delete client\n");
}
