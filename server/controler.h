#ifndef CONTROLER_H
#define CONTROLER_H

#include "version.h"
#include "interface.h"
#include "mapthread.h"
#include "cmdthread.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QObject>
#include <QVector>

typedef struct SocketAndThread
{
    QTcpSocket* socket;
    QThread*  thread;
} SocketAndThread;

class Controler : public QObject
{
    Q_OBJECT
private:
    QTcpServer* pCmdServer;
    QTcpServer* pMapServer;

    QVector<SocketAndThread>* vec;

public:
    const static int DATA_HEARER_MAP = 1;    //图像数据
    const static int DATA_HEADER_CMD = 2;    //命令数据
    const static int DATA_HEARER_RES = 3;    //命令结果数据

    const static int MAP_SERVER_PORT = 5648;
    const static int CMD_SERVER_PORT = 5649;

    explicit Controler(QObject *parent = 0);

public slots:
    void newMapClient();
    void newCmdClient();
    void someSocketDisconnected();
};

#endif // CONTROLER_H
