#include "customprotocol.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QNetworkInterface>
#include <QSettings>
QList<SenderCheck> CustomProtocol::sendOnereciver;

quint16 CustomProtocol::getAddr(quint16 addr)
{
    if(addr<24)
    {
        return ((quint16)0x0100+addr);
    }
    if(addr<48)
    {
        return ((quint16)0x0200+quint16(addr-24));
    }
    if(addr<72)
    {
        //qDebug()<<(quint16)0x0300+quint16(addr-72);
        return ((quint16)0x0300+quint16(addr-48));

    }
    if(addr<96)
    {
        return ((quint16)0x0400+quint16(addr-72));
    }
}
quint16 CustomProtocol::getThreeAddr(quint16 startAddr,quint16 addr)
{

     return ((quint16)startAddr+addr);
}

QByteArray CustomProtocol::sendOrderOne(quint16 startAddr, quint8 deviceAddr, quint16 regeistAddr, quint16 value, QString modbuspro)
{

    QByteArray order;
    QDataStream dataStream(&order, QIODevice::WriteOnly);
    if((deviceAddr>=16)&&(modbuspro==QLatin1String("modbus ascii")))
    {
        //return sendOrderOneThree(startAddr, deviceAddr, regeistAddr, value);
        dataStream<<quint8(':');
    }
    dataStream<<deviceAddr<<(quint8)0x06<<(deviceAddr<16?getAddr(regeistAddr):getThreeAddr(startAddr, regeistAddr))<<value;
    quint16 tempCrc=(quint16)TMath::fCrc16((unsigned char *)order.data(), order.size());
    dataStream<<(quint8)(tempCrc)<<(quint8)(tempCrc>>8);

    //qDebug()<<"sendOrderOne"<<order.toHex();
    return order;

}
QByteArray CustomProtocol::Response(quint8 deviceAddr)
{

    QByteArray order;
    QDataStream dataStream(&order, QIODevice::WriteOnly);
    dataStream<<deviceAddr<<(quint8)0x04;

    quint16 tempCrc=(quint16)TMath::fCrc16((unsigned char *)order.data(), order.size());
    dataStream<<(quint8)(tempCrc)<<(quint8)(tempCrc>>8);
    qDebug()<<order.toHex()<<order.size();
    return order;
}
QByteArray CustomProtocol::sendOrderMore(quint16 startAddr, quint8 deviceAddr, quint16 regeistAddr, quint16 num, QList<quint16> value)
{
    QByteArray order;
    QDataStream dataStream(&order, QIODevice::WriteOnly);

    dataStream<<deviceAddr<<(quint8)16<<(deviceAddr<16?getAddr(regeistAddr):getThreeAddr(startAddr,regeistAddr));
    dataStream<<(quint16)num<<(quint16)(num*2);
    foreach(quint16 valuses, value)
    {

        dataStream<<valuses;
    }
    quint16 tempCrc=(quint16)TMath::fCrc16((unsigned char *)order.data(), order.size());
    dataStream<<(quint8)(tempCrc)<<(quint8)(tempCrc>>8);
    //qDebug()<<order.toHex();
    return order;

}
QByteArray CustomProtocol::senderCheck(quint16 startAddr, quint8 deviceAddr, quint16 regeistAddr, quint16 num, QString modbuspro)
{
    /*if((regeistAddr>=16)&&(modbuspro==QLatin1String("modbus ascii")))
    {
        return senderCheckThree(startAddr, deviceAddr, regeistAddr, num);
    }*/

    QByteArray order;
    QDataStream dataStream(&order, QIODevice::WriteOnly);
   // qDebug()<<modbuspro;
    if((deviceAddr>=16)&&(modbuspro==("modbus ascii")))
    {
       //return senderCheckThree(startAddr, deviceAddr, regeistAddr, num);
        dataStream<<quint8(':');
    }
    dataStream<<deviceAddr<<(quint8)0x03<<(deviceAddr<16?getAddr(regeistAddr):getThreeAddr(startAddr,regeistAddr))<<num;
    quint16 tempCrc=(quint16)TMath::fCrc16((unsigned char *)order.data(), order.size());
    dataStream<<(quint8)(tempCrc)<<(quint8)(tempCrc>>8);
    //qDebug()<<"senderCheck"<<order.toHex();
    return order;
}
QByteArray CustomProtocol::senderCheckThree(quint16 startAddr, quint8 deviceAddr, quint16 regeistAddr, quint16 num)
{
    QByteArray order;
    QDataStream dataStream(&order, QIODevice::WriteOnly);
    dataStream<<quint8(':')<<uh_ASCIIh_124(deviceAddr)<<uh_ASCIIl_124(deviceAddr);
    dataStream<<(quint8)('0')<<quint8('3');
    quint16 threeAddr=getThreeAddr(startAddr, regeistAddr);

    dataStream<<uh_ASCIIh_124(quint8(threeAddr>>8))<<uh_ASCIIl_124(quint8(threeAddr>>8));
    dataStream<<uh_ASCIIh_124(threeAddr)<<uh_ASCIIl_124(threeAddr);
    dataStream<<uh_ASCIIh_124(num>>8)<<uh_ASCIIl_124(num>>8);
    dataStream<<uh_ASCIIh_124(num)<<uh_ASCIIl_124(num);

    quint8 lrc=lrc_124(order.data(), (order.size()-1)/2);

    dataStream<<uh_ASCIIh_124(lrc)<<uh_ASCIIl_124(lrc);
    dataStream<<quint8('\r')<<quint8('\n');

    return order;
}

