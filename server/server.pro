#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T13:31:08
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    interface.cpp \
    controler.cpp \
    cmdthread.cpp \
    mapthread.cpp \
    algo.cpp \
    httpsender.cpp

HEADERS  += mainwindow.h \
    interface.h \
    controler.h \
    cmdthread.h \
    mapthread.h \
    consts.h \
    algo.h \
    httpsender.h \
    version.h

INCLUDEPATH += ../exinclude/ffmpeg

LIBS += ../exlib/avcodec.lib
LIBS += ../exlib/avformat.lib
LIBS += ../exlib/avutil.lib
LIBS += ../exlib/swscale.lib
LIBS += User32.lib



