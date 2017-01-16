#include "Syreal.h"

#include "../../Math/Math.h"

#include <QDebug>

TSyreal::TSyreal() : TProtocol()
{}

int TSyreal::fAnaly(TConnection *Conn)
{
    if(NULL == Conn)
    {
        return PROTOCOL_IDLE;
    }

    const int iLength = 256;
    unsigned char pcRecv[iLength];

    TRingBuf *RingBuf = Conn->fGetReadRingbuf();

    if(NULL == RingBuf || RingBuf->fDataSize() <= 0)
    {
        return PROTOCOL_IDLE;
    }

    int iRead = RingBuf->fRead(256, pcRecv);

    //qDebug() << "iRead = " << iRead;

    if(iRead <= 0)
    {
        return PROTOCOL_IDLE;
    }

    //int Value = 0;

    //for(int i = 0; i < iRead; i += 4)
    for(int i = 0; i < iRead; i++)
    {
        /*Value = (pcRecv[i] << 24)
                | (pcRecv[i + 1] << 16)
                | (pcRecv[i + 2] << 8)
                | pcRecv[i + 3];

        qDebug() << "Syreal emit" << Value;

        fSendSignal(Conn, 1, Value, 0);*/

        //qDebug() << "Syreal emit" << pcRecv[i];
        fSendSignal(Conn, 1, pcRecv[i], 0);
        fSendSignal(Conn, 2, pcRecv[i], 0);
        fSendSignal(Conn, 3, pcRecv[i], 0);
    }

    return PROTOCOL_OK;
}
