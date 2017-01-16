#include "v6service.h"

//#include "../ringbuf/ringbuf.h"
//#include "../base/base.h"

#include <QDebug>
#include <QLayout>
#include <QDateTime>
#include <QAction>
#include <QStandardItemModel>

V6Service::V6Service(QWidget *parent) : QMainWindow(parent)
{
    // 智能型水位计服务系统 - IPV6型
    // 中国水利科学研究院
    QFont ft;
    ft.setPointSize(13); //设置字体大小
    ft.setBold(true);  //设置字体加粗，类似的还有斜体等等
    mLabelSoftName = TBase::fLabel(tr("智能型水位计服务系统 - IPV6型"), tr("mLabelSoftName"));
    mLabelSoftName->setFont(ft);

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::blue);//设置文本颜色，类似还可以设置背景颜色
    mLabelSoftName->setPalette(pal);

    mLabelCompany = TBase::fLabel(tr("中国水利科学研究院 "), tr("mLabelCompany"));
    mLabelIP = TBase::fLabel(tr("IP"), tr("mLabelIP"));
    mTextEditIP = TBase::fTextEdit(tr("mTextEditIP"), 300, 24);
    QHBoxLayout *mLayoutIp = new QHBoxLayout();
    mLayoutIp->addWidget(mLabelIP);
    mLayoutIp->addWidget(mTextEditIP);

    mLabelPort = TBase::fLabel(tr("Port"), tr("mLabelPort"));
    mSpinBoxPort = TBase::fSpinBox(tr("mSpinBoxPort"), 1, 65535, 75, 24);
    mSpinBoxPort->setValue(2404);
    QHBoxLayout *mLayoutPort = new QHBoxLayout();
    mLayoutPort->addWidget(mLabelPort);
    mLayoutPort->addWidget(mSpinBoxPort);

    mPushButtonStart = TBase::fButton(tr("启动"), tr("mPushButtonStart"), 75, 24);
    connect(mPushButtonStart, SIGNAL(clicked()), this, SLOT(fServerStart()));

    mPushButtonStop = TBase::fButton(tr("停止"), tr("mPushButtonStop"), 75, 24);
    connect(mPushButtonStop, SIGNAL(clicked()), this, SLOT(fServerStop()));

    QHBoxLayout *mLayoutTop = new QHBoxLayout();
    mLayoutTop->addLayout(mLayoutIp);
    mLayoutTop->addLayout(mLayoutPort);
    mLayoutTop->addWidget(mPushButtonStart);
    mLayoutTop->addWidget(mPushButtonStop);
    mLayoutTop->addStretch();
    mLayoutTop->addWidget(mLabelSoftName);
    mLayoutTop->addStretch();

    mTextEditMsg = TBase::fTextEdit(tr("mTextEditMsg"), 500, 500);
    //mTextEditMsg->setEnabled(false);

    //mListView = new QListView();
    //mListView->setObjectName(tr("mListView"));
    //mListView->setFixedSize(300, 500);
    QList<QString> ListString;
    ListString.push_back(tr("IP"));
    ListString.push_back(tr("Port"));
    ListString.push_back(tr("Name"));

    mTableSocket = new TTable(tr("mTabelSocket"), ListString);
    mTableSocket->setFixedSize(500, 500);

    QList<int> ListInt;
    ListInt.push_back(300);
    ListInt.push_back(50);
    ListInt.push_back(150);
    mTableSocket->fSetColFixLength(ListInt);

    mComboBoxSend = TBase::fComboBox(tr("mComboBoxSend"));
    mComboBoxSend->addItem(tr("读编号"));
    mComboBoxSend->addItem(tr("读数据"));
    connect(mComboBoxSend, SIGNAL(currentIndexChanged(int)), this, SLOT(fComboBoxSendChanged(int)));

    mTextEditSend = TBase::fTextEdit(tr("mTextEditSend"), 700, 24);
    mTextEditSend->setText(tr("$AAAAR"));
    mButtonSend = TBase::fButton(tr("发送"), tr("mButtonSend"), 50, 24);
    connect(mButtonSend, SIGNAL(clicked()), this, SLOT(fSendData()));

    QHBoxLayout *mLayoutEnd = new QHBoxLayout();
    mLayoutEnd->addWidget(mComboBoxSend);
    mLayoutEnd->addWidget(mTextEditSend);
    mLayoutEnd->addWidget(mButtonSend);
    mLayoutEnd->addStretch();
    mLayoutEnd->addWidget(mLabelCompany);

    QWidget *mWidget = new QWidget(this);
    mWidget->setObjectName(QStringLiteral("mWidget"));
    setCentralWidget(mWidget);

    QGridLayout *mLayoutMain = new QGridLayout(mWidget);
    mLayoutMain->addLayout(mLayoutTop, 0, 0, 1, 2);
    mLayoutMain->addWidget(mTableSocket, 1, 0);
    mLayoutMain->addWidget(mTextEditMsg, 1, 1);
    mLayoutMain->addLayout(mLayoutEnd, 2, 0, 1, 2);

    //setLayout(mLayoutMain);
    //setCentralWidget(mLayoutMain);

    setWindowTitle(mLabelSoftName->text());
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    mServer = new TTcpServer(this);
    connect(mServer, SIGNAL(mNewSocket(QString,QString,QString)), this, SLOT(fNewSocket(QString,QString,QString)));
    connect(mServer, SIGNAL(mDelSocket(QString,QString,QString)), this, SLOT(fDelSocket(QString,QString,QString)));

    // 创建服务线程
    mThreadv6 = new TThreadV6(mServer, this);
    connect(mThreadv6, SIGNAL(mDataCome(QString,QString,QString,QString)), this, SLOT(fRecvData(QString,QString,QString,QString)));

    mThreadv6->start();
}

