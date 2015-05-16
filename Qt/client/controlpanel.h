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

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

class ControlPanel : public QWidget
{
    Q_OBJECT

private:
    QImage* image;
    boost::mutex img_mutex;

    uchar translateKey(int key);

public:
    ControlPanel(QRect rect, QWidget *parent = 0);
    ~ControlPanel();

    void setImage(char* src, int width, int height);

signals:
    void mouseMoveTo(int, int);

public slots:
    void frameChanged(QImage*);

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
