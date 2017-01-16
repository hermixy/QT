#ifndef COM_H
#define COM_H

#include "../Connection.h"

#include <QThread>
#include <QSerialPort>

class TCom : public TConnection
{
    //Q_OBJECT

public:
    TCom(QObject *parent = 0);
    ~TCom();

    bool fSetPortName(QString Name);
    bool fSetBaudRate(int Rate);
    int fGetBaudRate();
    bool fSetDataBits(int DataBits);
    bool fSetStopBits(int StopBits);
    bool fSetParity(int Parity);

    int fStart();
    int fStop();
    int fRecv(void *Dst, int Len);
    int fSend(const void *Data, int Len);

private:
    int mMode;
    QString mName;

    int mRate;
    QSerialPort::DataBits mDataBits;
    QSerialPort::StopBits mStopBits;
    QSerialPort::Parity mParity;

    QSerialPort *mSerial;
};

#endif // COM_H
