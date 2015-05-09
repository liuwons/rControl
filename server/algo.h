#ifndef ALGO_H
#define ALGO_H

#include "version.h"
#include <QTcpSocket>

void writeAndBlock(QTcpSocket* socket, uchar* buf, int len);
void readAndBlock(QTcpSocket* socket, uchar* buf, int len);

int minv(int, int);

#endif // ALGO_H
