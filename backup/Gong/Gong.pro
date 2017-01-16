QT      += serialport
QT      += core gui
QT      += script
QT      += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \    
    gong.h \
    ../Scada/Src/Action/Action.h \
    ../Scada/Src/Base/Base.h \
    ../Scada/Src/Base/LibBase.h \
    ../Scada/Src/Clock/Clock.h \
    ../Scada/Src/Connection/Connection.h \
    ../Scada/Src/Connection/Com/Com.h \
    ../Scada/Src/Graphics/Arc.h \
    ../Scada/Src/Graphics/Ellipse.h \
    ../Scada/Src/Graphics/Graphics.h \
    ../Scada/Src/Graphics/GraphicsPub.h \
    ../Scada/Src/Graphics/Group.h \
    ../Scada/Src/Graphics/Line.h \
    ../Scada/Src/Graphics/Pixmap.h \
    ../Scada/Src/Graphics/Point.h \
    ../Scada/Src/Graphics/Polygon.h \
    ../Scada/Src/Graphics/Rect.h \
    ../Scada/Src/Graphics/Text.h \
    ../Scada/Src/Item/Item.h \
    ../Scada/Src/Math/Math.h \
    ../Scada/Src/Protocol/Protocol.h \
    ../Scada/Src/Protocol/Modbus/Modbus.h \
    ../Scada/Src/Ringbuf/RingBuf.h \
    ../Scada/Src/Scale/Scale.h \
    ../Scada/Src/Scene/Scene.h \
    ../Scada/Src/Thread/ThreadConnection.h \
    ../Scada/Src/View/View.h \
    ../Scada/Dialog/ComConfig/DialogComConfig.h \
    GongConfig/DialogGongConfig.h \
    ../Scada/Src/Js/Js.h \
    ../Scada/Src/File/File.h \
    ../Scada/Src/Service/Service.h \
    GongConfig/GongConfig.h \
    ../Scada/Src/LightView/LightView.h \
    ../Scada/Src/TableView/TableService.h \
    ../Scada/Src/TableView/TableView.h \
    ../Scada/Src/Thread/Thread.h

SOURCES += \
    main.cpp \
    gong.cpp \
    ../Scada/Src/Action/Action.cpp \
    ../Scada/Src/Base/Base.cpp \
    ../Scada/Src/Clock/Clock.cpp \
    ../Scada/Src/Connection/Connection.cpp \
    ../Scada/Src/Connection/Com/Com.cpp \
    ../Scada/Src/Graphics/Arc.cpp \
    ../Scada/Src/Graphics/Ellipse.cpp \
    ../Scada/Src/Graphics/Group.cpp \
    ../Scada/Src/Graphics/Line.cpp \
    ../Scada/Src/Graphics/Pixmap.cpp \
    ../Scada/Src/Graphics/Point.cpp \
    ../Scada/Src/Graphics/Polygon.cpp \
    ../Scada/Src/Graphics/Rect.cpp \
    ../Scada/Src/Graphics/Text.cpp \
    ../Scada/Src/Math/Math.cpp \
    ../Scada/Src/Protocol/Protocol.cpp \
    ../Scada/Src/Protocol/Modbus/Modbus.cpp \
    ../Scada/Src/Scale/Scale.cpp \
    ../Scada/Src/Scene/Scene.cpp \
    ../Scada/Src/Thread/ThreadConnection.cpp \
    ../Scada/Src/View/View.cpp \
    ../Scada/Dialog/ComConfig/DialogComConfig.cpp \
    GongConfig/DialogGongConfig.cpp \
    ../Scada/Src/File/File.cpp \
    ../Scada/Src/Service/Service.cpp \
    GongConfig/GongConfig.cpp \
    ../Scada/Src/Ringbuf/RingBuf.cpp \
    ../Scada/Src/Graphics/Graphics.cpp \
    ../Scada/Src/Item/Item.cpp \
    ../Scada/Src/Js/Js.cpp \
    ../Scada/Src/LightView/LightView.cpp \
    ../Scada/Src/TableView/TableService.cpp \
    ../Scada/Src/TableView/TableView.cpp \
    ../Scada/Src/Thread/Thread.cpp

RC_FILE = ico.rc

RESOURCES += \
    pic.qrc
