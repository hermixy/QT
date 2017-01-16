#ifndef IEC104_SERVICE_H
#define IEC104_SERVICE_H

#include "Iec104.h"
#include "../Protocol.h"
#include "../../Service/Service.h"

class TIec104Service : public TProtocol
{
    Q_OBJECT

public:
    TIec104Service(TIec104Data *Iec104Data);
    int fAnaly(TConnection *Conn);

    enum
    {
        STATE_WAIT_ACTIVE = STATE_PARENT,       // 等待激活确认
        STATE_ACTIVE                            // 连接激活，发送总召
    };

private:
    TIec104 *mIec104;

    int fRecv(TConnection *Conn, int WaitTimeMS);

    void fSendData(TConnection *Conn, const char *_pcSend, int _iLen);

    void fSendRepleS(TConnection *Conn, int _VrLocal);
    void fSendRepleU(TConnection *Conn, int _Control);
    void fSendState(TConnection *Conn, int _Type, int _Reason, int _QOI);

    void fSendYK(TConnection *Conn, int _Addr, int _YKType, int _YKValue);

    void fAnalyData(TConnection *Conn, const unsigned char *_pcRecv, int _iLen);
};

#endif // IEC104_H
