#include "Pixmap.h"
#include <QDebug>
#include <QDateTime>
#include <ActiveQt/QAxWidget>
#include <QGraphicsProxyWidget>
#include <QtWebView/QtWebView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
TPixmap::TPixmap(double StartX, double StartY, double StopX, double StopY,
                    const Qt::PenStyle &LineStyle, const int LineWidth,
                    const QColor &LineColor, const QColor &BackColor,
                    const QString &FileDir) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Pixmap);

    mFileDir.push_back(FileDir);

    currentPixmap=QPixmap(FileDir);
    isPixmap=true;
    timer=nullptr;
    flash=nullptr;
    w=nullptr;
    vbox=nullptr;
    proxy=nullptr;
    movie=nullptr;


    /*w=new QWidget;
    vbox=new QVBoxLayout(w);
    vbox->setContentsMargins(0, 0, 0, 0);


    proxy=new QGraphicsProxyWidget(this);
    proxy->setAcceptHoverEvents(true);
    proxy->setAcceptDrops(true);
    proxy->setMaximumSize(this->boundingRect().size().toSize()-QSize(10,10));
    proxy->setMinimumSize(this->boundingRect().size().toSize()-QSize(10,10));
    proxy->setWidget(w);

    proxy->setPos(StartX+5, StartY+5);
    proxy->hide();*/

}

TPixmap::~TPixmap()
{}

TItem *TPixmap::fCopy()
{
    TPixmap *Pixmap = new TPixmap();

    Pixmap->fCopy(this);

    return Pixmap;
}

void TPixmap::fCopy(TPixmap *PixmapFrom)
{

    if(NULL == PixmapFrom)
    {
        return;
    }

    TGraphics::fCopy(PixmapFrom);
    this->condiation=PixmapFrom->condiation;
    this->fSetFileDirList(PixmapFrom->mFileDir);
}

void TPixmap::fSetFileDir(const QString FileDir)
{
    //qDebug()<<"set fileDir";
    if(FileDir.isEmpty())
        return;
    if(QThread::currentThread()->objectName()!=QLatin1String("GUI thread"))
    {
        return (void)QMetaObject::invokeMethod(this, "fSetFileDir", Q_ARG(QString, FileDir));
    }
    currentPixmapPath=FileDir;
    if(timer)
        timer->stop();
    if(flash)
    {
        delete flash;
        flash=nullptr;
    }

    //qDebug()<<"set file list"<<FileDir;
    QFileInfo fileinfo(FileDir);

    if(fileinfo.suffix()==tr("swf"))
    {

        if(movie)
            movie->stop();
        if(!w)
            w=new QWidget;
        if(!vbox)
            vbox=new QVBoxLayout(w);
        vbox->setContentsMargins(0, 0, 0, 0);
        if(!proxy)
            proxy=new QGraphicsProxyWidget(this);
        proxy->setAcceptHoverEvents(true);
        proxy->setAcceptDrops(true);
        proxy->setMaximumSize(this->boundingRect().size().toSize()-QSize(10,10));
        proxy->setMinimumSize(this->boundingRect().size().toSize()-QSize(10,10));
        proxy->setWidget(w);

        proxy->setPos(mStartX+5, mStartY+5);
        if(!flash)
            flash=new QAxWidget(0, NULL);
        flash->setControl(QString::fromUtf8("{d27cdb6e-ae6d-11cf-96b8-444553540000}"));
        vbox->addWidget(flash);
        flash->dynamicCall("LoadMovie(long,string)",0, FileDir);
        proxy->show();
        if(!timer)
        {
            timer=new QTimer(this);
            connect(timer, SIGNAL(timeout()), this , SLOT(updateFlash()));
        }
        isPixmap=false;
        timer->start(200);
        return;
    }
    if(fileinfo.suffix()==tr("gif"))
    {
        if(proxy)
            proxy->hide();
        if(timer)
            timer->stop();
        if(!movie)
        {
            movie=new QMovie(this);

            connect(movie, SIGNAL(frameChanged(int)), this, SLOT(updateCurrentPixmap(int)));
        }
        //qDebug()<<"gif 1"<<FileDir;
        isPixmap=true;
        currentPixmap=QPixmap(FileDir);

        movie->stop();
        movie->setCacheMode(QMovie::CacheAll);
        movie->setFileName(FileDir);
        movie->start();
        update();
        return;
    }
    if(proxy)
        proxy->hide();
    if(timer)
        timer->stop();

    isPixmap=true;
    currentPixmap=QPixmap(FileDir);
    update();
}

