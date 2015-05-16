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

#include "controlpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLineEdit* edAddr;
    QLineEdit* edHeight;
    QLineEdit* edWidth;
    ControlPanel* panel;
    
public:
    MainWindow(int x, int y, int w, int h, QWidget *parent = 0);
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
