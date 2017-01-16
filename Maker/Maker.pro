QT      += core gui
QT      += script
QT      += xml
QT      += sql
QT      += network
QT += axcontainer multimedia

# 嵌入式设备下运行(Qt4.8.5)
#DEFINES += DEF_RUN_IN_EMBEDDED
#增加调试信息
DEFINES  += LOG_TO_FILE MAKER
CONFIG   += c++11
CONFIG   += link_pkgconfig

OBJDIR = obj_temp
UI_DIR = $$OBJDIR/ui
RCC_DIR = $$OBJDIR/rcc
MOC_DIR = $$OBJDIR/moc
OBJECTS_DIR = $$OBJDIR/obj

DESTDIR = $$PWD

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    Maker.cpp \
    main.cpp \
    ../Scada/Dialog/Play/DialogPlay.cpp \
    ../Scada/Dialog/Style/DialogStyle.cpp \
    ../Scada/Dialog/Service/DialogService.cpp \
    ../Scada/Src/Xml/XmlReader.cpp \
    ../Scada/Src/Xml/XmlWriter.cpp \
    ../Scada/Dialog/AddLink/DialogAddLink.cpp \
    ../Scada/Src/Service/Service.cpp \
    ../Scada/Src/Action/Action.cpp \
    ../Scada/Src/Base/Base.cpp \
    ../Scada/Dialog/NewScene/DialogNewScene.cpp \
    ../Scada/Src/File/File.cpp \
    ../Scada/Dialog/DataBase/DialogDataBase.cpp \
    ../Scada/Src/Math/Math.cpp \
    ../Scada/Src/Scene/Scene.cpp \
    ../Scada/Src/View/View.cpp \
    ../Scada/Src/Graphics/Arc.cpp \
    ../Scada/Src/Graphics/Ellipse.cpp \
    ../Scada/Src/Graphics/Group.cpp \
    ../Scada/Src/Graphics/Line.cpp \
    ../Scada/Src/Graphics/Pixmap.cpp \
    ../Scada/Src/Graphics/Point.cpp \
    ../Scada/Src/Graphics/Polygon.cpp \
    ../Scada/Src/Graphics/Rect.cpp \
    ../Scada/Src/Graphics/Text.cpp \
    ../Scada/Src/Linkage/Linkage.cpp \
    ../Scada/Dialog/JsHelper/DialogJsHelper.cpp \
    ../Scada/Dialog/Linkage/DialogLinkage.cpp \
    ../Scada/Src/Graphics/ValueText.cpp \
    ../Scada/Src/Undo/Undo.cpp \
    ../Scada/Src/Undo/UndoAddDel.cpp \
    ../Scada/Src/Undo/UndoMoveDrag.cpp \
    ../Scada/Src/Widgets/Button.cpp \
    ../Scada/Src/Widgets/InputEdit.cpp \
    ../Scada/Src/Widgets/Table.cpp \
    ../Scada/Src/Graphics/Light.cpp \
    ../Scada/Src/TableView/TableView.cpp \
    ../Scada/Src/Graphics/Meter.cpp \
    ../Scada/Src/Graphics/ProcessBar.cpp \
    ../Scada/Src/Js/Js.cpp \
    ../Scada/Src/Graphics/Graphics.cpp \
    ../Scada/Src/Widgets/Widgets.cpp \
    ../Scada/Src/Item/Item.cpp \
    ../Scada/Src/Item/ItemEx.cpp \
    ../Scada/Dialog/Service/DialogServiceEdit.cpp \
    ../Scada/Src/Graphics/Curve.cpp \
    ../Scada/Src/DataBase/DataBaseService.cpp \
    ../Scada/Src/DataBase/MySQL/MySQL.cpp \
    ../Scada/Src/DataBase/Access/Access.cpp \
    ../Scada/Src/DataBase/DataBase.cpp \
    ../Scada/Src/DataBase/DataBaseMsg.cpp \
    ../Scada/Src/Item/ItemFactory.cpp \
    ../Scada/Dialog/Style/dialogaddgroup.cpp \
    ../Scada/Dialog/Style/scenselistdialog.cpp \
    ../Scada/Dialog/Style/tabwidgetreport.cpp \
    ../Scada/Dialog/Timer/timeritemshow.cpp \
    ../Scada/Dialog/Timer/timeritemedit.cpp \
    ../Scada/Dialog/Style/modorder.cpp

