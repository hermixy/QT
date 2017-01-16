#ifndef MATH_H
#define MATH_H

#include <QApplication>
#include <QString>

class TMath
{
    Q_DECLARE_TR_FUNCTIONS(TMath)

private:
    TMath();
    ~TMath();

public:    
    static double fSqrt(double Value);
    static double fPow(double Value, double Order);

    static double fSin(double Value);
    static double fCos(double Value);
    static double fTan(double Value);
    static double fArcSin(double Value);
    static double fArcCos(double Value);
    static double fArcTan(double Value);

    static int fCheckSum(const void *pcSrc, int iLen);
    static int fCrc16(const unsigned char *puchMsg, unsigned short usDataLen);
    static quint16 s_Crc16Bit(quint8 *p_uch_Data, quint16 uin_CrcDataLen);

    // 低前高后
    static int fGetBytes(const unsigned char *_Src, int _Offset, int _Len);

    // 高前低后
    static int fGetBytesHighFront(const unsigned char *_Src, int _Offset, int _Len);
    static void fSetBytes(char *_pcDst, int _iStart, int _iBytes, int _iValue);
    static int fGetBits(int _Byte, int _Offset, int _Len);

    static int fFloatToInt(float f);
    static float fIntToFloat(int i);

    static bool fCheckIp(const QString &Ip);
    static bool fCheckPort(const int Port);

    static qint64 fGetCurrentTime();
    static quint8 LRC(char *auchMsg,unsigned short usDataLen);

};

#endif // MATH_H
