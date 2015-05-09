#ifndef CMDTHREAD_H
#define CMDTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QMetaType>

#include "version.h"
#include "consts.h"
#include "algo.h"

class CmdThread : public QObject
{
    Q_OBJECT
private:
    QTcpSocket* cmdSocket;
    QString     address;
    int         port;
    bool socketConnected;

    uchar cmd_buf[8];
    int   cmd_buf_fill;
public:
    CmdThread(QString add, int p, QObject* parent = 0);
    void connectToServer();
    void reconnectToServer();


signals:
    void setServerScreenSize(int, int);
    
public slots:
    void connectError(QAbstractSocket::SocketError);
    void connectSucceed();
    void newData();
    void newCommand();
    void cmdMouseMoveTo(int x, int y);
    void cmdMouseLeftDown(int x, int y);
    void cmdMouseLeftUp(int x, int y);
    void cmdMouseRightDown(int x, int y);
    void cmdMouseRightUp(int x, int y);
    void cmdMouseWheel(int delta, int x, int y);
    void cmdScreenSize();
    void cmdMouseDoubleClick(int x, int y);

    void cmdKeyPress(uchar key);
    void cmdKeyRelease(uchar key);


    void run();
    
};

Q_DECLARE_METATYPE(QAbstractSocket::SocketError);

#endif // CMDTHREAD_H
