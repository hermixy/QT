#include "senderorder.h"
#include <QHostInfo>
#include <QTimer>
#include "customprotocol.h"
#include "../Scada/Dialog/Timer/timerframeitem1.h"
#include "../Scada/Dialog/Timer/onlinestatus.h"
#include "timersendthread.h"
#include "Runer.h"
#include <QMessageBox>
#include <math.h>
#include <QPair>
QMutex SenderOrder::mutex;
QMutex SenderOrder::sendTimerByte;
SenderOrder::SenderOrder(QObject *parent) : QObject(parent)
{
    //创建核心线程
    qRegisterMetaType<QTextBlock>("QTextBlock");
    qRegisterMetaType<QTextCursor>("QTextCursor");
    sendOrderThread=new QThread(this);
    sendOrderThread->setObjectName(tr("sendOrader thread"));

    udpSocket=new QUdpSocket();

    timer=new QTimer();
    SchedleTimer=new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSendBrocast()));
    connect(SchedleTimer, SIGNAL(timeout()), this, SLOT(ScheldTime()));

    sendOrderThread->start(QThread::LowPriority);
    moveToThread(sendOrderThread);

    /*************************初始化链表******************************************/
    foreach(TScene *sence, Runer::runner->mSceneList)
    {
        QList<TGraphics *> list;
        sence->fGetItemsList(list);
    foreach(TGraphics *graphics, list)
    {
        if(!Runer::runner->serIdToServer.contains(graphics->fGetServiceId()))
        {
            continue;
        }
        QByteArray commd=CustomProtocol::senderCheck(graphics->startAddr ,graphics->fGetMainId(),\
                                            graphics->fGetNodeId(), 1, graphics->threeDevName);

        TService *server= Runer::runner->serIdToServer.value(graphics->fGetServiceId());

        TServerData *serverdata=server->fGetServerData();

        if((graphics->fGetMainId()>=16))
        {
            //qDebug()<<">=16"<<graphics->fGetMainId()<<sendThreeList.size();
            if(sendThreeList.contains(serverdata->fGetIp())) {
            QList<QByteArray> &commlist=sendThreeList[serverdata->fGetIp()];
            if(!commlist.contains(commd))
                commlist.append(commd);

            } else {
                QList<QByteArray> commlist;
                commlist.append(commd);
                sendThreeList.insert(serverdata->fGetIp(), commlist);
            }
        }
        else {
            //qDebug()<<"<16"<<graphics->fGetMainId()<<graphics->fGetNodeId()<<sendList.size();

            if(sendList.contains(serverdata->fGetIp())) {
                QList<QByteArray> &commlist=sendList[serverdata->fGetIp()];
                if(!commlist.contains(commd))
                    commlist.append(commd);
            } else {
                QList<QByteArray> commlist;
                commlist.append(commd);
                sendList.insert(serverdata->fGetIp(), commlist);
            }
        }
    }
    }
    qDebug()<<sendThreeList.size();
    qDebug()<<sendList.size();
    /************************定时饼图***********************************/
    foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
    {
          TService *server= Runer::runner->serIdToServer.value(timer.serverId);
          if(server==nullptr)
          {
                 continue;
          }
          QByteArray commd=CustomProtocol::senderCheck(timer.startAddr ,timer.mainId,\
                                              timer.devId, 1);
          TServerData *serverdata=server->fGetServerData();
          if(serverdata==nullptr)
          {
                  continue;
          }
          if(timer.mainId>=16)
          {
              if(sendThreeList.contains(serverdata->fGetIp())) {
              QList<QByteArray> &commlist=sendThreeList[serverdata->fGetIp()];
              if(!commlist.contains(commd))
                  commlist.append(commd);

              } else {
                  QList<QByteArray> commlist;
                  commlist.append(commd);
                  sendThreeList.insert(serverdata->fGetIp(), commlist);
              }
          }
          else {

              if(sendList.contains(serverdata->fGetIp())) {
                  QList<QByteArray> &commlist=sendList[serverdata->fGetIp()];
                  if(!commlist.contains(commd))
                      commlist.append(commd);
              } else {
                  QList<QByteArray> commlist;
                  commlist.append(commd);
                  sendList.insert(serverdata->fGetIp(), commlist);
              }
          }
   }
    /******************************定时曲线***************************/
    foreach(TScene::TimerStruct timer, Runer::runner->PieStructList)
    {
          TService *server= Runer::runner->serIdToServer.value(timer.serverId);
          if(server==nullptr)
          {
                 continue;
          }
          QByteArray commd=CustomProtocol::senderCheck(timer.startAddr ,timer.mainId,\
                                              timer.devId, 1);
          TServerData *serverdata=server->fGetServerData();
          if(serverdata==nullptr)
          {
                  continue;
          }
          if(timer.mainId>=16)
          {
              if(sendThreeList.contains(serverdata->fGetIp())) {
              QList<QByteArray> &commlist=sendThreeList[serverdata->fGetIp()];
              if(!commlist.contains(commd))
                  commlist.append(commd);

              } else {
                  QList<QByteArray> commlist;
                  commlist.append(commd);
                  sendThreeList.insert(serverdata->fGetIp(), commlist);
              }
          }
          else {

              if(sendList.contains(serverdata->fGetIp())) {
                  QList<QByteArray> &commlist=sendList[serverdata->fGetIp()];
                  if(!commlist.contains(commd))
                      commlist.append(commd);
              } else {
                  QList<QByteArray> commlist;
                  commlist.append(commd);
                  sendList.insert(serverdata->fGetIp(), commlist);
              }
          }
   }
    /**************************************************************************/
    timer->start(1000);
    SchedleTimer->start(1000);
}

