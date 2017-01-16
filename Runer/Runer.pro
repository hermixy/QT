QT     += serialport
QT      += core gui
QT      += script
QT      += xml
QT      += network
QT      += sql
QT      += axcontainer printsupport multimedia
QT += webkitwidgets
OBJDIR = obj_temp
UI_DIR = $$OBJDIR/ui
RCC_DIR = $$OBJDIR/rcc
MOC_DIR = $$OBJDIR/moc
OBJECTS_DIR = $$OBJDIR/obj

DESTDIR = $$PWD

# 杩愯鍣ㄦā寮?
DEFINES += DEF_RUN_AS_RUNER RUNNER
CONFIG   += c++11 qwt

# CS瀹㈡埛绔繍琛屽�?
DEFINES += DEF_RUN_AS_CLIENT

# JS鑴氭湰淇濆瓨鐨勪俊鎭細鏂囦欢鍚嶃€佹枃浠跺唴瀹广€佽В鏋愬悗鏁版嵁
#DEFINES += DEF_JS_SAVE_NAME
DEFINES += DEF_JS_SAVE_FILE
#DEFINES += DEF_JS_SAVE_DATA

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
HEADERS += \
    Runer.h \
    ../Scada/Src/Action/Action.h \
    ../Scada/Src/Base/Base.h \
    ../Scada/Src/Base/LibBase.h \
    ../Scada/Src/Connection/Connection.h \
    ../Scada/Src/DataBase/DataBase.h \
    ../Scada/Src/DataBase/DataBasePub.h \
    ../Scada/Src/File/File.h \
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
    ../Scada/Src/Js/Js.h \
    ../Scada/Src/Protocol/Protocol.h \
    ../Scada/Src/RingBuf/RingBuf.h \
    ../Scada/Src/Linkage/Linkage.h \
    ../Scada/Src/Scene/Scene.h \
    ../Scada/Src/Service/Service.h \
    ../Scada/Src/TableView/TableView.h \
    ../Scada/Src/TableView/TableService.h \
    ../Scada/Src/View/View.h \
    ../Scada/Src/Xml/XmlReader.h \
    ../Scada/Dialog/Alarm/DialogAlarm.h \
    ../Scada/Dialog/Data/DialogData.h \
    ../Scada/Dialog/Login/DialogLogin.h \
    ../Scada/Dialog/Hint/DialogHint.h \
    ../Scada/Src/DataBase/DataBaseMsg.h \
    ../Scada/Src/Connection/Com/Com.h \
    ../Scada/Src/Connection/TcpSocket/TcpSocket.h \
    ../Scada/Src/Protocol/Iec104/Iec104.h \
    ../Scada/Src/Protocol/Iec104/Iec104Service.h \
    ../Scada/Src/Protocol/Modbus/Modbus.h \
    ../Scada/Src/Protocol/Syclient/Syclient.h \
    ../Scada/Src/Protocol/Sygzzs/Sygzzs.h \
    ../Scada/Src/Protocol/Syznkg/Syznkg.h \
    ../Scada/Src/DataBase/Access/Access.h \
    ../Scada/Src/DataBase/MySQL/MySQL.h \
    ../Scada/Src/Thread/ThreadConnection.h \
    ../Scada/Src/Thread/ThreadServer.h \
    ../Scada/Src/Math/Math.h \
    ../Scada/Src/Graphics/ValueText.h \
    ../Scada/Src/Thread/Thread.h \
    ../Scada/Src/Widgets/Button.h \
    ../Scada/Src/Widgets/InputEdit.h \
    ../Scada/Src/Widgets/Table.h \
    ../Scada/Src/Widgets/Widgets.h \
    ../Scada/Src/Widgets/WidgetsPub.h \
    ../Scada/Src/Graphics/Light.h \
    ../Scada/Src/Graphics/Meter.h \
    ../Scada/Src/Graphics/ProcessBar.h \
    ../Scada/Src/SendTo/SendTo.h \
    ../Scada/Src/Thread/ThreadIO.h \
    ../Scada/Src/Thread/ThreadProcess.h \
    ../Scada/Src/Thread/ThreadList.h \
    ../Scada/Src/Server/TcpServer/TcpServer.h \
    ../Scada/Src/Item/ItemEx.h \
    ../Scada/Dialog/InputMessageBox/DialogInputMessageBox.h \
    ../Scada/Src/Protocol/Syreal/Syreal.h \
    ../Scada/Src/Graphics/Curve.h \
    ../Scada/Src/Thread/ThreadRecv.h \
    ../Scada/Src/Word/Word.h \
    ../Scada/Dialog/Report/DialogReport.h \
    ../Scada/Src/DataBase/DataBaseService.h \
    ../Scada/Src/DataBase/DataBasePub.h \
    ../Scada/Src/Thread/ThreadClient.h \
    ../Scada/Src/Item/ItemFactory.h \
    ../Scada/Src/Thread/senderorder.h \
    ../Scada/Src/Thread/analysisthread.h \
    ../Scada/Dialog/Timer/ttimer.h \
    ../Scada/Dialog/Timer/dialogtimer.h \
    customprotocol.h \
    ../Scada/Src/Thread/reciverhandle.h \
    ../Scada/Dialog/Timer/showhistory.h \
    ../Scada/Dialog/Style/tabwidgetreport.h \
    ../Scada/Src/Qt-ExcelEngine/excelengine.h \
    ../Scada/Dialog/Timer/timerframeitem1.h \
    ../Scada/Dialog/Timer/onlinestatus.h \
    ../Scada/Src/Thread/timersendthread.h \
    ../Scada/Src/Qchart/ChartLine.h \
    ../Scada/Src/Qchart/chartpie.h \
    ../Scada/Src/Qchart/mainwidget.h \
    ../Scada/Src/Qchart/qwidgetline.h \
    ../Scada/Src/Qchart/qwidgetpie.h \
    ../Scada/Dialog/Alarm/alarmdialog.h

