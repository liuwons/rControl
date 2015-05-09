#include "httpsender.h"

HttpSender::HttpSender(QObject *parent) : QObject(parent)
{
    http = new QHttp();
    connect(http, SIGNAL(done(bool)), this, SLOT(done(bool)));
}


void HttpSender::post(QString path, QByteArray ba)
{
    http->post(path, ba);
}


void HttpSender::done(bool error)
{
    if(error)
    {
#ifdef DEBUG
        qDebug()<<"error happened!";
#endif
        return;
    }
    QByteArray ba = http->readAll();
    http->close();
#ifdef DEBUG
    qDebug()<<"recv:"<<ba;
#endif
}


void HttpSender::setHost(QString host)
{
    http->setHost(host);
}