SenderOrder::~SenderOrder()
{
    qDebug()<<QThread::currentThread()->objectName();
    timer->stop();
    SchedleTimer->stop();
    sendOrderThread->exit(0);
    sendOrderThread->wait();
    delete udpSocket;
    delete timer;
    delete SchedleTimer;
    delete sendOrderThread;

}

void SenderOrder::sendOrder(TService *serverData,quint16 startaddr, int mainId, int secondId, int value)
{
    TServerData *serverdata=serverData->fGetServerData();
    const QByteArray command=CustomProtocol::sendOrderOne(startaddr, mainId, secondId, value);
    //qDebug()<<command.toHex();
    /************************************************/
    SenderOrder::sendTimerByte.lock();
    if(mainId>=16)
    {
        if(TimerSendThread::sendThreeList.contains(serverdata->fGetIp())) {
        QList<QByteArray> &commlist=TimerSendThread::sendThreeList[serverdata->fGetIp()];
        if(!commlist.contains(command))
            commlist.append(command);

        } else {
            QList<QByteArray> commlist;
            commlist.append(command);
            TimerSendThread::sendThreeList.insert(serverdata->fGetIp(), commlist);
        }
    }
    else {

        if(TimerSendThread::sendList.contains(serverdata->fGetIp())) {
            QList<QByteArray> &commlist=TimerSendThread::sendList[serverdata->fGetIp()];
            if(!commlist.contains(command))
                commlist.append(command);
        } else {
            QList<QByteArray> commlist;
            commlist.append(command);
            TimerSendThread::sendList.insert(serverdata->fGetIp(), commlist);
        }
    }


//    if(TimerSendThread::sendList.contains(serverdata->fGetIp())) {
//        QList<QByteArray> &commlist=TimerSendThread::sendList[serverdata->fGetIp()];
//        commlist.append(command);
//    } else {
//        QList<QByteArray> commlist;
//        commlist.append(command);
//        TimerSendThread::sendList.insert(serverdata->fGetIp(), commlist);
//    }
    SenderOrder::sendTimerByte.unlock();

   // qDebug()<<"发送";
    SenderCheck sendcheck;
    sendcheck.ip=serverdata->fGetIp();
    sendcheck.startAddr=startaddr;
    sendcheck.deviceAddr=mainId;
    sendcheck.regeistAddr=secondId;
    sendcheck.value=value;
    mutex.lock();
    CustomProtocol::sendOnereciver.append(sendcheck);
    mutex.unlock();
}
QString SenderOrder::getIp()
{
    QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
    if (info.error() != QHostInfo::NoError)
    {
            qDebug() << "Lookup failed:" << info.errorString();
            return QString();
     }

       for (int i = 0;i < info.addresses().size();i++)
       {
            if(info.addresses()[i].toString().left(7)=="192.168")
            {
                qDebug()<<info.addresses()[i].toString();
                return info.addresses()[i].toString();
            }

       }
}


