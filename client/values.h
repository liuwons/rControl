#ifndef VALUES_H
#define VALUES_H

#include <stdio.h>
#include <QDateTime>
#include "version.h"

class QTime;

extern int screen_width;
extern int screen_height;

extern bool control;

extern QString addr;
extern int rq_width;
extern int rq_height;

extern FILE* tlog;
extern QTime* ttime;

#ifdef DEBUG
extern QDateTime dt;
#endif

#endif // VALUES_H
