#ifndef HTTPSENDER_H
#define HTTPSENDER_H

#include "version.h"

#include <QObject>
#include <QHttp>
#include <QString>
#include <QByteArray>
#include <QDebug>

class HttpSender : public QObject
{
    Q_OBJECT

private:
    QHttp* http;

public:
    explicit HttpSender(QObject *parent = 0);
    void post(QString path, QByteArray ba);
    void setHost(QString host);
    
signals:
    
public slots:
    void done(bool);
    
};

#endif // HTTPSENDER_H
