#include "timersendthread.h"
#include "reciverhandle.h"
#include "analysisthread.h"
#include "../Scada/Src/Math/Math.h"
#include "../Runer/Runer.h"
#include "customprotocol.h"
#include "senderorder.h"
QHash<QString, QList<QByteArray>> TimerSendThread::sendList;
QHash<QString, QList<QByteArray>> TimerSendThread::sendThreeList;
//QHash<QString, QList<int>> TimerSendThread::ipToStarr;

TimerSendThread::TimerSendThread(QObject *parent):QThread(parent)
{
    /*timer=new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSeckThree()));
    timer->start(1000);*/
}

TimerSendThread::~TimerSendThread()
{

}

void TimerSendThread::run()
{
    int sendNum=0, timeNum=0;
    QUdpSocket socket;
    qDebug()<<"start TimerSendThread";
    while(!sTop)
    {
        //qDebug()<<"TimerSendThread::sendList"<<sendList.size();
        SenderOrder::sendTimerByte.lock();
        for(QHash<QString, QList<QByteArray>>::iterator ia=TimerSendThread::sendList.begin(); ia!=TimerSendThread::sendList.end(); ia++)
        {
            if(!ia.value().isEmpty())
            {
                QByteArray sendBuyte=ia.value().takeFirst();

                this->msleep(1);
                timeNum++;
                //timeNum++;
                socket.writeDatagram(sendBuyte, QHostAddress(ia.key()), 5001);

            } else {
                TimerSendThread::sendList.remove(ia.key());
            }
        }
        if(timeNum>1000)
        {
            //qDebug()<<TimerSendThread::sendThreeList.size();
            timeNum=0;

            for(QHash<QString, QList<QByteArray>>::iterator ia=TimerSendThread::sendThreeList.begin(); ia!=TimerSendThread::sendThreeList.end(); ia++)
            {
                //qDebug()<<ia.value().isEmpty();
                if(!ia.value().isEmpty())
                {
                    //qDebug()<<"ThreeAddr"<<ia.value().size()<<ia.value().first().toHex();
                    QByteArray sendBuyte=ia.value().takeFirst();
                    this->msleep(1);

                    socket.writeDatagram(sendBuyte, QHostAddress(ia.key()), 5001);

                } else {
                    TimerSendThread::sendList.remove(ia.key());
                }
            }
        }

        SenderOrder::sendTimerByte.unlock();
        this->msleep(5);
        timeNum += 5;

    }
    qDebug()<<"stop TimerSendThread";
}
void TimerSendThread::stop()
{
    sTop=true;
    //timer->stop();
    //delete timer;
}
/*void TimerSendThread::sendSeckThree()
{
    QUdpSocket socket;
    SenderOrder::sendTimerByte.lock();

    SenderOrder::sendTimerByte.unlock();
}*/
