#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QRect>
#include <QTcpSocket>
#include <QHostAddress>
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "version.h"
#include "mapthread.h"
#include "cmdthread.h"
#include "controlpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ControlPanel* controlPanel;
    QLineEdit* edAddr;
    QLineEdit* edWidth;
    QLineEdit* edHeight;
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void keyPressed(QKeyEvent *);
    void keyReleased(QKeyEvent *);

public slots:
    void tryconnect();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

};

#endif // MAINWINDOW_H
