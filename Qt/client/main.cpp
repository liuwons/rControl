#include <QtGui/QApplication>
#include <QRect>
#include <QDesktopWidget>
#include <QTime>

#include <stdio.h>
#include "mainwindow.h"


#include <stdio.h>
#include <stdlib.h>

#include <boost/thread.hpp>

#include "ScreenClient.h"
#include "Buffer.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    printf("start\n");

    int port = 1000;

    if (argc > 1)
        port = atoi(argv[1]);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endp(boost::asio::ip::address::from_string("127.0.0.1"), port);

    boost::shared_ptr<rc::DataBuffer> buf(new rc::DataBuffer(5 * 1024 * 1024));
    boost::shared_ptr<ScreenClient> client(new ScreenClient(io_service, endp, buf));

    boost::shared_ptr<rc::Decode> decode(new rc::Decode(buf));

    printf("starting client\n");
    client->start();

    printf("starting io_service\n");
    boost::thread([&io_service]{io_service.run(); });

    ControlPanel* panel = new ControlPanel(QRect(50, 50, 1000, 700));
    panel->show();

    printf("starting decode\n");
    decode->set_panel(panel);
    decode->start();

    boost::thread([decode]{decode->start(); });


    /*QDesktopWidget* deskWidget = a.desktop();
    QRect screenRect  = deskWidget->screenGeometry();
    int screen_width = screenRect.width();
    int screen_height = screenRect.height();
    MainWindow w(screen_width/4, screen_height/4, screen_width/2, screen_height/2);
    w.show();*/


    return a.exec();

}



