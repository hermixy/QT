QT      += core gui
QT      += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    v6service.h \
    ../tcp/tcpsocket.h \
    ../tcp/tcpserver.h \
    ../ringbuf/ringbuf.h \
    ../base/libbase.h \
    ../base/base.h \
    ../threadv6/thread.h \
    ../table/table.h

SOURCES += \
    v6service.cpp \
    main.cpp \
    ../tcp/tcpsocket.cpp \
    ../tcp/tcpserver.cpp \
    ../ringbuf/ringbuf.cpp \
    ../base/base.cpp \
    ../threadv6/thread.cpp \
    ../table/table.cpp

RC_FILE = ico.rc
