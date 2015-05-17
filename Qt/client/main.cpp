/*#include <QtGui/QApplication>
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

}*/


extern "C"
{
#include <libavformat/avformat.h>
}

int io_get_data(void *opaque, uint8_t *buf, int buf_size)
{
    return 0;
}

int main(int argc, char** argv)
{
    av_register_all();
    int AV_IO_BUF_LEN =5 * 1024 * 1024;
    AVFormatContext* fctx = avformat_alloc_context();

    char* av_buf = new char[AV_IO_BUF_LEN];
    AVIOContext* avio = avio_alloc_context((unsigned char*)av_buf, AV_IO_BUF_LEN, 0, 0, io_get_data, NULL, NULL);
    fctx->pb = avio;

    int err;

    err = avformat_open_input(&fctx, NULL, NULL, NULL);

    if (err != 0)
    {
        printf("open input failed:%d\n", err);
        return 0;
    }


    err = avformat_find_stream_info(fctx, NULL);

    printf("\n\nfind stream info:%d, streams:%d\n\n", err, fctx->nb_streams);

    if (err != 0)
    {
        return 0;
    }

    int video_id = -1;
    for (int i = 0; i < fctx->nb_streams; i++)    //区分视频流和音频流
    {
        if (fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) //找到视频流，这里也可以换成音频
        {
            video_id = i;
            break;
        }
    }

    printf("video_id:%d\n", video_id);

    AVCodec* codec = avcodec_find_decoder(fctx->streams[video_id]->codec->codec_id);
    AVCodecContext* cctx = fctx->streams[video_id]->codec;
    err = avcodec_open2(cctx, codec, NULL);    // 打开解码器

    printf("open codec:%d\n", err);

    if (err != 0)
    {
        return 0;
    }

    int width = cctx->width;
    int height = cctx->height;

    printf("width:%d, height:%d\n", width, height);

    AVRational frame_rate = fctx->streams[video_id]->r_frame_rate;

    AVPicture* pic = new AVPicture;
    avpicture_alloc(pic, PIX_FMT_RGB24, width, height);

    return 0;
}


