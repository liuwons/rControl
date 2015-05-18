#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T14:17:15
#
#-------------------------------------------------

QT       += core gui
QT       += network

CONFIG   += console

TARGET = xClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controlpanel.cpp \
    decode.cpp \
    Buffer.cpp \
    ScreenClient.cpp \
    struct.cpp \
    cJSON.cpp

HEADERS  += mainwindow.h \
    controlpanel.h \
    decode.h \
    Buffer.h \
    ScreenClient.h \
    struct.h \
    cJSON.h

INCLUDEPATH += ../../exinclude/ffmpeg
INCLUDEPATH += D:/bin/boost

LIBS += /OPT:NOREF

LIBS += ../../exlib/avcodec.lib
LIBS += ../../exlib/avformat.lib
LIBS += ../../exlib/avutil.lib
LIBS += ../../exlib/swscale.lib

LIBS += ../../exlib/libboost_thread-vc100-mt-1_58.lib
LIBS += ../../exlib/libboost_system-vc100-mt-1_58.lib
LIBS += ../../exlib/libboost_date_time-vc100-mt-1_58.lib
LIBS += ../../exlib/libboost_chrono-vc100-mt-1_58.lib
LIBS += ../../exlib/libboost_regex-vc100-mt-1_58.lib