void TPixmap::fSetFileDirList(QList<QString> fileDirList)
{
    //qDebug()<<"set file dir list"<<fileDirList.size();
    this->mFileDir=fileDirList;
    if(condiation.contains(3))
    {
        fSetFileDir(fileDirList.at(condiation.indexOf(3)));
        return;
    }
    fSetValue(0);
}

QString TPixmap::fGetFileDir() const
{
    return mFileDir.last();
}

void TPixmap::fSetValue(const double Value)
{
    //qDebug()<<"setValue"<<Value;

    TGraphics::fSetValue(Value);

    if(!fGetHint().isEmpty()&&Value==1) {
        if(!isALarm)
        {
        emit mStartAlarm(Value, true, QDateTime::currentDateTime().toMSecsSinceEpoch());
        isALarm = true;
        }
    }
    else
    {
        isALarm=false;
    }
    if(mFileDir.size()==1)
    {
        return;
    }
    if(condiation.contains(3))
    {
        QString path=mFileDir.at(condiation.indexOf(3));
        if(currentPixmapPath!=path)
            fSetFileDir(path);
        return;
    }
    if(condiation.contains(0)&&Value==0)
    {
        QString path=mFileDir.at(condiation.indexOf(0));
        if(currentPixmapPath!=path)
            fSetFileDir(path);
        return;
    }

    if(condiation.contains(1)&&Value==1)
    {
        QString path=mFileDir.at(condiation.indexOf(1));
        if(currentPixmapPath!=path)
            fSetFileDir(path);
        return;
    }


//    foreach (QString path, mFileDir) {

//        int i=mFileDir.indexOf(path);
//        if(condiation.at(i)==3)
//        {
//            if(currentPixmapPath!=path)
//                fSetFileDir(path);
//            return;
//        }
//        if(condiation.at(i)==0&&Value==0)
//        {
//            if(currentPixmapPath!=path)
//                fSetFileDir(path);
//            return;
//        }

//        if(condiation.at(i)==1&&Value==1)
//        {
//            if(currentPixmapPath!=path)
//                fSetFileDir(path);
//            return;
//        }
//    }
}

void TPixmap::fDraw(QPainter *painter)
{
#ifndef RUNNER
    if(flash)
    {
    proxy->setMaximumSize(this->boundingRect().size().toSize()-QSize(10,10));
    proxy->setMinimumSize(this->boundingRect().size().toSize()-QSize(10,10));
    proxy->setPos(mStartX+5, mStartY+5);
    if(proxy->isActive())
    {
        if(!timer->isActive())
            timer->start(300);
    }
    else
    {
        timer->stop();
    }
    }

#else
    if(flash)
    {
        proxy->setMaximumSize(this->boundingRect().size().toSize());
        proxy->setMinimumSize(this->boundingRect().size().toSize());
        proxy->setPos(mStartX, mStartY);
        if(proxy->isActive())
        {
            if(!timer->isActive()&&!isPixmap)
                timer->start(300);
        }
        else
        {
            timer->stop();
        }
    }

#endif
    if(setValue==0)
    {
        if(condiation.contains(2))
        {
            if(currentPixmapPath!=mFileDir.at(condiation.indexOf(2)))
                fSetFileDir(mFileDir.at(condiation.indexOf(2)));
        }
    }
    if(movie&&isPixmap)
    {
        if(this->isActive())
        {
                //qDebug()<<"movie start"<<movie->state();
                movie->start();
        }
    }

    if(isPixmap)
    {
        painter->drawPixmap(mStartX, mStartY, mStopX - mStartX, mStopY - mStartY, currentPixmap);
    }

}
void TPixmap::updateCurrentPixmap(int pixnum)
{
    Q_UNUSED(pixnum);

    if(movie)
    {
       currentPixmap=movie->currentPixmap();
        if(!this->isActive())
        {
            movie->stop();
        }
    }
    if(scene())
      scene()->update();
    //update();
}
void TPixmap::updateFlash()
{
    if(flash)
    {
        if(!proxy)
            return;
        if(proxy->isActive())
            update();
        else
        {
            //qDebug()<<"timer stop";
            timer->stop();
        }
    }
}
