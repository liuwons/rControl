

#include <QtGui/QApplication>
#include <QDesktopWidget>

#include "version.h"
#include "mainwindow.h"
#include "interface.h"
#include "controler.h"

int main(int argc, char *argv[])
{
#ifdef DEBUG
    FILE* f = fopen("m.txt", "w");
    fprintf(f, "debug1\n");
    fflush(f);
#endif

    QApplication a(argc, argv);
#ifdef DEBUG
    fprintf(f, "debug1.2\n");
    fflush(f);
#endif

    QDesktopWidget* deskWidget = a.desktop();
    QRect screenRect  = deskWidget->screenGeometry();
    screen_width = screenRect.width();
    screen_height = screenRect.height();

#ifdef DEBUG
    fprintf(f, "debug2\n");
    fflush(f);
#endif

    new Controler;

#ifdef DEBUG
    fprintf(f, "debug3\n");
    fflush(f);
#endif

    //MainWindow w;
    //w.show();
    
    return a.exec();
}
