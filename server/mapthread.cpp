#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}

#include "version.h"
#include "mapthread.h"
#include "interface.h"
#include "algo.h"
#include "consts.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096


/**
  *图像传输线程
  *建立一个双通道的tcp socket
  *启动一个定时器
  */
MapThread::MapThread(QTcpSocket* socket, QObject *parent) :
    QThread(parent)
{
#ifdef DEBUG
    dt = QDateTime::fromString("2014-05-31 00:00:00", "yyyy-MM-dd HH:mm:ss");
    log = fopen("log.txt", "w");
    time = new QTime;
    time->start();
#endif

    sent_img_buf  = 0;
    curt_img_buf  = 0;
    send_data_buf = 0;
    cmd_buf_fill = 0;
    mapSocket = socket;
    interval    = 100;
    started = false;
    inited = false;

    connect(mapSocket, SIGNAL(readyRead()), this, SLOT(newData()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendFrame()));
    timer->start(interval);
}


void MapThread::run()
{
    QThread::run();
}

/**
  *当tcp socket有新数据到达时运行此函数
  */
void MapThread::newData()
{
    while(true)
    {
        int r = mapSocket->read((char*)(cmd_buf + cmd_buf_fill), 4 - cmd_buf_fill);
        if(r <= 0)
            return;
        cmd_buf_fill += r;
        if(cmd_buf_fill == 4)
        {
            newCommand();
            cmd_buf_fill = 0;
        }
    }

}

/**
  *从socket里提取出一条命令时启动
  *目前的命令只有设置目的设备期望图像大小
  */
void MapThread::newCommand()
{
    uchar* uc = (uchar*)cmd_buf;
    uint width = uc[0];
    width = width << 8;
    width += uc[1];
    uint height = uc[2];
    height = height << 8;
    height += uc[3];

    dest_width = width;
    dest_height = height;
    if(sent_img_buf != 0)
    {
        delete[] sent_img_buf;
        sent_img_buf = 0;
    }
    sent_img_buf = new uchar[dest_width * dest_height * 3];
    memset(sent_img_buf, 0, dest_width * dest_height * 3);

    if(curt_img_buf != 0)
    {
        delete[] curt_img_buf;
        curt_img_buf = 0;
    }
    curt_img_buf = new uchar[dest_width * dest_height * 3];

    if(send_data_buf != 0)
    {
        delete[] send_data_buf;
        send_data_buf = 0;
    }
    send_data_buf = new uchar[dest_width * dest_height * 3 * 2];

    double scale_x = (double)dest_width / screen_width;
    double scale_y = (double)dest_height / screen_height;
    if(scale_x < scale_y)
        scaleby = SCALE_BY_WIDTH;
    else
        scaleby = SCALE_BY_HEIGHT;

    started = true;
}

/**
  *发送一帧图像
  *前四字节为所发送图像的尺寸，从低字节到高字节依次为width_high, width_low, height_high, height_low
  *后面为图像数据，每个像素3个字节，依次为RGB通道
  */
void MapThread::sendFrame()
{
    if(!started)
        return;

    if(!inited)
    {
        avcodec_register_all();

        c= NULL;
        pkt = new AVPacket;
        i = 0;

#ifdef DEBUG
        fprintf(log, "Encode video file %s\n", "test.mpg");
        fflush(log);
#endif
        /* find the mpeg1 video encoder */
        codec = avcodec_find_encoder(AV_CODEC_ID_MPEG1VIDEO);

        if (codec == 0)
        {
#ifdef DEBUG
            fprintf(log, "Codec not found\n");
            fflush(log);
#endif
            exit(1);
        }

        c = avcodec_alloc_context3(codec);
        if (!c)
        {
#ifdef DEBUG
            fprintf(log, "Could not allocate video codec context\n");
            fflush(log);
#endif
            exit(1);
        }
        //c->bit_rate = 400000;
        c->width = dest_width;
        c->height = dest_height;

        AVRational ar = {1,25};
        c->time_base = ar;
        c->gop_size = 100;
        c->max_b_frames = 0;
        c->delay = 0;
        c->pix_fmt = AV_PIX_FMT_YUV420P;

        //av_opt_set(c->priv_data, "preset", "slow", 0);

        av_opt_set(c->priv_data, "preset", "superfast", 0);
        av_opt_set(c->priv_data, "tune", "zerolatency", 0);

        int re = avcodec_open2(c, codec, NULL);
        av_opt_set(c->priv_data, "tune", "zerolatency", 0);
        if (re < 0) {
#ifdef DEBUG
            fprintf(log, "Could not open codec:%d\n", re);
            fflush(log);
#endif
            exit(1);
        }

        frame = av_frame_alloc();
        if (!frame) {
#ifdef DEBUG
            fprintf(log, "Could not allocate video frame\n");
            fflush(log);
#endif
            exit(1);
        }
        frame->format = c->pix_fmt;
        frame->width  = c->width;
        frame->height = c->height;

        ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height, c->pix_fmt, 32);
        if (ret < 0) {
#ifdef DEBUG
            fprintf(log, "Could not allocate raw picture buffer\n");
            fflush(log);
#endif
            exit(1);
        }
        inited = true;
    }


    if(mapSocket == 0)
    {
#ifdef DEBUG
        qDebug()<<"null socket"<<endl;
#endif
        return;
    }
    else if(mapSocket->isOpen() == false)
    {
        return;
    }
    else if(mapSocket->isWritable() == false)
    {
        return;
    }