void SenderOrder::timerSendBrocast()
{
    static int i=0, j=0, time=0, popTime=0;
    static bool sendThreeinEmpty=true, threadLitEmpty=true;
    static double dayTIme=24*60.0*60;
    static bool start=false;
    static bool prompt=true;
    static bool prompt1=true;
    if(start==false)
    {
        QSettings setting("inis", QSettings::IniFormat);
        setting.setIniCodec("UTF-8");
        start=setting.value("start", false).toBool();
    }
    sendThreeinEmpty=true;
//    for(QHash<QString, QList<QByteArray>>::iterator ia=TimerSendThread::sendThreeList.begin(); ia!=TimerSendThread::sendThreeList.end(); ia++)
//    {
//        if(!ia.value().isEmpty())
//        {
//            sendThreeinEmpty=false;
//        }
//    }
    if(!TimerSendThread::sendThreeList.isEmpty()) {
        sendThreeinEmpty= false;
    }
    threadLitEmpty=true;
//    for(QHash<QString, QList<QByteArray>>::iterator ia=TimerSendThread::sendList.begin(); ia!=TimerSendThread::sendList.end(); ia++)
//    {
//        if(!ia.value().isEmpty())
//        {
//            threadLitEmpty=false;
//        }
//    }
    if(!TimerSendThread::sendList.isEmpty()) {
        threadLitEmpty= false;
    }

    /*******************定时发送查询**********************************/
    SenderOrder::sendTimerByte.lock();
    if(sendThreeinEmpty)
        TimerSendThread::sendThreeList=this->sendThreeList;
    if(threadLitEmpty) {
        TimerSendThread::sendList=this->sendList;
    }

    SenderOrder::sendTimerByte.unlock();
    if((i++)==1)        //三秒重发
    {
        mutex.lock();
        int sendNUM=0;
        foreach(const SenderCheck &send, CustomProtocol::sendOnereciver)
        {
            //qDebug()<<"sendOnereciver"<<sendNUM;
            QByteArray command=CustomProtocol::sendOrderOne(send.startAddr,send.deviceAddr, send.regeistAddr, send.value);
            //qDebug()<<"重发"<<command.toHex();
            SenderOrder::sendTimerByte.lock();
//            if(TimerSendThread::sendList.contains(send.ip)) {
//                QList<QByteArray> &commlist=TimerSendThread::sendList[send.ip];
//                commlist.append(command);
//            } else {
//                QList<QByteArray> commlist;
//                commlist.append(command);
//                TimerSendThread::sendList.insert(send.ip, commlist);
//            }
            /*****************************************************/
            if(send.deviceAddr>=16)
            {
                if(TimerSendThread::sendThreeList.contains(send.ip)) {
                QList<QByteArray> &commlist=TimerSendThread::sendThreeList[send.ip];
                if(!commlist.contains(command))
                    commlist.append(command);

                } else {
                    QList<QByteArray> commlist;
                    commlist.append(command);
                    TimerSendThread::sendThreeList.insert(send.ip, commlist);
                }
            }
            else {
                qDebug()<<"重发"<<command.toHex();

                if(TimerSendThread::sendList.contains(send.ip)) {
                    QList<QByteArray> &commlist=TimerSendThread::sendList[send.ip];
                    if(!commlist.contains(command))
                        commlist.append(command);
                } else {
                    QList<QByteArray> commlist;
                    commlist.append(command);
                    TimerSendThread::sendList.insert(send.ip, commlist);
                }
            }


            if(CustomProtocol::sendOnereciver[sendNUM].resendnum++>=3)
            {
                CustomProtocol::sendOnereciver.removeAt(sendNUM);

            }
            else
            {
                sendNUM++;
            }
            SenderOrder::sendTimerByte.unlock();
        }

        //CustomProtocol::sendOnereciver.clear();
        mutex.unlock();
        i=0;
    }

    if((j++)==30)
    {
    foreach(TScene *sence, Runer::runner->mSceneList)
    {
        QList<TGraphics *> list;
        sence->fGetItemsList(list);
        foreach(TGraphics *graphics,list)
        {
            TService *server= Runer::runner->serIdToServer.value(graphics->fGetServiceId());
            if(server==nullptr)
            {
                continue;
            }
            if(graphics->setValue>1)
            {
                graphics->setValue=1;
                if(Runer::runner->onlinestatus)
                    Runer::runner->onlinestatus->ipToLabel.value(server->fGetServerData()->fGetIp()).at(2)->setText("在线");
            }
            else
            {

                //qDebug()<<graphics->fGetNameId()+ "::"+ tr("掉线")+QDateTime::currentDateTime().toString()+"\r\n";
                if(graphics->setValue==0)
                    continue;
                else
                {
                    graphics->setValue=0;
                    graphics->update();
                }
                /********************************************************/
                if(Runer::runner->onlinestatus)
                {
                    Runer::runner->onlinestatus->ipToLabel.value(server->fGetServerData()->fGetIp()).at(2)->setText("离线");
                    //Runer::runner->textBrowser->setText(graphics->fGetNameId()+ "::"+ tr("掉线")+"::"+QDateTime::currentDateTime().toString()+"\r\n");
                }

                /**********************************************************/
                QTextStream warout;    //警告
                QFile file("droplog.txt");
                if (file.size() > 1000000)
                {
                    QFileInfo fileInfo("droplog.txt");
                    QDir dir (fileInfo.absoluteDir());

                    if (dir.remove("droplog.txt"))
                        qDebug() << "Removed old log successfully";
                    else
                        qWarning() << "Unable to remove old log file";
                }
                file.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text);
                warout.setDevice(&file);
                warout<<graphics->fGetNameId()+ "::"+ tr("掉线")+"::"+QDateTime::currentDateTime().toString()+"\r\n";

            }

        }
    }
    j=0;
    }

    if(start)
    {
    if(time++>60)
    {
        QSettings setting("inis", QSettings::IniFormat);
        setting.setIniCodec("UTF-8");
        qlonglong times=setting.value("time", 0).toLongLong();
        if(prompt&&(times/dayTIme==300))
        {
            QMessageBox::warning(Runer::runner, tr("系统即将过期"), tr("系统即将过期"));
            prompt=false;
        }
        if(prompt1&&(times/dayTIme==330))
        {
            QMessageBox::warning(Runer::runner, tr("系统即将过期"), tr("系统即将过期"));
            prompt1=false;
        }
        if(times/dayTIme>=365)
        {
            qApp->quit();
        }
        else
        {
            setting.setValue("time", times+60);

        }
        time=0;
    }
    }


    /******************************************************************/
    //定时发送查询
    /***************************************************************/
       /* foreach(TScene *sence, Runer::runner->mSceneList)
        {
            QList<TGraphics *> list;
            sence->fGetItemsList(list);
            //qDebug()<<"Runer::runner->mSceneList"<<list.size();
            foreach(TGraphics *graphics,list)
            {
                //qDebug()<<list.size();
                if(!Runer::runner->serIdToServer.contains(graphics->fGetServiceId()))
                {
                    continue;
                }
                QByteArray commd=CustomProtocol::senderCheck(graphics->startAddr ,graphics->fGetMainId(),\
                                                    graphics->fGetNodeId(), 1, graphics->threeDevName);

                TService *server= Runer::runner->serIdToServer.value(graphics->fGetServiceId());

                TServerData *serverdata=server->fGetServerData();

                SenderOrder::sendTimerByte.lock();
                if(sendThreeinEmpty&&(graphics->fGetMainId()>=16))
                {
                    //qDebug()<<graphics->fGetMainId();
                    if(TimerSendThread::sendThreeList.contains(serverdata->fGetIp())) {
                    QList<QByteArray> &commlist=TimerSendThread::sendThreeList[serverdata->fGetIp()];
                    commlist.append(commd);

                    } else {
                        QList<QByteArray> commlist;
                        commlist.append(commd);

                        TimerSendThread::sendThreeList.insert(serverdata->fGetIp(), commlist);
                    }
                }
                else {

                if(TimerSendThread::sendList.contains(serverdata->fGetIp())) {
                    QList<QByteArray> &commlist=TimerSendThread::sendList[serverdata->fGetIp()];
                    commlist.append(commd);
                } else {
                    QList<QByteArray> commlist;
                    commlist.append(commd);
                    TimerSendThread::sendList.insert(serverdata->fGetIp(), commlist);
                }
                }
                SenderOrder::sendTimerByte.unlock();

            }
        }*/
}
void SenderOrder::ScheldTime()
{
     //Cur
    /* foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
     {
           TService *server= Runer::runner->serIdToServer.value(timer.serverId);
           if(server==nullptr)
           {
                  continue;
           }
           QByteArray commd=CustomProtocol::senderCheck(timer.startAddr ,timer.mainId,\
                                               timer.devId, 1);
           TServerData *serverdata=server->fGetServerData();
           if(serverdata==nullptr)
           {
                   continue;
           }

           SenderOrder::sendTimerByte.lock();
           if(TimerSendThread::sendList.contains(serverdata->fGetIp())) {
               QList<QByteArray> &commlist=TimerSendThread::sendList[serverdata->fGetIp()];
               commlist.append(commd);
           } else {
               QList<QByteArray> commlist;
               commlist.append(commd);
               TimerSendThread::sendList.insert(serverdata->fGetIp(), commlist);
           }
           SenderOrder::sendTimerByte.unlock();
    }
     //Pie
     foreach(TScene::TimerStruct timer, Runer::runner->PieStructList)
     {
           TService *server= Runer::runner->serIdToServer.value(timer.serverId);
           if(server==nullptr)
           {
                  continue;
           }
           QByteArray commd=CustomProtocol::senderCheck(timer.startAddr ,timer.mainId,\
                                               timer.devId, 1);
           TServerData *serverdata=server->fGetServerData();
           if(serverdata==nullptr)
           {
                   continue;
           }

           SenderOrder::sendTimerByte.lock();
           if(TimerSendThread::sendList.contains(serverdata->fGetIp())) {
               QList<QByteArray> &commlist=TimerSendThread::sendList[serverdata->fGetIp()];
               commlist.append(commd);
           } else {
               QList<QByteArray> commlist;
               commlist.append(commd);
               TimerSendThread::sendList.insert(serverdata->fGetIp(), commlist);
           }
           SenderOrder::sendTimerByte.unlock();
    }*/
    //定时时间表
    int week=QDate::currentDate().dayOfWeek();
    QTime Time=QTime::currentTime();
    QMutexLocker(&Runer::runner->mutex);

    for(QHash<TScene *, QHash<QString, QList<TimerOnOff>>>::iterator ia=Runer::runner->sceneToTImer.begin(); ia!=Runer::runner->sceneToTImer.end(); ia++)
    {
        for(QHash<QString, QList<TimerOnOff>>::iterator j=ia.value().begin(); j!=ia.value().end(); j++)
        {
            QList<TimerOnOff> timelist= j.value();
            {

                QString TimerId=j.key();
                for(int i=0; i<timelist.count(); i++)
                {
                    //qDebug()<<j.key()<<";;;"<<timelist.at(i).day<<timelist.at(i).onOff<<timelist.at(i).time.toString();
                    if(timelist.at(i).day!=week)
                    {
                        continue;
                    }
                    if(timelist.at(i).onOff==-1)
                    {
                        continue;
                    }
                    if(abs(timelist.at(i).time.secsTo(Time))==0)
                    {
                        //qDebug()<<"on off";
                        int onoff=timelist.at(i).onOff;
                        QList<TScene::TimerStruct> MymerStructList=ia.key()->TImerStructList;
                        foreach(TScene::TimerStruct timer, MymerStructList)
                        {
                            //qDebug()<<timer.TimerId<<TimerId<<timer.writeNum;

                            if(timer.TimerId==TimerId)
                            {
                                    qDebug()<<j.key()<<";;;"<<timelist.at(i).day<<timelist.at(i).onOff<<timelist.at(i).time.toString();
                                    qDebug()<<"timer"<<timer.mainId<<timer.devId;
                                    TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                                    if(server==nullptr)
                                    {
                                        continue;
                                    }
                                    sendOrder(server, timer.startAddr, timer.mainId, timer.devId, onoff);
                            }
                        }

                    }
                }
            }
        }
    }
}