void V6Service::fComboBoxSendChanged(int Index)
{
    switch(Index)
    {
    case 0:
        mTextEditSend->setText(tr("$AAAAR"));
        break;

    case 1:
        mTextEditSend->setText(tr("$9999D"));
        break;

    default:
        break;
    }
}

void V6Service::fSendData()
{
    qDebug() << mTableSocket->fCurrentIndex();
    qDebug() << mTableSocket->fCurrentData(0);
    qDebug() << mTableSocket->fCurrentData(1);

    mServer->mListLock.lock();
    foreach(TTcpSocket *Socket, mServer->mSocketList)
    {
        if(Socket->peerAddress().toString() == mTableSocket->fCurrentData(0)
                && QString::number(Socket->peerPort()) == mTableSocket->fCurrentData(1))
        {
            QByteArray ByteArray = mTextEditSend->toPlainText().toLatin1();
            Socket->write(ByteArray.data(), ByteArray.length());
            fAddRunMsg(tr("向 IP ") + Socket->peerAddress().toString() + tr(" Port ") + QString::number(Socket->peerPort()) + tr("(") + Socket->peerName() + tr(") 发送数据：") + mTextEditSend->toPlainText());
            break;
        }
    }
    mServer->mListLock.unlock();
}

void V6Service::fNewSocket(const QString Name, const QString IP, const QString Port)
{
    fAddRunMsg(tr("一个新连接：IP ") + IP + tr(" Port ") + Port + tr("(") + Name + tr(")"));

    QList<QString>ListString;
    ListString.push_back(IP);
    ListString.push_back(Port);
    ListString.push_back(Name);
    mTableSocket->fAddData(ListString);
}

void V6Service::fDelSocket(const QString Name, const QString IP, const QString Port)
{
    qDebug() << "V6Service::fDelSocket";
    fAddRunMsg(tr("连接断开：IP ") + IP + tr(" Port ") + Port + tr("(") + Name + tr(")"));

    QList<QString>ListString;
    ListString.push_back(IP);
    ListString.push_back(Port);
    ListString.push_back(Name);
    mTableSocket->fDelData(ListString);

    qDebug() << "V6Service::fDelSocket finash";
}

void V6Service::fRecvData(const QString Name, const QString IP, const QString Port, const QString pcRecv)
{
    fAddRunMsg(tr("从 IP ") + IP + tr(" Port ") + Port + tr("(") + Name + tr(") 接收数据：") + QString(pcRecv));
}

void V6Service::fAddRunMsg(const QString &mStr)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString TimeStr = time.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    mTextEditMsg->insertPlainText(tr("[") + TimeStr + tr("] ") + mStr + tr("\n"));
}

void V6Service::fServerStart()
{
    if(mServer)
    {
        //if(!mServer->listen(QHostAddress("2001:da8:2004:1000:202:116:160:22"), 2404))
        //if(!mServer->listen(QHostAddress::AnyIPv6, 2404))
        QHostAddress mHostAddress;

        if(mTextEditIP->toPlainText().length() <= 0)
        {
            mHostAddress = QHostAddress::Any;
        }
        else
        {
            mHostAddress = QHostAddress(mTextEditIP->toPlainText());
        }

        int mPort = mSpinBoxPort->value();

        if(mServer->listen(mHostAddress, mPort))
        {
            fAddRunMsg(tr("Server Started(") + tr("IP:") + mHostAddress.toString() + tr(",Port:") + QString::number(mPort) + tr(")"));
        }
        else
        {
            fAddRunMsg(tr("Server Start err"));
        }
    }
}

void V6Service::fServerStop()
{
    if(mServer)
    {
        mServer->close();
        fAddRunMsg(tr("Server Stoped"));
    }
}
