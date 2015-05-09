#ifndef CMDTHREAD_H
#define CMDTHREAD_H

#include "version.h"

#include <QThread>
#include <QTcpSocket>

class CmdThread : public QThread
{
    Q_OBJECT

private:
    QTcpSocket*  cmdSocket;
    int   cmd_buf_fill;
    uchar cmd_buf[16];             //每条命令占16字节

public:
    explicit CmdThread(QTcpSocket* socket, QObject *parent = 0);
    
signals:
    
public slots:
    void newData();
    void newCommand();

    void cmdMouseMoveTo();
    void cmdMouseLeftDown();
    void cmdMouseLeftUp();
    void cmdMouseRightDown();
    void cmdMouseRightUp();
    void cmdMouseWheel();
    void cmdScreenSize();
    void cmdMouseDoubleClick();

    void cmdKeyPressed();
    void cmdKeyReleased();

protected:
    void run();
    
};

#endif // CMDTHREAD_H
