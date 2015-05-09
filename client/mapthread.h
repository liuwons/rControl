#ifndef MAPTHREAD_H
#define MAPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QImage>
#include <QTime>

#include "version.h"
struct AVFrame;
struct AVPacket;
struct AVCodec;
struct AVCodecContext;

#define INBUF_SIZE 4096000
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096
#define QIMAGE_BUFFER_SIZE 12000000

class MapThread : public QThread
{
    Q_OBJECT

private:
    QTime* time;
    int newF;

    uchar* img_buf;
    QTcpSocket* mapSocket;
    QString     address;
    int         port;
    bool socketConnected;

    int request_width;
    int request_height;

    uchar* recv_buf;
    uchar* frame_buf;
    int    frame_buf_fill;
    int    frame_bytes;
    uchar  cmd_buf[8];
    int    cmd_buf_fill;

    bool cmd_got;
    bool frame_size_setted;

    bool cmd_parsed;
    int subX;
    int subY;
    int subWidth;
    int subHeight;
    int subSize;
    int subFill;

    bool inited;
    AVFrame *frame;
    AVPacket* pkt;
    AVCodec *codec;
    AVCodecContext *c;
    int i, ret, x, y, got_output;

    //QImage* image;

    uchar* inbuf;
#ifdef DEBUG
    FILE* log;
    int readlen;
#endif

    int decode_write_frame(AVCodecContext *avctx, AVFrame *frame, AVPacket *pkt);

public:
    int received_frame_width;
    int received_frame_height;
    QImage*  image;

    MapThread(QString add, int p, int w, int h, QObject *parent = 0);
    void sendRequestSize(int width, int height);
    void getSubWindow();
    void parseCommand();
    void updateFrame();
    
signals:
    void frameGot(QImage*);
    void frameSizeChanged(int, int);
    
public slots:
    void newData();
    void hostConnected();

protected:
    void run();
    
};

#endif // MAPTHREAD_H
