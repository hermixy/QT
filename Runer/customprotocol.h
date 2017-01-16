#ifndef CUSTOMPROTOCOL_H
#define CUSTOMPROTOCOL_H
#include "../Scada/Src/Math/Math.h"

#include <QObject>
#include <QByteArray>
struct SenderCheck{
    QString ip;
    quint16 startAddr;
    quint8 deviceAddr;
    quint16 regeistAddr;
    quint16 value;
    int resendnum=0;
};

class CustomProtocol : public QObject
{
    Q_OBJECT

public:
    explicit CustomProtocol(QObject *parent = 0);
    ~CustomProtocol();
    static quint16 getAddr(quint16 addr);
    static quint16 getThreeAddr(quint16 startAddr,quint16 addr);
    static QByteArray sendOrderOne(quint16 startAddr,quint8 deviceAddr, quint16 regeistAddr, quint16 value, QString modbuspro="modbus rtu");               //上位机发送命令一个
    static QByteArray sendOrderMore(quint16 startAddr,quint8 deviceAddr, quint16 regeistAddr, quint16 num, QList<quint16> value);               //上位机发送命令多个
    static QByteArray Response(quint8 deviceAddr);                //上位机响应
    static QByteArray senderCheck(quint16 startAddr,quint8 deviceAddr, quint16 regeistAddr, quint16 num, QString modbuspro="modbus rtu");              //上位机发送查询指令
    static QByteArray senderCheckThree(quint16 startAddr,quint8 deviceAddr, quint16 regeistAddr, quint16 num);              //上位机发送查询指令

    static QByteArray sendOrderOneThree(quint16 startAddr,quint8 deviceAddr, quint16 regeistAddr, quint16 value);



    //static QByteArray reciveData(QByteArray data);
    //加密
    static QByteArray getMd5Hash(QByteArray data);
    static QByteArray get_localmachine_mac();


    static QList<SenderCheck > sendOnereciver;
    static quint8 uh_ASCIIh_124(quint8 uh_124);
    static quint8 uh_ASCIIl_124(quint8 uh_124);

    static quint8  lrc_124(char *p_124, quint8 usDataLen_124);

    static quint8 ASCII_uh_124(quint8 asciih_124,quint8 asciil_124);  //把两个ASCII码转换为10进制的值


signals:

public slots:
};

#endif // CUSTOMPROTOCOL_H
