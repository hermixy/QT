#include "reciverhandle.h"
#include "analysisthread.h"
#include "../Scada/Src/Math/Math.h"
#include "../Runer/Runer.h"
#include "customprotocol.h"
#include "Runer.h"
#include <QPair>
#include "timersendthread.h"
#include "../Scada/Src/Qchart/mainwidget.h"
ReciverHandle::ReciverHandle(QObject *parent):QThread(parent)
{
    this->moveToThread(this);
    this->setObjectName("ReciverHandle");
    /***************************初始化 ******************************/
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
            TServerData *serverdata=server->fGetServerData();

            if(reciverDataAnylsys.contains(serverdata->fGetIp())) {
                QList<TGraphics *> &commlist=reciverDataAnylsys[serverdata->fGetIp()];
                commlist.append(graphics);
            } else {
                QList<TGraphics *> commlist;
                commlist.append(graphics);
                reciverDataAnylsys.insert(serverdata->fGetIp(), commlist);
            }
        }
    }

}

ReciverHandle::~ReciverHandle()
{

}
void ReciverHandle::stop()
{
    sTop=true;
}

void ReciverHandle::run()
{
    qDebug()<<"start ReciverHandle";
    while(!sTop)
    {
        //qDebug()<<"stop"<<AnalysisThread::waitHandl.size();
        QByteArray data;
        QString ip;
        if(AnalysisThread::waitHandl.size()==0)
        {
            this->msleep(1000);
            //continue;
        }
        if(AnalysisThread::waitHandl.size())
        {
            QMutexLocker locker(&AnalysisThread::mutex);
            reciverData recivdata=AnalysisThread::waitHandl.takeFirst();
            //qDebug()<<"AnalysisThread::waitHandl"<<AnalysisThread::waitHandl.size()<<recivdata.data.toHex();
            ip=recivdata.ip;
            data=recivdata.data;
        }
        if(data.size())
        {
            reciveData(ip, data);
        }
        /******************************************/
    }
    qDebug()<<"stop ReciverHandle";
}
int ReciverHandle::getMainId(quint16 id)
{
    if(id>>8==0x01)
    {
       return  (id-(quint16)0x0100);
    }
    if(id>>8==0x02)
    {
        return  24+(id-(quint16)0x0200);
    }
    if(id>>8==0x03)
    {
        return  48+(id-(quint16)0x0300);
    }
    if(id>>8==0x04)
    {
        return  72+(id-(quint16)0x0400);
    }
    return -1;
}
int ReciverHandle::getThreeMainId(quint16 startAddr,quint16 id)
{

    return (id-(quint16)startAddr);
}

