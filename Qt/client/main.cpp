#include <QtGui/QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QTime>

#include "mainwindow.h"

#include <stdio.h>
#include <stdlib.h>

#include <boost/thread.hpp>

#include "ScreenClient.h"
#include "Buffer.h"
#include "decode.h"
#include "struct.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    printf("start\n");

    int port = 1000;

    if (argc > 1)
        port = atoi(argv[1]);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endp(boost::asio::ip::address::from_string("127.0.0.1"), port);
    boost::shared_ptr<rc::DataBuffer> buf(new rc::DataBuffer(50 * 1024 * 1024));

    boost::shared_ptr<rc::Decode> decode(new rc::Decode(buf));

    boost::shared_ptr<ScreenClient> client(new ScreenClient(io_service, endp, buf));
    client->on_recved_init_info = [decode](InitInfo& info)
    {
            printf("client set init info\n");
        decode->init(info);
    };

    printf("starting client\n");
    boost::thread([client]{client->start(); });

    ControlPanel* panel = new ControlPanel(QRect(50, 50, 1000, 700));
    panel->show();

    printf("starting decode\n");
    decode->set_panel(panel);
    boost::thread([decode]{decode->start(); });

    printf("starting io_service\n");
    boost::thread([&io_service]{io_service.run(); });

    return a.exec();
}