HEADERS += \
    Maker.h \
    ../Scada/Dialog/Play/DialogPlay.h \
    ../Scada/Dialog/Style/DialogStyle.h \
    ../Scada/Dialog/Service/DialogService.h \
    ../Scada/Src/Xml/XmlReader.h \
    ../Scada/Src/Xml/XmlWriter.h \
    ../Scada/Dialog/AddLink/DialogAddLink.h \
    ../Scada/Src/Service/Service.h \
    ../Scada/Src/Action/Action.h \
    ../Scada/Src/Base/Base.h \
    ../Scada/Src/Base/LibBase.h \
    ../Scada/Dialog/NewScene/DialogNewScene.h \
    ../Scada/Src/File/File.h \
    ../Scada/Dialog/DataBase/DialogDataBase.h \
    ../Scada/Src/Math/Math.h \
    ../Scada/Src/Scene/Scene.h \
    ../Scada/Src/View/View.h \
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
    ../Scada/Src/Widgets/Widgets.h \
    ../Scada/Src/Widgets/WidgetsPub.h \
    ../Scada/Src/Linkage/Linkage.h \
    ../Scada/Src/Js/Js.h \
    ../Scada/Dialog/JsHelper/DialogJsHelper.h \
    ../Scada/Dialog/Linkage/DialogLinkage.h \
    ../Scada/Src/DataBase/DataBaseMsg.h \
    ../Scada/Src/Graphics/ValueText.h \
    ../Scada/Src/Undo/Undo.h \
    ../Scada/Src/Undo/UndoAddDel.h \
    ../Scada/Src/Undo/UndoMoveDrag.h \
    ../Scada/Src/Widgets/Button.h \
    ../Scada/Src/Widgets/InputEdit.h \
    ../Scada/Src/Widgets/Table.h \
    ../Scada/Src/Graphics/Light.h \
    ../Scada/Src/TableView/TableView.h \
    ../Scada/Src/Graphics/Meter.h \
    ../Scada/Src/Graphics/ProcessBar.h \
    ../Scada/Src/Item/Item.h \
    ../Scada/Src/Item/ItemEx.h \
    ../Scada/Dialog/Service/DialogServiceEdit.h \
    ../Scada/Src/Graphics/Curve.h \
    ../Scada/Src/DataBase/DataBaseService.h \
    ../Scada/Src/DataBase/DataBasePub.h \
    ../Scada/Src/DataBase/MySQL/MySQL.h \
    ../Scada/Src/DataBase/Access/Access.h \
    ../Scada/Src/DataBase/DataBase.h \
    ../Scada/Src/Item/ItemFactory.h \
    ../Scada/Dialog/Style/dialogaddgroup.h \
    ../Scada/Dialog/Style/scenselistdialog.h \
    ../Scada/Dialog/Style/tabwidgetreport.h \
    ../Scada/Dialog/Timer/timeritemshow.h \
    ../Scada/Dialog/Timer/timeritemedit.h \
    ../Scada/Dialog/Style/modorder.h

RESOURCES += \
    Maker.qrc

RC_FILE = ico.rc

#注释 取消 快捷键：Ctrl + /

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scada/Src/Item/ -lItem
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scada/Src/Item/ -lItemd
#else:unix: LIBS += -L$$PWD/../Scada/Src/Item/ -lItem

#INCLUDEPATH += $$PWD/../Scada/Src/Item
#DEPENDPATH += $$PWD/../Scada/Src/Item

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scada/Src/Js/ -lJs
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scada/Src/Js/ -lJsd
#else:unix: LIBS += -L$$PWD/../Scada/Src/Js/ -lJs

#INCLUDEPATH += $$PWD/../Scada/Src/Js
#DEPENDPATH += $$PWD/../Scada/Src/Js

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scada/Src/Graphics/ -lGraphics
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scada/Src/Graphics/ -lGraphicsd
#else:unix: LIBS += -L$$PWD/../Scada/Src/Graphics/ -lGraphics

#INCLUDEPATH += $$PWD/../Scada/Src/Graphics
#DEPENDPATH += $$PWD/../Scada/Src/Graphics

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Scada/Src/Widgets/ -lWidgets
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Scada/Src/Widgets/ -lWidgetsd
#else:unix: LIBS += -L$$PWD/../Scada/Src/Widgets/ -lWidgets

#INCLUDEPATH += $$PWD/../Scada/Src/Widgets
#DEPENDPATH += $$PWD/../Scada/Src/Widgets

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
#LIBS+= D:/Qt/Qt5.4.0/mysql/lib/libmysql.lib
#INCLUDEPATH += D:/Qt/Qt5.4.0/mysql/include/

OTHER_FILES +=

FORMS += \
    ../Scada/Dialog/Style/reportform.ui