SOURCES += \
    Runer.cpp \
    main.cpp \
    ../Scada/Src/Action/Action.cpp \
    ../Scada/Src/Base/Base.cpp \
    ../Scada/Src/Connection/Connection.cpp \
    ../Scada/Src/DataBase/DataBase.cpp \
    ../Scada/Src/File/File.cpp \
    ../Scada/Src/Graphics/Arc.cpp \
    ../Scada/Src/Graphics/Ellipse.cpp \
    ../Scada/Src/Graphics/Group.cpp \
    ../Scada/Src/Graphics/Line.cpp \
    ../Scada/Src/Graphics/Pixmap.cpp \
    ../Scada/Src/Graphics/Point.cpp \
    ../Scada/Src/Graphics/Polygon.cpp \
    ../Scada/Src/Graphics/Rect.cpp \
    ../Scada/Src/Graphics/Text.cpp \
    ../Scada/Src/Protocol/Protocol.cpp \
    ../Scada/Src/Linkage/Linkage.cpp \
    ../Scada/Src/Scene/Scene.cpp \
    ../Scada/Src/Service/Service.cpp \
    ../Scada/Src/TableView/TableView.cpp \
    ../Scada/Src/TableView/TableService.cpp \
    ../Scada/Src/View/View.cpp \
    ../Scada/Src/Xml/XmlReader.cpp \
    ../Scada/Dialog/Alarm/DialogAlarm.cpp \
    ../Scada/Dialog/Data/DialogData.cpp \
    ../Scada/Dialog/Login/DialogLogin.cpp \
    ../Scada/Dialog/Hint/DialogHint.cpp \
    ../Scada/Src/Connection/Com/Com.cpp \
    ../Scada/Src/Connection/TcpSocket/TcpSocket.cpp \
    ../Scada/Src/Protocol/Iec104/Iec104.cpp \
    ../Scada/Src/Protocol/Iec104/Iec104Service.cpp \
    ../Scada/Src/Protocol/Modbus/Modbus.cpp \
    ../Scada/Src/Protocol/Syclient/Syclient.cpp \
    ../Scada/Src/Protocol/Sygzzs/Sygzzs.cpp \
    ../Scada/Src/Protocol/Syznkg/Syznkg.cpp \
    ../Scada/Src/DataBase/Access/Access.cpp \
    ../Scada/Src/DataBase/MySQL/MySQL.cpp \
    ../Scada/Src/Thread/ThreadConnection.cpp \
    ../Scada/Src/Math/Math.cpp \
    ../Scada/Src/Thread/ThreadServer.cpp \
    ../Scada/Src/Item/Item.cpp \
    ../Scada/Src/Graphics/Graphics.cpp \
    ../Scada/Src/Js/Js.cpp \
    ../Scada/Src/RingBuf/RingBuf.cpp \
    ../Scada/Src/Graphics/ValueText.cpp \
    ../Scada/Src/Thread/Thread.cpp \
    ../Scada/Src/Widgets/Button.cpp \
    ../Scada/Src/Widgets/InputEdit.cpp \
    ../Scada/Src/Widgets/Table.cpp \
    ../Scada/Src/Widgets/Widgets.cpp \
    ../Scada/Src/Graphics/Light.cpp \
    ../Scada/Src/Graphics/Meter.cpp \
    ../Scada/Src/Graphics/ProcessBar.cpp \
    ../Scada/Src/SendTo/SendTo.cpp \
    ../Scada/Src/Thread/ThreadIO.cpp \
    ../Scada/Src/Thread/ThreadProcess.cpp \
    ../Scada/Src/Thread/ThreadList.cpp \
    ../Scada/Src/Server/TcpServer/TcpServer.cpp \
    ../Scada/Src/Item/ItemEx.cpp \
    ../Scada/Dialog/InputMessageBox/DialogInputMessageBox.cpp \
    ../Scada/Src/Protocol/Syreal/Syreal.cpp \
    ../Scada/Src/Graphics/Curve.cpp \
    ../Scada/Src/Thread/ThreadRecv.cpp \
    ../Scada/Src/Word/Word.cpp \
    ../Scada/Dialog/Report/DialogReport.cpp \
    ../Scada/Src/DataBase/DataBaseService.cpp \
    ../Scada/Src/DataBase/DataBaseMsg.cpp \
    ../Scada/Src/Thread/ThreadClient.cpp \
    ../Scada/Src/Item/ItemFactory.cpp \
    ../Scada/Src/Thread/senderorder.cpp \
    ../Scada/Src/Thread/analysisthread.cpp \
    ../Scada/Dialog/Timer/ttimer.cpp \
    ../Scada/Dialog/Timer/dialogtimer.cpp \
    customprotocol.cpp \
    ../Scada/Src/Thread/reciverhandle.cpp \
    ../Scada/Dialog/Timer/showhistory.cpp \
    ../Scada/Dialog/Style/tabwidgetreport.cpp \
    ../Scada/Src/Qt-ExcelEngine/excelengine.cpp \
    ../Scada/Dialog/Timer/timerframeitem1.cpp \
    ../Scada/Dialog/Timer/onlinestatus.cpp \
    ../Scada/Src/Thread/timersendthread.cpp \
    ../Scada/Src/Qchart/ChartLine.cpp \
    ../Scada/Src/Qchart/chartpie.cpp \
    ../Scada/Src/Qchart/mainwidget.cpp \
    ../Scada/Src/Qchart/qwidgetline.cpp \
    ../Scada/Src/Qchart/qwidgetpie.cpp \
    ../Scada/Dialog/Alarm/alarmdialog.cpp

RC_FILE = ico.rc

FORMS += \
    ../Scada/Dialog/Style/reportform.ui \
    ../Scada/Src/Echart11111/Qchart/widget.ui \
    ../Scada/Dialog/Timer/timerframeitem1.ui \
    ../Scada/Dialog/Timer/dialogtimer.ui \
    ../Scada/Dialog/Timer/onlinestatus.ui \
    ../Scada/Src/Qchart/mainwidget.ui
