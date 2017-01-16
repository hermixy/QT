#include "Maker.h"
#include "../Scada/Src/DataBase/MySQL/MySQL.h"
#include <QApplication>
#include <QTextCodec>
#include <ActiveQt/QAxWidget>
#include <QVBoxLayout>
#ifdef LOG_TO_FILE
static QAtomicPointer<FILE> logFileFile = nullptr;               //原子的访问file指针
static QList<QByteArray>* logBuffer = new QList<QByteArray>();   //存储消息直到记录文件打开    字节数组
QMutex* logBufferMutex = new QMutex();                           //logbuffer访问锁
#endif

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

#ifdef LOG_TO_FILE
    FILE * logFilePtr = logFileFile.load(); // atomically load the file pointer
    if (!logFilePtr)
    {
        logBufferMutex->lock();
        if(logBuffer)
        {
            logBuffer->append(LogMsgBytes);
        }
        logBufferMutex->unlock();
    }
    else
    {
        logBufferMutex->lock();
        if(logBuffer)
        {
            // empty logBuffer to file
            foreach(QByteArray msg, *logBuffer)
            {
                fwrite(msg.constData(), 1, msg.size(), logFilePtr);
            }

            delete logBuffer;   // no longer needed
            logBuffer = nullptr;
        }
        logBufferMutex->unlock();

        fwrite(LogMsgBytes.constData(), 1, LogMsgBytes.size(), logFilePtr);
        fflush(logFilePtr);
    }
#endif
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread::currentThread()->setObjectName("GUI thread");            //设置GUI线程的名字
    qInstallMessageHandler(logMessageHandler);

    #ifdef DEF_RUN_IN_EMBEDDED
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    QFont font;
    font.setPointSize(16); //
    font.setFamily(("wenquanyi"));
    //font.setFamily(("unifont"));
    font.setBold(false);
    a.setFont(font);
    #endif

    Maker mMaker;

    mMaker.showMaximized();
    
    a.exec();
    return 0;
}
