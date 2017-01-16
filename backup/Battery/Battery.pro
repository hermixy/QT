QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    battery.cpp \
    ../base/base.cpp

HEADERS += \
    battery.h \
    ../base/libbase.h \
    ../base/base.h

RESOURCES += \
    battery.qrc
