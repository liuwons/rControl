#include "controlpanel.h"
#include "mainwindow.h"
#include "values.h"

ControlPanel::ControlPanel(QRect rect, QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    server_screen_width  = -1;
    server_screen_height = -1;
    image = 0;
    frame_width  = -1;
    frame_height = -1;
    setGeometry(rect);
    mapThread = new MapThread(addr, 5603, rect.width(), rect.height());
    connect(mapThread, SIGNAL(frameGot(QImage*)), this, SLOT(frameChanged(QImage*)));
    connect(mapThread, SIGNAL(frameSizeChanged(int,int)), this, SLOT(changeFrameSize(int,int)));
    mapThread->start();
    cmdThread = new CmdThread(addr, 5602);
    connect(cmdThread, SIGNAL(setServerScreenSize(int,int)), this, SLOT(gotServerScreenSize(int,int)));
    cmdThread->run();
}

ControlPanel::~ControlPanel()
{

}

void ControlPanel::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
    {
        return;
    }
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
    {
        return;
    }
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    cmdThread->cmdMouseMoveTo(x, y);
}

void ControlPanel::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    if(e->button() == Qt::LeftButton)
    {
        cmdThread->cmdMouseLeftDown(x, y);
    }
    else if(e->button() == Qt::RightButton)
    {
        cmdThread->cmdMouseRightDown(x, y);
    }

}

void ControlPanel::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    if(e->button() == Qt::LeftButton)
    {
        cmdThread->cmdMouseLeftUp(x, y);
    }
    else if(e->button() == Qt::RightButton)
    {
        cmdThread->cmdMouseRightUp(x, y);
    }
}

void ControlPanel::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->pos().x();
    int y = e->pos().y();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    cmdThread->cmdMouseDoubleClick(x, y);
}

void ControlPanel::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
    if(!control)
        return;

    if(server_screen_width < 0 || mapThread->received_frame_width < 0)
        return;
    int x = e->globalX();
    int y = e->globalY();
    if(x >= mapThread->received_frame_width || y >= mapThread->received_frame_height)
        return;
    double off_x = (double)x / mapThread->received_frame_width;
    double off_y = (double)y / mapThread->received_frame_height;
    x = (int)(server_screen_width * off_x);
    y = (int)(server_screen_height * off_y);
    int d = e->delta();
    cmdThread->cmdMouseWheel(d, x, y);
}

void ControlPanel::paintEvent(QPaintEvent *e)
{
#ifdef DEBUG
    fprintf(tlog, "start repaint:%d\n", QDateTime::currentDateTime().msecsTo(dt));
    //fprintf(tlog, "repaint start:%d\n", ttime->elapsed());
    fflush(tlog);
#endif

    if(image != 0)
    {
        QPainter p(this);
        p.drawImage(QRect(0, 0, frame_width, frame_height), *image);
        p.end();
    }
    QWidget::paintEvent(e);
#ifdef DEBUG
    fprintf(tlog, "end repaint:%d\n", QDateTime::currentDateTime().msecsTo(dt));
    //fprintf(tlog, "repaint over:%d\n", ttime->elapsed());
    fflush(tlog);
#endif
}

uchar ControlPanel::translateKey(int key)
{
    int k = key;
    bool legal = true;
    if(k >= Qt::Key_0 && k <= Qt::Key_9)
    {
    }
    else if(k >= Qt::Key_A && k <= Qt::Key_Z)
    {

    }
    else if(k >= Qt::Key_F1 && k <= Qt::Key_F24)
    {
        k &= 0x000000ff;
        k += 0x40;
    }
    else if(k == Qt::Key_Tab)
    {
        k = 0x09;
    }
    else if(k == Qt::Key_Backspace)
    {
        k = 0x08;
    }
    else if(k == Qt::Key_Return)
        k = 0x0d;
    else if(k <= Qt::Key_Down && k >= Qt::Key_Left)
    {
        int off = k - Qt::Key_Left;
        k = 0x25 + off;
    }
    else if(k == Qt::Key_Shift)
        k = 0x10;
    else if(k == Qt::Key_Control)
        k = 0x11;
    else if(k == Qt::Key_Alt)
        k = 0x12;
    else if(k == Qt::Key_Meta)
        k = 0x5b;
    else if(k == Qt::Key_Insert)
        k = 0x2d;
    else if(k == Qt::Key_Delete)
        k = 0x2e;
    else if(k == Qt::Key_Home)
        k = 0x24;
    else if(k == Qt::Key_End)
        k = 0x23;
    else if(k == Qt::Key_PageUp)
        k = 0x21;
    else if(k == Qt::Key_PageDown)
        k = 0x22;
    else if(k == Qt::Key_CapsLock)
        k = 0x14;
    else if(k == Qt::Key_NumLock)
        k = 0x90;
    else if(k == Qt::Key_Space)
        k = 0x20;
    else
        legal = false;
    if(legal == false)
        return 0;
    return k;
}

void ControlPanel::keyPressEvent(QKeyEvent *e)
{
    if(!control)
        return;

    int k = translateKey(e->key());
    if(k == 0)
        return;
    cmdThread->cmdKeyPress(k);
    e->ignore();
}

void ControlPanel::keyReleaseEvent(QKeyEvent *e)
{
    if(!control)
        return;

    int k = translateKey(e->key());
    if(k == 0)
        return;
    cmdThread->cmdKeyRelease(k);
    e->ignore();
}

void ControlPanel::keyPressSlot(QKeyEvent *e)
{
    keyPressEvent(e);
}

void ControlPanel::keyReleaseSlot(QKeyEvent *e)
{
    keyReleaseEvent(e);
}

void ControlPanel::frameChanged(QImage* i)
{
#ifdef DEBUG
    fprintf(tlog, "recv emit:%d\n", QDateTime::currentDateTime().msecsTo(dt));
#endif
    image = i;
    update();
#ifdef DEBUG
    fprintf(tlog, "update over:%d\n", QDateTime::currentDateTime().msecsTo(dt));
    fflush(tlog);
#endif
}

void ControlPanel::changeFrameSize(int w, int h)
{
    frame_width  = w;
    frame_height = h;
}

void ControlPanel::gotServerScreenSize(int w, int h)
{
    server_screen_width  = w;
    server_screen_height = h;
}