#ifdef DEBUG
    fprintf(log, "start cap:%d\n", QDateTime::currentDateTime().msecsTo(dt));
#endif
    QImage image = Interface::grapScreen().toImage();
    image = image.scaled(QSize(dest_width, dest_height));

#ifdef DEBUG
    fprintf(log, "end cap:%d\n", QDateTime::currentDateTime().msecsTo(dt));
    //fprintf(log, "cap:%d\n", time->elapsed());
    fflush(log);
#endif


    av_init_packet(pkt);
    pkt->data = NULL;    // packet data will be allocated by the encoder
    pkt->size = 1000000;

    for (int h = 0; h < c->height; h++)
    {
        for (int w = 0; w < c->width; w++)
        {
            QRgb rgb = image.pixel(w, h);

            int r = qRed(rgb);
            int g = qGreen(rgb);
            int b = qBlue(rgb);

            int dy = ((66*r + 129*g + 25*b) >> 8) + 16;
            int du = ((-38*r + -74*g + 112*b) >> 8) + 128;
            int dv = ((112*r + -94*g + -18*b) >> 8) + 128;

            uchar yy = (uchar)dy;
            uchar uu = (uchar)du;
            uchar vv = (uchar)dv;

            frame->data[0][h * frame->linesize[0] + w] = yy;

            if(h % 2 == 0 && w % 2 == 0)
            {
                frame->data[1][h/2 * (frame->linesize[1]) + w/2] = uu;
                frame->data[2][h/2 * (frame->linesize[2]) + w/2] = vv;
            }

        }
    }

    frame->pts = i;

    ret = avcodec_encode_video2(c, pkt, frame, &got_output);

    if (ret < 0)
    {
#ifdef DEBUG
        fprintf(log, "Error encoding frame\n");
        fflush(log);
#endif
        exit(1);
    }

    if (got_output)
    {
#ifdef DEBUG
        fprintf(log, "start send:%d\n", QDateTime::currentDateTime().msecsTo(dt));
#endif
        int ss = pkt->size;
#ifdef DEBUG
        qDebug()<<"ss:"<<ss;
        fprintf(log, "size:%d\n", ss);
#endif
        writeAndBlock(mapSocket, pkt->data, ss);
        mapSocket->flush();
#ifdef DEBUG
        fprintf(log, "end send:%d\n", QDateTime::currentDateTime().msecsTo(dt));
#endif

        av_free_packet(pkt);
    }

    i ++;
}


void MapThread::setSendInterval(int i)
{
    timer->setInterval(i);
}

void MapThread::quit()
{
    if(timer)
        timer->stop();
    QThread::quit();
}

MapThread::~MapThread()
{
    if(timer)
    {
        if(timer->isActive())
        {
            timer->stop();
        }
        delete timer;
        timer = 0;
    }
    if(mapSocket)
    {
        if(mapSocket->isOpen())
        {
            mapSocket->close();
        }
        delete mapSocket;
        mapSocket = 0;
    }
    if(sent_img_buf != 0)
        delete[] sent_img_buf;
    if(curt_img_buf != 0)
        delete[] curt_img_buf;
    if(send_data_buf != 0)
        delete[] send_data_buf;
}