QByteArray CustomProtocol::getMd5Hash(QByteArray data)
{
    QByteArray hash_byte_array = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    return hash_byte_array.toHex();
}
QByteArray CustomProtocol::get_localmachine_mac()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
    QString md5;
    foreach(QNetworkInterface ni,nets)
    {
        md5+=ni.hardwareAddress();
    }
    QSettings setting("inis", QSettings::IniFormat);

    setting.setIniCodec("UTF-8");
    md5+=QString::number(setting.value("registNum", 0).toInt());
    return QByteArray(md5.toUtf8());
}
QByteArray CustomProtocol::sendOrderOneThree(quint16 startAddr, quint8 deviceAddr, quint16 regeistAddr, quint16 value)
{

    QByteArray order;
    QDataStream dataStream(&order, QIODevice::WriteOnly);
    dataStream<<quint8(':')<<uh_ASCIIh_124(deviceAddr)<<uh_ASCIIl_124(deviceAddr);
    dataStream<<(quint8)('0')<<quint8('6');
    quint16 threeAddr=getThreeAddr(startAddr, regeistAddr);

    dataStream<<uh_ASCIIh_124(quint8(threeAddr>>8))<<uh_ASCIIl_124(quint8(threeAddr>>8));
    dataStream<<uh_ASCIIh_124(threeAddr)<<uh_ASCIIl_124(threeAddr);
    dataStream<<uh_ASCIIh_124(value>>8)<<uh_ASCIIl_124(value>>8);
    dataStream<<uh_ASCIIh_124(value)<<uh_ASCIIl_124(value);
    //char ab[]=":10030402230203";

    //quint8 lrc=TMath::LRC(order.data()+1, order.size()-1);
    quint8 lrc=lrc_124(order.data(), (order.size()-1)/2);
    //quint8 lrc=lrc_124(ab, 7);

    dataStream<<uh_ASCIIh_124(lrc)<<uh_ASCIIl_124(lrc);
    dataStream<<quint8('\r')<<quint8('\n');

    return order;
}

CustomProtocol::CustomProtocol(QObject *parent) : QObject(parent)
{

}

CustomProtocol::~CustomProtocol()
{

}
quint8 CustomProtocol::uh_ASCIIh_124(quint8 uh_124)   //高四位转化为ascii
{
    quint8 temph_124;
    quint8 asciih_124;
    temph_124=uh_124/16;
    if(temph_124<10)  {
        asciih_124=temph_124+0x30;
    }   else   {
        asciih_124=temph_124+0x37;
    }
    return asciih_124;
}
quint8 CustomProtocol::uh_ASCIIl_124(quint8 uh_124)  //低四位转化为ascii
{
   quint8 templ_124;

   quint8 asciil_124;
   templ_124=uh_124%16;
   if(templ_124<10)  {
    asciil_124=templ_124+0x30;
 }  else  {
   asciil_124=templ_124+0x37;  }

 return asciil_124;
}
quint8  CustomProtocol::lrc_124(char *p_124, quint8 usDataLen_124)
{
    quint8  uchLRC_124 = 0 ;
    quint8  temp_124;
    quint8  q_124=0;
    while(usDataLen_124)
    {
        usDataLen_124--;
        q_124=q_124+2;
        //qDebug()<<p_124[q_124-1]<<p_124[q_124];
        temp_124= ASCII_uh_124(p_124[q_124-1],p_124[q_124]);
        //qDebug()<<temp_124;
        uchLRC_124 += temp_124;
    }
    uchLRC_124=255-uchLRC_124+1;
    return uchLRC_124;
}

quint8 CustomProtocol::ASCII_uh_124(quint8 asciih_124,quint8 asciil_124) //把两个ASCII码转换为10进制的值

{
    quint8 uh_124;
    if(asciih_124<=57)
        asciih_124=asciih_124-48;
    else
        asciih_124=asciih_124-55;
    if(asciil_124<=57)
        asciil_124=asciil_124-48;
    else
        asciil_124=asciil_124-55;
    uh_124=asciih_124*16+asciil_124;
    return uh_124;
}
