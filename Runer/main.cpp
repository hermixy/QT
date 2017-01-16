#include "Runer.h"
#include "customprotocol.h"
#include <QTextCodec>
#include <QApplication>
#include <QSettings>
#include "../Scada/Dialog/Login/DialogLogin.h"
#include "../Scada/Src/Qt-ExcelEngine/excelengine.h"
#include "../Scada/Src/Thread/timersendthread.h"
#include "Runer.h"
#include <QFile>
#include <QInputDialog>
#include <QSound>
#include <QMessageBox>
void logMessageHandler(QtMsgType type, const QMessageLogContext& ctxt, const QString& msg)
{

        // Silence qWarning spam due to bug in QTextBrowser (trying to open a file for base64 images)
        if (ctxt.function == QString("virtual bool QFSFileEngine::open(QIODevice::OpenMode)")
                && msg == QString("QFSFileEngine::open: No file name specified"))
            return;

        QString LogMsg = QString("[%1] %2:%3 : ")
                    .arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(ctxt.file).arg(ctxt.line);
        switch (type)
        {
            case QtDebugMsg:
                LogMsg += "Debug";
                break;
            case QtWarningMsg:
                LogMsg += "Warning";
                break;
            case QtCriticalMsg:
                LogMsg += "Critical";
                break;
            case QtFatalMsg:
                LogMsg += "Fatal";
                break;
            default:
                break;
        }

        LogMsg += ": " + msg + "\n";
        QByteArray LogMsgBytes = LogMsg.toLocal8Bit(); //debug 编码格式
        fwrite(LogMsgBytes.constData(), 1, LogMsgBytes.size(), stderr);
        fflush(stderr);   //刷新输出
}
int main(int argc, char *argv[])
{
    //QApplication::addLibraryPath("./plugins");

    QApplication a(argc, argv);
    QThread::currentThread()->setObjectName("GUI thread");            //设置GUI线程的名字
    qInstallMessageHandler(logMessageHandler);
    a.setApplicationName(QObject::tr("建筑节能系统V3.0"));
    a.setStyleSheet("QPushButton{\
                    border-style: outset;\
                    border-width: 1px;\
                    border-bottom-width: 4px; \
                    border-left-width: 2px;\
                    border-right-width: 2px;\
                    min-width: 5em;\
                    border-color: qlineargradient(spread:pad, x1:0, y1:0.511364, x2:1, y2:0.494, stop:0 rgba(199, 213, 120, 255), stop:1 rgba(0, 0, 0, 255));\
}QPushButton:pressed{border-width: 1px;}");


//qDebug()<<CustomProtocol::sendOrderOneThree(0x0000, 0x10, 0x0001, 0x02);

    qRegisterMetaType<qintptr>("qintptr");
    double dayTIme=24*60.0*60;

    QSettings setting("inis", QSettings::IniFormat);
    setting.setIniCodec("UTF-8");

    QList<MyLoginData> logins;
    MyLoginData data={true, "admin", "1235"};
    logins.append(data);
    int size=setting.value("num", 0).toInt();
    if(size==0) {
    setting.setValue("num", 1);
    setting.beginWriteArray("logins", logins.size());
    for (int i = 0; i < logins.size(); ++i) {
        QByteArray bytes;
        bytes.append(logins.at(i).password);
        bytes=CustomProtocol::getMd5Hash(bytes);
        setting.setArrayIndex(i);
        setting.setValue("permission", logins.at(i).permission);
        setting.setValue("userName", logins.at(i).userName);
        setting.setValue("password", bytes);
    }
    setting.endArray();
    }

    /***********************************/
    QSettings setting1("inis", QSettings::IniFormat);
    setting1.setIniCodec("UTF-8");
    qlonglong time=setting1.value("time", 0).toLongLong();
    if(time/dayTIme>=365)
    {
        QString regist=QInputDialog::getText(0, QObject::tr("软件授权"), QObject::tr("请输入注册码"));
        if(!regist.isEmpty())
        {
            qDebug()<<CustomProtocol::getMd5Hash(CustomProtocol::get_localmachine_mac());
            if(regist==CustomProtocol::getMd5Hash(CustomProtocol::get_localmachine_mac()))
            {
                setting1.setValue("time", 0);
                setting1.setValue("registNum", setting1.value("registNum", 0).toInt()+1);
            }
            else
            {
                return 0;
            }
        }
        else
        {
        return 0;
        }
    }

   // qDebug()<<QDir::currentPath();
    DialogLogin *Login = new DialogLogin(true, 0);
    Login->exec();

    if(!Login->ok)
    {
        return 0;
    }

    Runer mRuner(0, Login->admin);
    if(!Runer::runner)
        return -1;
    mRuner.showMaximized();

//    AnalysisThread thread;
//    thread.start();
//    ReciverHandle reciverThread;
//    reciverThread.start();
//    TimerSendThread timerSend;
//    timerSend.start();

    a.exec();
//    thread.quit();
//    thread.wait();

//    reciverThread.stop();
//    reciverThread.exit(0);
//    reciverThread.wait();

//    timerSend.stop();
//    timerSend.exit(0);
//    timerSend.wait();

    return 0;

}

