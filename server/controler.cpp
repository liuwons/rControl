#include "controler.h"

Controler::Controler(QObject *parent) :
    QObject(parent)
{

    vec = new QVector<SocketAndThread>;
    pMapServer = new QTcpServer;
    pMapServer->listen(QHostAddress::Any, MAP_SERVER_PORT);
    qDebug()<<"map server listening"<<endl;
    connect(pMapServer, SIGNAL(newConnection()), this, SLOT(newMapClient()));

    pCmdServer = new QTcpServer;
    pCmdServer->listen(QHostAddress::Any, CMD_SERVER_PORT);
    qDebug()<<"cmd server listening"<<endl;
    connect(pCmdServer, SIGNAL(newConnection()), this, SLOT(newCmdClient()));
}

/**
  *每当有一个新连接时，启动一个线程单独为之服务
  */
void Controler::newMapClient()
{
    QTcpSocket* clientMapSocket = pMapServer->nextPendingConnection();

    qDebug()<<"new map connection:"<<clientMapSocket->peerAddress().toString()<<endl;

    MapThread* mapThread = new MapThread(clientMapSocket);
    connect(clientMapSocket, SIGNAL(disconnected()), this, SLOT(someSocketDisconnected()));

    SocketAndThread st = {clientMapSocket, mapThread};
    vec->append(st);

    mapThread->start();
}

void Controler::newCmdClient()
{
    QTcpSocket* clientCmdSocket = pCmdServer->nextPendingConnection();

    qDebug()<<"new cmd connection:"<<clientCmdSocket->peerAddress().toString()<<endl;

    CmdThread* cmdThread = new CmdThread(clientCmdSocket);
    connect(clientCmdSocket, SIGNAL(disconnected()), this, SLOT(someSocketDisconnected()));

    SocketAndThread st = {clientCmdSocket, cmdThread};
    vec->append(st);

    cmdThread->start();
}

void Controler::someSocketDisconnected()
{
    for(int i = 0; i < vec->size(); i ++)
    {
        if(vec->at(i).socket->state() == QAbstractSocket::UnconnectedState)
        {
            if(vec->at(i).socket->isOpen())
                vec->at(i).socket->close();
            vec->at(i).thread->quit();
            connect(vec->at(i).thread, SIGNAL(finished()), vec->at(i).thread, SLOT(deleteLater()));
            vec->remove(i);
            i --;
        }
    }
}

