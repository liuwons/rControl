#include <QString>
#include <QDateTime>
#include <stdio.h>

class QTime;

int screen_width  = 0;
int screen_height = 0;

bool control = true;

QString addr;
int rq_width;
int rq_height;

FILE* tlog;
QTime* ttime;

QDateTime dt;
