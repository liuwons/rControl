#include "controlpanel.h"
#include "mainwindow.h"

ControlPanel::ControlPanel(QRect rect, QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    image = 0;
    setGeometry(rect);
}

ControlPanel::~ControlPanel()
{

}

void ControlPanel::setImage(char* src, int width, int height)
{
    boost::lock_guard<boost::mutex> lock(img_mutex);
    if(image)
        delete image;
    image = new QImage((uchar*)src, width, height, QImage::Format_RGB888);
}

void ControlPanel::frameChanged(QImage* i)
{
    image = i;
}

void ControlPanel::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
}

void ControlPanel::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void ControlPanel::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
}

void ControlPanel::mouseDoubleClickEvent(QMouseEvent *e)
{
    QWidget::mouseDoubleClickEvent(e);
}

void ControlPanel::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
}

void ControlPanel::paintEvent(QPaintEvent *e)
{
    if(image != 0)
    {
        QPainter p(this);
        p.drawImage(QRect(0, 0, image->width(), image->height()), *image);
        p.end();
    }
    QWidget::paintEvent(e);
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
    QWidget::keyPressEvent(e);
}

void ControlPanel::keyReleaseEvent(QKeyEvent *e)
{
    QWidget::keyReleaseEvent(e);
}

void ControlPanel::keyPressSlot(QKeyEvent *e)
{
    keyPressEvent(e);
}

void ControlPanel::keyReleaseSlot(QKeyEvent *e)
{
    keyReleaseEvent(e);
}
