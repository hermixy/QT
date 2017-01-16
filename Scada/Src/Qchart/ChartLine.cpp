
#include <QWebElement>
#include <QWebFrame>

#include "ChartLine.h"
//#include "mainwindow.h"

EChartTest::EChartTest(QObject* parent)
    : QObject(parent)
{
    //QTimer *testTimer = new QTimer(this);
    //testTimer->start(60000);
    //connect( testTimer,SIGNAL(timeout()), this, SLOT(setXdata()) );
    //connect( testTimer,SIGNAL(timeout()), this, SLOT(setYdata()) );
    //connect( testTimer,SIGNAL(timeout()), this, SLOT(setTitle()) );
    //connect( testTimer,SIGNAL(timeout()), this, SLOT(setSubTitle()));
}

EChartTest::~EChartTest()
{
}

void EChartTest::setXdata(QString datatime)
//void EChartTest::setXdata()
{
    QDateTime currentTime;
    if(Xdata.size()<10)
    {
        mutexX.lock();
        //Xdata.append(currentTime.currentDateTime().toString());
        Xdata.append(datatime);
        mutexX.unlock();
    }
    else
    {
         mutexX.lock();
         Xdata.append(currentTime.currentDateTime().toString());
         //Xdata.append(datatime);
         Xdata.removeFirst();
         mutexX.unlock();
    }
    //qDebug()<<"setXdata_Xdata.size"<<Xdata.size();
}
QStringList EChartTest::getXdata()
{
    mutexX.lock();
    //qDebug()<<"GetXdata_Xdata.size"<<Xdata.size();
    mutexX.unlock();
    return Xdata;

}

void EChartTest::setYdata(QString value)
//void EChartTest::setYdata()
{
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int data = qrand()%39;
    //qDebug()<<"rand"<<data;

    if(Ydata.size()<10)
    {
        mutexY.lock();
        //Ydata.append(QString::number(data,10));
        Ydata.append(value);

        mutexY.unlock();
    }
     else
    {
        mutexY.lock();
        //Ydata.append(QString::number(data,10));
        Ydata.append(value);
        Ydata.removeFirst();
        mutexY.unlock();
    }
    //qDebug()<<"setYdata_Ydata.size()"<<Ydata.size();
}
QStringList EChartTest::getYdata()
{
     mutexY.lock();
     //qDebug()<<"GetYdata_Ydata.size"<<Ydata.size();
     mutexY.unlock();
     return Ydata;
}

QString EChartTest::getTitle()
{
    return Title;
}
void EChartTest::setTitle(QString title)
//void EChartTest::setTitle()
{
    mutexTitle.lock();
    Title = title;
    //Title =QString("'Title'");
    mutexTitle.unlock();
}

QString EChartTest::getSubTitle()
{
    return SubTitle;
}
void EChartTest::setSubTitle(QString subtitle)
//void EChartTest::setSubTitle()
{
    mutexSubTitle.lock();
    SubTitle = subtitle;
    //SubTitle =QString("SubTitle");
    mutexSubTitle.unlock();
}
