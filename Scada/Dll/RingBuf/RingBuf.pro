#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T13:40:46
#
#-------------------------------------------------

QT       -= gui

TARGET = RingBuf
TEMPLATE = lib

DEFINES += RINGBUF_LIBRARY

SOURCES += RingBuf.cpp

HEADERS += RingBuf.h\
        ringbuf_global.h \
    ../../Src/Base/LibBase.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
