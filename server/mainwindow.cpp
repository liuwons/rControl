#include "mainwindow.h"
#include "interface.h"
#include "controler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Interface::mouse(100, 100);
    new Controler;
}

MainWindow::~MainWindow()
{

}
