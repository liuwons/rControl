#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QtGui/QMainWindow>
#include <QRect>
#include <QTcpSocket>
#include <QHostAddress>
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

#include "version.h"
#include "mapthread.h"
#include "cmdthread.h"
#include "values.h"

class ControlPanel : public QWidget
{
    Q_OBJECT

private:
    QImage* image;
    MapThread* mapThread;
    CmdThread* cmdThread;
    int frame_width;
    int frame_height;
    int server_screen_width;
    int server_screen_height;

    uchar translateKey(int key);

public:
    ControlPanel(QRect rect, QWidget *parent = 0);
    ~ControlPanel();

signals:
    void mouseMoveTo(int, int);

public slots:
    void frameChanged(QImage*);
    void changeFrameSize(int, int);
    void gotServerScreenSize(int, int);

    void keyPressSlot(QKeyEvent *);
    void keyReleaseSlot(QKeyEvent *);

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
};

#endif // CONTROLPANEL_H
