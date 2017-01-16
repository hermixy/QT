#include "TcpSocket.h"

#include <QHostAddress>
#include <QDebug>
#include <QThread>

TTcpSocket::TTcpSocket(QObject *parent) : TConnection(TCON_SOCKET_TCP)
{
    //qDebug() << "create TTcpSocket at" << QThread::currentThread();

    //mIp = tr("");
    mPort = 0;

    mSocket = new QTcpSocket(parent);

    //connect(mSocket, SIGNAL(readyRead()), this, SLOT(fRead()));
    //connect(mSocket, SIGNAL(disconnected()), this, SLOT(fSocketDisconnect()));

    // 监视连接状态
    //connect(mSocket, SIGNAL(connected()), this, SLOT(fSocketConnected()));

    // 必须添加参数Qt::DirectConnection，否则报错：
    // QObject::connect: Cannot queue arguments of type 'QAbstractSocket::SocketError',
    // (Make sure 'QAbstractSocket::SocketError' is registered using qRegisterMetaType().)
    //connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)),
    //        this, SLOT(fSocketConnectErr(QAbstractSocket::SocketError))/*,Qt::DirectConnection*/);
}

int TTcpSocket::fStart()
{
    if(mSocket && mIp.length() > 0 && mPort > 0)
    {
        //mSocket->disconnectFromHost();
        mSocket->abort();

        qDebug() << "TTcpSocket::fStart() connect to :"
                 << mIp << mPort << QThread::currentThread();

        mSocket->connectToHost(QHostAddress(mIp), mPort);

        if(mSocket->waitForConnected(5 * 1000))
        {
            fSocketConnected();
        }
        else
        {
            qDebug() << mSocket->error() << mSocket->errorString();
            return ERR;
        }
    }

    return OK;
}

int TTcpSocket::fStop()
{

    if(mSocket && mSocket->isOpen())
    {
        fSocketDisconnect();

        //mSocket->disconnectFromHost();
        //mSocket->waitForDisconnected();

        return OK;
    }
    else
    {
        return ERR;
    }
}

bool TTcpSocket::fSetHost(QString Ip, int Port)
{
    mIp = Ip;
    mPort = Port;

    return true;
}

TTcpSocket::~TTcpSocket()
{
    qDebug() << "~TTcpSocket()";

    if(mRingbufForRead)
    {
        delete mRingbufForRead;
        mRingbufForRead = NULL;
    }

    if(mRingbufForWrite)
    {
        delete mRingbufForWrite;
        mRingbufForWrite = NULL;
    }

    if(mSocket)
    {
        mSocket->deleteLater();
    }
}

void TTcpSocket::fSocketDisconnect()
{
    qDebug() << "TTcpSocket::fSocketDisconnect";
    fSetState(TConnection::STATE_CLOSEED);
}

void TTcpSocket::fSocketConnected()
{
    qDebug() << "TTcpSocket::fSocketConnected";
    fSetState(TConnection::STATE_RUNED);
}

void TTcpSocket::fSocketConnectErr(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);

    qDebug() << "TTcpSocket::fSocketConnectErr:";// << err;
    mSocket->abort();
    fSetState(TConnection::STATE_INIT);
}

int TTcpSocket::fRecv(void *Dst, int Len)
{
    int iRead = 0;

    if(mSocket->waitForReadyRead(10))
    {
        iRead = mSocket->read((char *)Dst, Len);

        //qDebug() << "TTcpSocket::fRecv() iRead" << iRead;

        // 交给上层去处理
        /*if(iRead < 0)
        {
            fSocketDisconnect();
        }*/
    }

    return iRead;
}

int TTcpSocket::fSend(const void *Data, int Len)
{
    qDebug() << "TTcpSocket::fSend at" << QThread::currentThread();;

    int Ret = -1;

    if(mSocket && STATE_RUNED == fGetState())
    {
        qDebug() << "in TTcpSocket write:" << Len;
        Ret = mSocket->write((const char *)Data, Len);

        if(!mSocket->waitForBytesWritten(1000))
        {
            fSocketDisconnect();
        }

        qDebug() << "in TTcpSocket ret:" << Ret;
    }

    //qDebug() << "in TTcpSocket send ret:" << Ret;

    return Ret;
}

bool TTcpSocket::fSetSocketDescriptor(qintptr handle)
{
    bool Ret = false;

    if(mSocket)
    {
        Ret = mSocket->setSocketDescriptor(handle);
    }

    return Ret;
}

QString TTcpSocket::fGetPeerName()
{
    //QString Ret = tr("");
    QString Ret;

    if(mSocket)
    {
        Ret = mSocket->peerName();
    }

    return Ret;
}

QString TTcpSocket::fGetPeerAddr()
{
    //QString Ret = tr("");
    QString Ret;

    if(mSocket)
    {
        Ret = mSocket->peerAddress().toString();
    }

    return Ret;
}

QString TTcpSocket::fGetPeerPort()
{
    //QString Ret = tr("");
    QString Ret;

    if(mSocket)
    {
        Ret = QString::number(mSocket->peerPort());
    }

    return Ret;
}
