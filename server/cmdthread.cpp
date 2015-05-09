#include "cmdthread.h"
#include "interface.h"
#include "consts.h"
#include "algo.h"

CmdThread::CmdThread(QTcpSocket* socket, QObject *parent) :
    QThread(parent)
{
    cmdSocket = socket;
    cmd_buf_fill = 0;
    connect(cmdSocket, SIGNAL(readyRead()), this, SLOT(newData()));
}

void CmdThread::run()
{
    QThread::run();
}

/**
  *当tcp socket有新数据到达时运行此函数
  */
void CmdThread::newData()
{
    while(true)
    {
        int r = cmdSocket->read((char*)(cmd_buf + cmd_buf_fill), 8 - cmd_buf_fill);
        if(r <= 0)
            return;
        cmd_buf_fill += r;
        if(cmd_buf_fill == 8)
        {
            newCommand();
            cmd_buf_fill = 0;
        }
    }

}

void CmdThread::newCommand()
{
    int cmd = cmd_buf[0];
    switch(cmd)
    {
        case CMD_MOUSE_MOVE_TO:
            cmdMouseMoveTo();
            break;
        case CMD_MOUSE_LEFT_DOWN:
            cmdMouseLeftDown();
            break;
        case CMD_MOUSE_LEFT_UP:
            cmdMouseLeftUp();
            break;
        case CMD_MOUSE_RIGHT_DOWN:
            cmdMouseRightDown();
            break;
        case CMD_MOUSE_RIGHT_UP:
            cmdMouseRightUp();
            break;
        case CMD_MOUSE_WHEEL:
            cmdMouseWheel();
            break;
        case CMD_GET_SCREEN_SIZE:
            cmdScreenSize();
            break;
        case CMD_MOUSE_DOUBLE_CLICK:
            cmdMouseDoubleClick();
            break;
        case CMD_KEY_PRESS:
            cmdKeyPressed();
            break;
        case CMD_KEY_RELEASE:
            cmdKeyReleased();
            break;
    }
}

//buf[1] -> key code
void CmdThread::cmdKeyPressed()
{
    uchar c = cmd_buf[1];
    Interface::keyPressed(c);
}

//buf[1] -> key code
void CmdThread::cmdKeyReleased()
{
    uchar c = cmd_buf[1];
    Interface::keyReleased(c);
}

/**
  *移动鼠标到点x, y
  * 1 xh, 2 xl, 3 yh, 4yl
  */
void CmdThread::cmdMouseMoveTo()
{
    int x = cmd_buf[1];
    x = x << 8;
    x += cmd_buf[2];
    int y = cmd_buf[3];
    y = y << 8;
    y += cmd_buf[4];
    //qDebug()<<"mouse move to:"<<x<<" "<<y;
    Interface::mouseMoveTo(x, y);
}

void CmdThread::cmdMouseLeftDown()
{
    int x = cmd_buf[1];
    x = x << 8;
    x += cmd_buf[2];
    int y = cmd_buf[3];
    y = y << 8;
    y += cmd_buf[4];
    Interface::mouseLeftDown(x, y);
}
void CmdThread::cmdMouseLeftUp()
{
    int x = cmd_buf[1];
    x = x << 8;
    x += cmd_buf[2];
    int y = cmd_buf[3];
    y = y << 8;
    y += cmd_buf[4];
    Interface::mouseLeftUp(x, y);
}

void CmdThread::cmdMouseRightDown()
{
    int x = cmd_buf[1];
    x = x << 8;
    x += cmd_buf[2];
    int y = cmd_buf[3];
    y = y << 8;
    y += cmd_buf[4];
    Interface::mouseRightDown(x, y);
}

void CmdThread::cmdMouseRightUp()
{
    int x = cmd_buf[1];
    x = x << 8;
    x += cmd_buf[2];
    int y = cmd_buf[3];
    y = y << 8;
    y += cmd_buf[4];
    Interface::mouseRightUp(x, y);
}

void CmdThread::cmdMouseDoubleClick()
{
    int x = cmd_buf[1];
    x = x << 8;
    x += cmd_buf[2];
    int y = cmd_buf[3];
    y = y << 8;
    y += cmd_buf[4];
    Interface::mouseLeftDown(x, y);
    Interface::mouseLeftUp(x, y);
    Interface::mouseLeftDown(x, y);
    Interface::mouseLeftUp(x, y);
}

void CmdThread::cmdMouseWheel()
{
    int delta = cmd_buf[1];
    delta = delta << 8;
    delta += cmd_buf[2];
    int x = cmd_buf[3];
    x = x << 8;
    x += cmd_buf[4];
    int y = cmd_buf[5];
    y = y << 8;
    y += cmd_buf[6];
    Interface::mouseWheel(delta, x, y);
}

void CmdThread::cmdScreenSize()
{
    uchar uc[8];
    uc[0] = CMD_GET_SCREEN_SIZE_RES;
    uc[1] = screen_width / 0x100;
    uc[2] = screen_width % 0x100;
    uc[3] = screen_height / 0x100;
    uc[4] = screen_height % 0x100;
    writeAndBlock(cmdSocket, uc, 8);
}
