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

#include "mapthread.h"
#include "algo.h"
#include "consts.h"
#include "values.h"


MapThread::MapThread(QString add, int p, int w, int h, QObject *parent) :
    QThread(parent)
{
    img_buf = new uchar[QIMAGE_BUFFER_SIZE];
    image = 0;

#ifdef DEBUG
    log = fopen("log.txt", "w");
    readlen = 0;
#endif

    inbuf = new uint8_t[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    inited = false;
    time = new QTime;
    newF = 1;

    mapSocket = new QTcpSocket;
    address   = add;
    port      = p;
    socketConnected = false;

    request_width  = w;
    request_height = h;
    connect(mapSocket, SIGNAL(connected()), this, SLOT(hostConnected()));
    connect(mapSocket, SIGNAL(readyRead()), this, SLOT(newData()));

    recv_buf  = new uchar[BLOCK_WIDTH * BLOCK_HEIGHT * 3];
    frame_buf = new uchar[20000000];
    frame_buf_fill = 0;
    cmd_buf_fill = 0;
    image = 0;

    cmd_got = false;
    frame_size_setted = false;
    received_frame_width = -1;
    received_frame_height = -1;

    cmd_parsed = false;
    subX = 0;
    subY = 0;
    subWidth  = 0;
    subHeight = 0;
    subSize   = 0;
    subFill   = 0;

    mapSocket->connectToHost(address, port);
    if(mapSocket->state() == QAbstractSocket::ConnectedState)
    {
        socketConnected = true;
    }
}

void MapThread::run()
{
    QThread::run();
}

void MapThread::hostConnected()
{
    sendRequestSize(request_width, request_height);
}

void MapThread::sendRequestSize(int width, int height)
{
    request_width = width;
    request_height = height;
    uchar uc[4];
    uc[0] = width / 0x100;
    uc[1] = width % 0x100;
    uc[2] = height / 0x100;
    uc[3] = height % 0x100;
    writeAndBlock(mapSocket, uc, 4);
}

int MapThread::decode_write_frame(AVCodecContext *avctx, AVFrame *frame, AVPacket *pkt)
{
    int len, got_frame;
    len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);

    if (got_frame)
    {
#ifdef DEBUG
        fprintf(tlog, "get frame:%d\n", QDateTime::currentDateTime().msecsTo(dt));
        //fprintf(tlog, "got frame:%d\n", ttime->elapsed());
#endif

        if(image == 0)
            image = new QImage(img_buf, avctx->width, avctx->height, QImage::Format_RGB888);

        received_frame_width = avctx->width;
        received_frame_height = avctx->height;

        for(int h = 0; h < avctx->height; h++)
        {
            for(int w = 0; w < avctx->width; w ++)
            {
                int hh = h >> 1;
                int ww = w >> 1;
                int Y = frame->data[0][h * frame->linesize[0] + w];
                int U = frame->data[1][hh * (frame->linesize[1]) + ww];
                int V = frame->data[2][hh * (frame->linesize[2]) + ww];

                int C = Y - 16;
                int D = U - 128;
                int E = V - 128;

                int r = 298 * C           + 409 * E + 128;
                int g = 298 * C - 100 * D - 208 * E + 128;
                int b = 298 * C + 516 * D           + 128;

                r = qBound(0, r >> 8, 255);
                g = qBound(0, g >> 8, 255);
                b = qBound(0, b >> 8, 255);

                QRgb rgb = qRgb(r, g, b);
                image->setPixel(QPoint(w, h), rgb);
            }
        }
#ifdef DEBUG
        fprintf(tlog, "emit frame:%d\n", QDateTime::currentDateTime().msecsTo(dt));
        //fprintf(tlog, "emit frame:%d\n", ttime->elapsed());
#endif
        emit frameGot(image);
    }
    if (pkt->data) {
        pkt->size -= len;
        pkt->data += len;
    }

    return 0;
}

void MapThread::newData()
{
    if(!inited)
    {
        avcodec_register_all();

        pkt = new AVPacket;
        av_init_packet(pkt);

        memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);

        codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
        if (!codec)
        {
#ifdef DEBUG
            fprintf(log, "Codec not found\n");
            fflush(log);
#endif
            exit(1);
        }
        c = avcodec_alloc_context3(codec);

        if (!c) {
#ifdef DEBUG
            fprintf(log, "Could not allocate video codec context\n");
            fflush(log);
#endif
            exit(1);
        }

        av_opt_set(c->priv_data, "preset", "superfast", 0);
        av_opt_set(c->priv_data, "tune", "zerolatency", 0);

        c->delay = 0;

        if(codec->capabilities&CODEC_CAP_TRUNCATED)
            c->flags|= CODEC_FLAG_TRUNCATED;
        if (avcodec_open2(c, codec, NULL) < 0) {
#ifdef DEBUG
            fprintf(log, "Could not open codec\n");
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

        inited = true;
    }
    while(true)
    {
        int nread = mapSocket->read((char*)inbuf, INBUF_SIZE);
#ifdef DEBUG
        readlen += nread;
        fprintf(tlog, "recv time:%d\n", QDateTime::currentDateTime().msecsTo(dt));
        fprintf(tlog, "recv all:%d\n", readlen);
        fflush(tlog);
#endif

        if(nread <= 0)
            break;

        av_init_packet(pkt);
        pkt->size = nread;
        pkt->data = inbuf;
        while (pkt->size > 0)
        {
            if (decode_write_frame(c, frame, pkt) < 0)
                exit(1);
        }
        av_free_packet(pkt);
    }
}
