#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T14:17:15
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    values.cpp \
    mapthread.cpp \
    algo.cpp \
    cmdthread.cpp \
    controlpanel.cpp

HEADERS  += mainwindow.h \
    values.h \
    mapthread.h \
    algo.h \
    consts.h \
    cmdthread.h \
    controlpanel.h \
    version.h

INCLUDEPATH += ../exinclude/ffmpeg

LIBS += ../exlib/avcodec.lib
LIBS += ../exlib/avformat.lib
LIBS += ../exlib/avutil.lib
LIBS += ../exlib/swscale.lib
