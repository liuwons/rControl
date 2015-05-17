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
    //boost::shared_ptr<ScreenClient> client(new ScreenClient(io_service, endp, buf));

    boost::shared_ptr<rc::Decode> decode(new rc::Decode(buf));

    printf("starting client\n");
    //boost::thread([client]{client->start(); });


    printf("starting io_service\n");
    //boost::thread([&io_service]{io_service.run(); });

    ControlPanel* panel = new ControlPanel(QRect(50, 50, 1000, 700));
    panel->show();

    printf("starting decode\n");
    decode->set_panel(panel);
    boost::thread([decode]{decode->start(); });


    return a.exec();

}


/*int main()
{
    printf("av start register\n");
    av_register_all();
    printf("av register succeed\n");

    AVPacket pkt;
    av_init_packet(&pkt);

    AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
    printf("av find codec succeed\n");

    AVCodecContext* cctx = avcodec_alloc_context3(codec);

    //cctx->bit_rate = 400000;
    cctx->width = 1366;
    cctx->height = 768;
    //AVRational ar = { 1, 25 };
    //cctx->time_base = ar;
    //cctx->gop_size = 10;
    //cctx->max_b_frames = 1;
    //cctx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->capabilities&CODEC_CAP_TRUNCATED)
        cctx->flags |= CODEC_FLAG_TRUNCATED;


    if (avcodec_open2(cctx, codec, NULL) < 0)
    {
        fprintf(stderr, "Could not open codec\n");
        exit(0);
    }
    else
    {
        printf("av open codec succeed\n");
    }

    AVFrame* frame = av_frame_alloc();
    FILE* f = fopen("test.mpg", "ab+");
    char* buf = new char[10000];

    int fcount = 0;

    while (1)
    {
        int len = fread(buf, 1, 10000, f);

        pkt.size = len;
        pkt.data = (uint8_t*)(buf);

        while (pkt.size > 0)
        {
            int got;
            len = avcodec_decode_video2(cctx, frame, &got, &pkt);
            if (len < 0)
            {
                printf("decode err:%d\n", len);
                return -1;
            }
            if (got)
            {
                fcount++;
                int w = cctx->width;
                int h = cctx->height;
                printf("frame:%d\n", fcount);
            }

            pkt.size -= len;
            pkt.data += len;
        }

    }

}*/



