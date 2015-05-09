#include "interface.h"

int screen_width = 0;
int screen_height = 0;
//int i = 0;

Interface::Interface()
{
}

QPoint Interface::transformCoordinate(QPoint p)
{
    int x = (int)((double)p.x() / screen_width * 65535);
    int y = (int)((double)p.y() / screen_height * 65535);
    return QPoint(x, y);
}

void Interface::mouseMoveTo(int x, int y)
{
    QPoint p = transformCoordinate(QPoint(x, y));
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE, p.x(), p.y(), 0, 0);
}

void Interface::mouseLeftDown(int x, int y)
{
    QPoint p;
    if(x < 0 || y < 0)
    {
        p = getCursorPos();
    }
    else
    {
        p = transformCoordinate(QPoint(x, y));
    }
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN, p.x(), p.y(), 0, 0);
}

void Interface::mouseLeftUp(int x, int y)
{
    QPoint p;
    if(x < 0 || y < 0)
    {
        p = getCursorPos();
    }
    else
    {
        p = transformCoordinate(QPoint(x, y));
    }
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP, p.x(), p.y(), 0, 0);
}

void Interface::mouseRightDown(int x, int y)
{
    QPoint p;
    if(x < 0 || y < 0)
    {
        p = getCursorPos();
    }
    else
    {
        p = transformCoordinate(QPoint(x, y));
    }
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN, p.x(), p.y(), 0, 0);
}

void Interface::mouseRightUp(int x, int y)
{
    QPoint p;
    if(x < 0 || y < 0)
    {
        p = getCursorPos();
    }
    else
    {
        p = transformCoordinate(QPoint(x, y));
    }
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP, p.x(), p.y(), 0, 0);
}

void Interface::mouseWheel(int delta, int x, int y)
{
    QPoint p;
    if(x < 0 || y < 0)
    {
        p = getCursorPos();
    }
    else
    {
        p = transformCoordinate(QPoint(x, y));
    }
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_WHEEL, p.x(), p.y(), delta, 0);
}

QPoint Interface::getCursorPos()
{
    return QCursor::pos();
}

QPixmap Interface::grapScreen()
{
    return QPixmap::grabWindow(QApplication::desktop()->winId());
}

void Interface::setCursorPos(int x, int y)
{
    QCursor::setPos(x, y);
}

void Interface::keyPressed(uchar key)
{
    keybd_event(key, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
}

void Interface::keyReleased(uchar key)
{
    keybd_event(key, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}
