#include "Com.h"
#include <QDebug>

TCom::TCom(QObject *parent) : TConnection(TCON_COM)
{
    mSerial = new QSerialPort(parent);

    fSetState(STATE_INIT);

    mRate = 9600;
    mDataBits = QSerialPort::Data8;
    mStopBits = QSerialPort::OneStop;
    mParity = QSerialPort::NoParity;
}

TCom::~TCom()
{
    fStop();
    mSerial->deleteLater();
}

bool TCom::fSetPortName(QString Name)
{
    mName = Name;
    return true;
}

bool TCom::fSetBaudRate(int Rate)
{
    mRate = Rate;

    return true;
}

int TCom::fGetBaudRate()
{
    return mRate;
}

bool TCom::fSetDataBits(int DataBits)
{
    bool Ret = false;

    switch(DataBits)
    {
    case 5:
        Ret = true;
        mDataBits = QSerialPort::Data5;
        break;

    case 6:
        Ret = true;
        mDataBits = QSerialPort::Data6;
        break;

    case 7:
        Ret = true;
        mDataBits = QSerialPort::Data7;
        break;

    case 8:
        Ret = true;
        mDataBits = QSerialPort::Data8;
        break;

    default:
        break;
    }

    return Ret;
}

bool TCom::fSetParity(int Parity)
{
    bool Ret = false;

    switch(Parity)
    {
    // 无校验(None)
    case 0:
        Ret = true;
        mParity = QSerialPort::NoParity;
        break;

    // 奇校验(Odd)
    case 1:
        Ret = true;
        mParity = QSerialPort::OddParity;
        break;

    // 偶校验(Even)
    case 2:
        Ret = true;
        mParity = QSerialPort::EvenParity;
        break;

    // 1校验(Mark)
    case 3:
        Ret = true;
        mParity = QSerialPort::MarkParity;
        break;

    // 0校验(Space)
    case 4:
        Ret = true;
        mParity = QSerialPort::SpaceParity;
        break;

    default:
        break;
    }

    return Ret;
}

bool TCom::fSetStopBits(int StopBits)
{
    bool Ret = false;

    switch(StopBits)
    {
    // 1
    case 0:
        Ret = true;
        mStopBits = QSerialPort::OneStop;
        break;

    // 1.5
    case 1:
        Ret = true;
        mStopBits = QSerialPort::OneAndHalfStop;
        break;

    // 2
    case 2:
        Ret = true;
        mStopBits = QSerialPort::TwoStop;
        break;

    default:
        break;
    }

    return Ret;
}

int TCom::fStart()
{
    if(mSerial)
    {
        mSerial->setPortName(mName);

        qDebug() << "open com" << mName;

        if(mSerial->open(QIODevice::ReadWrite))
        {
            mSerial->setBaudRate(mRate);
            mSerial->setDataBits(mDataBits);
            mSerial->setStopBits(mStopBits);
            mSerial->setParity(mParity);
            mSerial->setFlowControl(QSerialPort::NoFlowControl);

            fCreateReadRingbuf(1024);

            TConnection::fStart();
        }
        else
        {
            qDebug() << "open com err";
        }
    }    

    return -1;
}

int TCom::fStop()
{
    if(mSerial && mSerial->isOpen())
    {
        mSerial->close();
    }

    TConnection::fStop();

    return 0;
}

int TCom::fRecv(void *Dst, int Len)
{
    int iRet = -1;

    //qDebug() << "TCom::fRecv" << mSerial << mSerial->isOpen() << mSerial->isReadable();

    if(mSerial && mSerial->isOpen() && mSerial->isReadable())
    {
        //mSerial->flush();

        if(mSerial->waitForReadyRead(10))
        {
            iRet = mSerial->read((char *)Dst, Len);
        }

        /*if(iRet > 0)
        {
            qDebug() << "TCom::fRecv" << iRet;
        }*/
    }

    return iRet;
}

int TCom::fSend(const void *Data, int Len)
{
    int iRet = -1;

    //qDebug() << "TCom::fSend at" << QThread::currentThread();

    if(mSerial && mSerial->isOpen() && mSerial->isWritable())
    {

#if false
        QString StrSend;
        for(int i = 0; i < Len; i++)
        {
            StrSend += QString::number(((unsigned char *)Data)[i], 16) + tr(" ");
        }

        qDebug() << StrSend;
#endif

        iRet = mSerial->write((const char *)Data, Len);

        //qDebug() << "Com Send" << iRet;
        mSerial->waitForBytesWritten(10);
    }

    return iRet;
}