void ReciverHandle::reciveData(QString ip, QByteArray data)
{
    quint8 mainDeviceNum, channelNum, FunctionCode;   //主设备， 通道号， 功能码
    QDataStream dataStream(&data, QIODevice::ReadWrite);

    //qDebug()<<"reciveData"<<data.toHex();
    if((quint8)data[1]==(quint8)0x04)
    {
        //qDebug()<<"0x04"<<data.toHex();
        quint16 crc=(quint16)((data[data[2]+4]<<8)|(quint8)(data[data[2]+3]));

        if(crc==(quint16)TMath::fCrc16((unsigned char *)data.data(), data[2]+3))
        {
            dataStream>>mainDeviceNum>>FunctionCode>>channelNum;
            for(int i=0; i<data[2]/4; i++)
            {
                quint16 addr, value;
                dataStream>>addr>>value;
                //qDebug()<<(mainDeviceNum<16?getMainId(addr):1)<<value;

//                foreach(TScene *sence, Runer::runner->mSceneList)
//                {
                    QList<TGraphics *> list=reciverDataAnylsys.value(ip);
                    //sence->fGetItemsList(list);

                    foreach(TGraphics *graphics,list)
                    {
//                        TService *server= Runer::runner->serIdToServer.value(graphics->fGetServiceId());
//                        if(server==nullptr)
//                        {
//                            continue;
//                        }
//                        TServerData *serverdata=server->fGetServerData();

                        if((graphics->fGetMainId()==mainDeviceNum))//&&(serverdata->fGetIp()==ip))
                        {
                           if(graphics->fGetNodeId()==(mainDeviceNum<16?getMainId(addr):getThreeMainId(graphics->startAddr, addr)))
                           {
                               if((value>>14) >0)
                               {
                                    double setvalue=(value&0x3fff)/100.0;
                                    setvalue=swapShowNum(graphics->showStart, graphics->showStop, setvalue);
                                    graphics->fSetValue(setvalue);
                                    value=setvalue;
                               }else {
                               value=swapShowNum(graphics->showStart, graphics->showStop, value);
                                graphics->fSetValue(value);

                               }
                           }
                        //}
                    }
                }
                if(Runer::runner->curverPie&&Runer::runner->curverPie->isVisible())
                {
                //Cur


                foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
                {
                    TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                    if(server==nullptr)
                    {
                           continue;
                    }
                    TServerData *serverdata=server->fGetServerData();
                    if(serverdata==nullptr)
                    {
                            continue;
                    }
                    if(timer.mainId==mainDeviceNum&&serverdata->fGetIp()==ip)
                    {
                        if(timer.devId==(mainDeviceNum<16?getMainId(addr):getThreeMainId(timer.startAddr, addr)))
                        {
                            timer.value=value;
                        }
                    }
                }
                //Pie
                foreach(TScene::TimerStruct timer, Runer::runner->PieStructList)
                {
                    TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                    if(server==nullptr)
                    {
                           continue;
                    }
                    TServerData *serverdata=server->fGetServerData();
                    if(serverdata==nullptr)
                    {
                            continue;
                    }
                    if(timer.mainId==mainDeviceNum&&serverdata->fGetIp()==ip)
                    {
                        if(timer.devId==(mainDeviceNum<16?getMainId(addr):getThreeMainId(timer.startAddr, addr)))
                        {
                            timer.value=value;
                        }
                    }
                }
                }
            }
        }

    }
    if((quint8)data[1]==(quint8)0x06)
    {
        quint16 addr, value, crc;
        crc=(quint16)((data[7]<<8)|(quint8)data[6]);

        if(crc==(quint16)TMath::fCrc16((unsigned char *)data.data(), 6))
        {
            //qDebug()<<"0x06 "<<data.toHex();
            dataStream>>mainDeviceNum>>FunctionCode>>addr>>value;
            SenderOrder::mutex.lock();
            for(int i=0; i<CustomProtocol::sendOnereciver.size(); i++)
            {
                const SenderCheck send= CustomProtocol::sendOnereciver.at(i);
                if(send.ip==ip&&send.deviceAddr==mainDeviceNum&&send.regeistAddr==(mainDeviceNum<16?getMainId(addr):getThreeMainId(send.startAddr,addr))
                        )
                {
                   // qDebug()<<"remove"<<ip<<mainDeviceNum<<data.toHex();
                    CustomProtocol::sendOnereciver.removeAt(i);
                }

            }
            SenderOrder::mutex.unlock();

        }

    }
    if((quint8)data[1]==(quint8)0x03)
    {
        quint16  crc;
        crc=(quint16)((data[data[2]+4]<<8)|(quint8)(data[data[2]+3]));

        if(crc==(quint16)TMath::fCrc16((unsigned char *)data.data(), data[2]+3))
        {
            //qDebug()<<"0x03"<<data.toHex();
            dataStream>>mainDeviceNum>>FunctionCode>>channelNum;
            for(int i=0; i<data[2]/4; i++)
            {
                quint16 addr, value;
                dataStream>>addr>>value;

//                foreach(TScene *sence, Runer::runner->mSceneList)
//                {
//                    QList<TGraphics *> list;
//                    sence->fGetItemsList(list);
                    QList<TGraphics *> list=reciverDataAnylsys.value(ip);
                    foreach(TGraphics *graphics,list)
                    {
                        //TService *server= Runer::runner->serIdToServer.value(graphics->fGetServiceId());
                        //if(server==nullptr)
                        //{
                           // continue;
                        //}
                       // TServerData *serverdata=server->fGetServerData();

                        if((graphics->fGetMainId())==mainDeviceNum)//&&(serverdata->fGetIp()==ip))
                        {
                           if(graphics->fGetNodeId()==(mainDeviceNum<16?getMainId(addr):getThreeMainId(graphics->startAddr, addr)))
                           {
                               double setvalue=0;
                               if(mainDeviceNum>=16)
                               {
                                   if(graphics->threeDevName=="modbus ascii")
                                   {

                                   setvalue=(value>>12&0x000F)*1000+(value>>8&0x000F)*100+(value>>4&0x000F)*10+(value&0x000F);
                                   setvalue=setvalue/10;
                                   setvalue=swapShowNum(graphics->showStart, graphics->showStop, setvalue);

                                   graphics->fSetValue(setvalue);
                                   value=setvalue;
                                   }
                                   else
                                   {
                                       value=swapShowNum(graphics->showStart, graphics->showStop, value);

                                       graphics->fSetValue(value);
                                   }

                               }
                               else
                               {

                               if((value>>14) >0)
                               {
                                    double setvalue=(value&0x3fff)/100.0;
                                    setvalue=swapShowNum(graphics->showStart, graphics->showStop, setvalue);
                                        graphics->fSetValue(setvalue);
                                    value=setvalue;
                               }else {
                                    value=swapShowNum(graphics->showStart, graphics->showStop, value);
                                        graphics->fSetValue(value);
                               }
                               }


                           }
                       // }
                    }
                }
                if(Runer::runner->curverPie&&Runer::runner->curverPie->isVisible())
                {

                //Cur
                foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
                {
                    TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                    if(server==nullptr)
                    {
                           continue;
                    }
                    TServerData *serverdata=server->fGetServerData();
                    if(serverdata==nullptr)
                    {
                            continue;
                    }
                    if(timer.mainId==mainDeviceNum&&serverdata->fGetIp()==ip)
                    {
                        if(timer.devId==(mainDeviceNum<16?getMainId(addr):getThreeMainId(timer.startAddr, addr)))
                        {
                            timer.value=value;
                        }
                    }
                }
                //Pie
                foreach(TScene::TimerStruct timer, Runer::runner->PieStructList)
                {
                    TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                    if(server==nullptr)
                    {
                           continue;
                    }
                    TServerData *serverdata=server->fGetServerData();
                    if(serverdata==nullptr)
                    {
                            continue;
                    }
                    if(timer.mainId==mainDeviceNum&&serverdata->fGetIp()==ip)
                    {
                        if(timer.devId==(mainDeviceNum<16?getMainId(addr):getThreeMainId(timer.startAddr, addr)))
                        {
                            timer.value=value;
                        }
                    }
                }
                }
            }
        }


    }
    if((quint8)data[1]==(quint8)16)
    {
        quint16 addr, registerNum, crc;
        dataStream>>mainDeviceNum>>FunctionCode>>addr>>registerNum;

        crc=(quint16)((data[7]<<8)|(quint8)(data[6]));
        qDebug()<<crc<<QString::number(data[7])<<(quint8)data[6];

        if(crc==(quint16)TMath::fCrc16((unsigned char *)data.data(), 6))
        {

            qDebug()<<"0x16"<<data.toHex();

            //qDebug()<<mainDeviceNum<<FunctionCode<<(mainDeviceNum<16?getMainId(addr):getThreeMainId(addr))<<registerNum;
        }

    }

}
/*void ReciverHandle::reciveDataThree(QString ip, QByteArray data)
{
    quint8 mainDeviceNum, byteNum, FunctionCode;   //主设备， 通道号， 功能码

    quint8 lrc=CustomProtocol::lrc_124(data.data(), (data.size()-5)/2);

    if(((quint8)(data[data.size()-4])==CustomProtocol::uh_ASCIIh_124(lrc))&&((quint8)(data[data.size()-3])==CustomProtocol::uh_ASCIIl_124(lrc)))
    {

        char *p_124=data.data();
        quint8  q_124=0;

        q_124=q_124+2;
        mainDeviceNum= CustomProtocol::ASCII_uh_124(p_124[q_124-1],p_124[q_124]);

        q_124=q_124+2;
        FunctionCode= CustomProtocol::ASCII_uh_124(p_124[q_124-1],p_124[q_124]);

        q_124=q_124+2;
        byteNum= CustomProtocol::ASCII_uh_124(p_124[q_124-1],p_124[q_124]);

       // qDebug()<<mainDeviceNum<<FunctionCode<<byteNum;
        byteNum=byteNum/2;
        while(byteNum)
        {
            quint16 addr=TimerSendThread::ipToStarr[ip].takeFirst();
            double value;
            byteNum--;
            q_124=q_124+2;
            value = (p_124[q_124-1] -'0')*1000+(p_124[q_124] -'0')*100;
            q_124=q_124+2;
            value = value+(p_124[q_124-1] -'0')*10+(p_124[q_124] -'0');
            value=value/10;
            qDebug()<<value;

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
                    TServerData *serverdata=server->fGetServerData();

                    if((graphics->fGetMainId())==mainDeviceNum&&(serverdata->fGetIp()==ip))
                    {
                       if(graphics->fGetNodeId()==addr)
                       {

                            value=swapShowNum(graphics->showStart, graphics->showStop, value);
                            graphics->fSetValue(value);

                       }

                    }
                }
            }
            //Cur
            foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
            {
                TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                if(server==nullptr)
                {
                       continue;
                }
                TServerData *serverdata=server->fGetServerData();
                if(serverdata==nullptr)
                {
                        continue;
                }
                if(timer.mainId==mainDeviceNum&&serverdata->fGetIp()==ip)
                {
                    if(timer.devId==addr)
                    {
                        timer.value=value;
                    }
                }
            }
            //Pie
            foreach(TScene::TimerStruct timer, Runer::runner->PieStructList)
            {
                TService *server= Runer::runner->serIdToServer.value(timer.serverId);
                if(server==nullptr)
                {
                       continue;
                }
                TServerData *serverdata=server->fGetServerData();
                if(serverdata==nullptr)
                {
                        continue;
                }
                if(timer.mainId==mainDeviceNum&&serverdata->fGetIp()==ip)
                {
                    if(timer.devId==addr)
                    {
                        timer.value=value;
                    }
                }
            }
        }

    }
}*/

double ReciverHandle::swapShowNum(double start, double stop,double value)
{
    double spaceNum=stop-start;
    if(spaceNum<=0) {
        return value;
    }
    double a=value/10.0;
    return start+spaceNum*a;
}
