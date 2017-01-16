#include "Runer.h"

#include "../Scada/Dialog/Data/DialogData.h"
#include "../Scada/Dialog/Login/DialogLogin.h"
#include "../Scada/Dialog/Alarm/DialogAlarm.h"
#include "../Scada/Dialog/Timer/dialogtimer.h"
#include "../Scada/Dialog/Report/DialogReport.h"
#include "../Scada/Dialog/Timer/showhistory.h"
#include "../Scada/Src/Qchart/mainwidget.h"
#include "../Scada/Dialog/Timer/onlinestatus.h"

#include "../Scada/Src/View/View.h"
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/Js/Js.h"

#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QLayout>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextBrowser>
#include <QDebug>
#include <QDesktopWidget>
#include <QInputDialog>
#include "customprotocol.h"
#include <QTableWidget>
#include "../Scada/Src/Qt-ExcelEngine/excelengine.h"
#include "../Scada/Dialog/Alarm/alarmdialog.h"
Runer * Runer::runner=nullptr;

Runer::Runer(QWidget *parent, bool isAdmin) : QMainWindow(parent), isAdmins(isAdmin)
{

    /*TMySQL *mysql=new TMySQL();
    mysql->fInit("localhost", "mysql", "root", "123456");
    mysql->qsqlExec("create table student(id double, value double)");
   mysql->qsqlExec("insert into student values(50, 50)");
*/
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    alarmdialog=nullptr;
    qRegisterMetaType<TService>("TService");
    fInit();
    //qDebug()<<qApp->applicationDirPath();

    //qDebug() << "Runer::Runer(QWidget *parent)" << QThread::currentThread();

    // 构造主界面
    QWidget *mWidget = new QWidget(this);
    mWidget->setObjectName(tr("mWidget"));
    setCentralWidget(mWidget);

    sound=new QSound(tr("alarm.wav"), this);
    sound->setLoops(10);

    // 构建界面
    mView = new TView();
    mView->setObjectName(tr("mView"));

#ifndef DEF_RUN_AS_CLIENT
    setWindowTitle(tr("SCADA系统"));
#else
    setWindowTitle(tr("建筑节能系统V3.0"));
#endif

    // 禁用最大化 最小化 关闭按钮
    //setWindowFlags(windowFlags() & ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));

    fCreateMenu();

    QVBoxLayout *mLayoutMain = new QVBoxLayout(mWidget);
    mLayoutMain->setContentsMargins(0, 0, 0, 0);
    mLayoutMain->setSpacing(0);
    textBrowser=new QTextBrowser(this);
    textBrowser->setMaximumHeight(50);
    textBrowser->setAlignment(Qt::AlignCenter);
    textBrowser->setFont(QFont("微软雅黑", 12));
    textBrowser->setTextColor(Qt::red);

    mLayoutMain->addWidget(mView);

    mLayoutMain->addWidget(textBrowser);

    //lableTime=new QLabel(mWidget);

    //lableTime->setGeometry(QRect(0, 0, 200, 20));

    // 打开关键文件    
    qDebug() << QApplication::applicationDirPath() + tr("/main.xml");

    if(!mXml.fOpen(QApplication::applicationDirPath() + tr("/main.xml")))
    {    
        qDebug()<<"fdsg";
        QMessageBox::warning(this, tr("初始化"), tr("解析关键文件发生错误"));
        return;
    }
    runner=this;

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(insertDatatoDB()));
    timer->start(2000);

    // 解析数据库信息
    mXml.fAnalyDB(mDBMsg);
    mXml.fAnalyCurver(this->CurverStructList);
    mXml.fAnalyPie(this->PieStructList);
    fDB(mDBMsg);        //创建数据库

    fLoadFromFile();
    if(!mServiceList.isEmpty())
    {
        foreach (TService *server, mServiceList) {
            //qDebug()<<server->fGetId();
            serIdToServer.insert(server->fGetId(), server);
        }
    }

    // 绑定页面
    if(!mSceneList.isEmpty())
    {
        mView->setScene(mSceneList.first());
    }

    foreach(TScene *Scene, mSceneList)
    {
        QList<TItemEx *>ItemExList;
        Scene->fGetItemsList(ItemExList);

        foreach(TItemEx *ItemEx, ItemExList)
        {
            // 设定可操作画布
            ItemEx->fSetHostScene(Scene);
            connect(ItemEx, &TItemEx::sendData, this, &Runer::handleData);
        }

        QList<TGraphics *>GraphicsList;
        Scene->fGetItemsList(GraphicsList);

        foreach(TGraphics *Graphics, GraphicsList)
        {
            if(NULL == Graphics)
            {
                continue;
            }
           if(groupToGraphics.contains(Graphics->fGetGroupId()))
           {
               groupToGraphics.value(Graphics->fGetGroupId())->append(Graphics);
           }
           else
           {
               if(Graphics->fGetGroupId()!=0) {
               QList<TGraphics *> *list=new QList<TGraphics *>;
               list->append(Graphics);
               groupToGraphics.insert(Graphics->fGetGroupId(), list);
               }
           }

            Graphics->fSetDragAble(false);
            Graphics->fSetSelectAble(false);
            Graphics->fSetMoveAble(false);
            Graphics->setAcceptHoverEvents(true);

        }

        QList<TWidgets *>WidgetsList;
        Scene->fGetItemsList(WidgetsList);

        foreach(TWidgets *Widgets, WidgetsList)
        {
            if(Widgets)
            {
                Widgets->fSetDragAble(false);
                Widgets->fSetSelectAble(false);
                Widgets->fSetMoveAble(false);
                Widgets->setAcceptHoverEvents(true);

            }
        }
    }

    // 初始化联动信息(应在根据数据库更新页面之前)
    //fInitLinkage(mLinkageList);

    //qDebug() << "mDataBase->fDoSqlResult";

    if(mDataBase&&mDataBase->fIsOpen())
    {
        //ResultData = mDataBase->fDoSqlResult(tr("SELECT service_id,node_id,val FROM data1"));
        TMySQL * mysql=static_cast<TMySQL *>(mDataBase);
        if(mysql)
        {
        foreach(int id, groupToGraphics.keys())
        {
            QList<TGraphics *> * list=groupToGraphics.value(id);
            QString sql=tr("create table if not exists %1%2(").arg("mysql").arg(id);

            for(int i=0; i<list->size()-1; i++)
            {
                sql+="value"+QString::number(i)+" double, ";
            }
            sql+="date bigint)";
            //mysql->qsqlExec(QString("drop table id exists%1").arg(id));
           // qDebug()<<sql<<list->size();
            if(!mysql->qsqlExec(sql))                    //插入数据表
            {
                mysql->qsqlExec(QString("drop table mysql%1").arg(id));
                mysql->qsqlExec(sql);
                qDebug()<<"插入数据表失败";
            }
        }
        }
    }

    //qDebug() << "mDataBase->fDoSqlResult finish";

    // 解析完成,添加动作，刷新页面，绑定信号和槽
    foreach(TScene *Scene, mSceneList)
    {
        QList<TGraphics *>ItemList;
        Scene->fGetItemsList(ItemList);

        //qDebug() << "flush scene" << Scene->fGetName();

        foreach(TGraphics *Graphics, ItemList)
        {
            if(NULL == Graphics)
            {
                continue;
            }

            connect(Graphics, SIGNAL(mGotoLink(int)), this, SLOT(fGotoLink(int)));
            connect(Graphics, SIGNAL(mStartAlarm(double, bool, qint64)), this, SLOT(fGetAlarm(double,bool ,qint64)));
            connect(Graphics, SIGNAL(sendContral(TButton::Control)), this, SLOT(reciverContral(TButton::Control)));

        }

        connect(Scene, SIGNAL(mClickItem(TGraphics*)), this, SLOT(fClick(TGraphics*)));

        QList<TButton *>buttonItemList;
        Scene->fGetItemsList(buttonItemList);
        foreach(TButton *button, buttonItemList)
        {
            if(NULL == button)
            {
                continue;
            }
            connect(button, SIGNAL(mGotoLink(int)), this, SLOT(fGotoLink(int)));   //界面跳转
            connect(button, SIGNAL(sendContral(TButton::Control)), this, SLOT(reciverContral(TButton::Control)));
        }
    }

    qDebug() << "fInitService(); in Runner";
    #ifndef DEF_RUN_AS_CLIENT
    fInitService();
    #else

    #endif
    //qDebug()<<groupToGraphics.value(1)->at(0)->fGetGroupId();
    onlinestatus=new OnlineStatus(mServiceList);
    /********************************************/
    fupdateFromInitTImer(this->sceneToTImer);
    curverPie=nullptr;
    /*curverPie=new MainWidget();  //曲线init
    curverPie->setWindowTitle(tr("实时图表"));
    foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
    {
       curverPie->addLineInterface(timer.TimerId);
    }
    curverPie->addPieInterface("pie");*/

    /*******************开启发送县城 ********************/
    senderOrder= new SenderOrder();  //初始化发送线程
    connect(this, &Runer::sendData, senderOrder, &SenderOrder::sendOrder);

    thread=new AnalysisThread();
    thread->start();
    reciverThread=new ReciverHandle();
    reciverThread->start();

    timerSend=new TimerSendThread();

    timerSend->start();
}

int Runer::excel(QTableView *tableView)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),

                                                       tr("Excel Files (*.xls)"));
     if (fileName.isEmpty())
           return -1;
      QString sheetName;

      sheetName="mydatas";
      qDebug() <<"fileName"<<fileName;

      QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "excelexport");
        if(!db.isValid())
        {
            qDebug() << "export2Excel failed: QODBC not supported.";
            return -2;
        }
        // set the dsn string
        QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";DBQ=%2").

                      arg(fileName).arg(fileName);

        db.setDatabaseName(dsn);
        if(!db.open())
        {
            qDebug() << "export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
            QSqlDatabase::removeDatabase("excelexport");
            return -3;
        }
        QSqlQuery query(db);

        //drop the table if it's already exists

        QString  sSql = QString("DROP TABLE [%1] ").arg(sheetName);

        query.exec(sSql);



        //create the table (sheet in Excel file)

        sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
        int i=0;
        for(i=0; i<tableView->model()->columnCount()-1; i++)
        {
            sSql+=tr("[%1] varchar(50), ").arg(tableView->model()->headerData(i, Qt::Horizontal).toString());
        }
        sSql+=tr("[%1] varchar(100))").arg(tableView->model()->headerData(i, Qt::Horizontal).toString());

        qDebug()<<"sSql"<<sSql;
        query.prepare(sSql);
        if(! query.exec())
        {

            qDebug() << "export2Excel failed: Create Excel sheet failed.";
            db.close();
            QSqlDatabase::removeDatabase("excelexport");
            return -4;
        }

        for(int i=0; i<tableView->model()->rowCount(); i++)
        {
            sSql =QString("INSERT INTO [%1] VALUES(").arg(sheetName);
            int j=0;
            for(j=0; j<tableView->model()->columnCount()-1; j++)
            {

                QString name=tableView->model()->index(i, j).data().toString();
                sSql+=tr("%1, ").arg(name);
            }
            QString name=tableView->model()->index(i, j).data().toString();
            //qDebug()<<name.toLongLong();
            name=QDateTime::fromMSecsSinceEpoch(name.toLongLong()).toString(Qt::ISODate);
            sSql+=tr("'%1')").arg(name);
           // qDebug()<<sSql;;
            if(!query.exec(sSql))
            {
                qDebug()<<"插入数据失败";
            }
        }
        QThread::msleep(100);
        return 1;
}

TAction* Runer::fCreateAction(const QString Text)
{
    TAction *Action = new TAction(Text, this);

    return Action;
}

void Runer::fCreateMenu()
{
    mMenuBar = new QMenuBar(this);
    mMenuBar->setObjectName(QStringLiteral("mMenuBar"));
    //mMenuBar->setGeometry(QRect(0, 0, 1366, 23));
    this->setMenuBar(mMenuBar);

    mUserMenu = mMenuBar->addMenu(tr("用户(&U)"));
    mActionLogin = fCreateAction(tr("注册新用户(&L)..."));
    connect(mActionLogin, SIGNAL(triggered()), this, SLOT(fLogin()));
    mUserMenu->addAction(mActionLogin);
    mActionUser = fCreateAction(tr("软件授权(&M)..."));
    connect(mActionUser, SIGNAL(triggered()), this, SLOT(fAccredit()));
    mUserMenu->addAction(mActionUser);
    mActionPermission = fCreateAction(tr("启用(&P)..."));
    mUserMenu->addAction(mActionPermission);
    connect(mActionPermission, SIGNAL(triggered()), this, SLOT(fStart()));
    //mActionRole = fCreateAction(tr("角色管理(&R)..."));
    //mUserMenu->addAction(mActionRole);

    mDataMenu = mMenuBar->addMenu(tr("数据(&D)"));
    mActionDataLately = fCreateAction(tr("查看历史数据(&L)..."));
    connect(mActionDataLately, SIGNAL(triggered()), this, SLOT(fLatelyData()));
    mDataMenu->addAction(mActionDataLately);
    //mActionDataHistory = fCreateAction(tr("查看系统日志(&H)..."));
    //connect(mActionDataHistory, SIGNAL(triggered()), this, SLOT(fHistoryData()));
    //mDataMenu->addAction(mActionDataHistory);
    mActionDataRealCurve = fCreateAction(tr("查看警告日志(&R)..."));
    connect(mActionDataRealCurve, SIGNAL(triggered()), this, SLOT(fRealCurve()));
    mDataMenu->addAction(mActionDataRealCurve);
    mActionDataHistoryCurve = fCreateAction(tr("查看系统日志(&D)..."));
    connect(mActionDataHistoryCurve, SIGNAL(triggered()), this, SLOT(fHistoryCurve()));
    mDataMenu->addAction(mActionDataHistoryCurve);

    mAlarmMenu = mMenuBar->addMenu(tr("在线状态(&A)"));
    mActionAlarm = fCreateAction(tr("在线状态(&M)"));
    connect(mActionAlarm, SIGNAL(triggered()), this, SLOT(fAlarm()));
    mAlarmMenu->addAction(mActionAlarm);
    //mActionIgnoreAlarm = fCreateAction(tr("处理所有(&C)"));
    //mAlarmMenu->addAction(mActionIgnoreAlarm);

    mReportMenu = mMenuBar->addMenu(tr("报表(&R)"));
    mActionReportCustom = fCreateAction(tr("生成报表(&C)..."));
    mReportMenu->addAction(mActionReportCustom);
    connect(mActionReportCustom, SIGNAL(triggered()), this, SLOT(fReportCustom()));
    mActionReportAuto = fCreateAction(tr("曲线(&A)"));
    mReportMenu->addAction(mActionReportAuto);
    connect(mActionReportAuto, SIGNAL(triggered()), this, SLOT(fCurve()));

    mSystemMenu = mMenuBar->addMenu(tr("系统(&S)"));
    mActionRefresh = fCreateAction(tr("刷新(&F)"));
    mActionReStart = fCreateAction(tr("重启(&R)"));
    mActionExit = fCreateAction(tr("退出(&E)"));
    mSystemMenu->addAction(mActionRefresh);
    mSystemMenu->addAction(mActionReStart);
    mSystemMenu->addAction(mActionExit);
    connect(mActionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(mActionRefresh, SIGNAL(triggered()), this, SLOT(updateScense()));

    mAboutMenu = mMenuBar->addMenu(tr("关于(&H)"));
    mActionAbout = fCreateAction(tr("关于(&A)"));
    connect(mActionAbout, SIGNAL(triggered()), this, SLOT(Abort()));
    mAboutMenu->addAction(mActionAbout);
}

void Runer::fInit()
{
    mDataBase = NULL;
    //mClinetServer = NULL;
    //mClient = NULL;
    ThreadClient = NULL;

    mMenuBar = NULL;
    mUserMenu = NULL;
    mActionLogin = NULL;
    mActionUser = NULL;
    mActionPermission = NULL;
    mActionRole = NULL;

    mDataMenu = NULL;
    mActionDataLately = NULL;
    mActionDataHistory = NULL;
    mActionDataRealCurve = NULL;
    mActionDataHistoryCurve = NULL;

    mAlarmMenu = NULL;
    mActionAlarm = NULL;
    mActionIgnoreAlarm = NULL;

    mReportMenu = NULL;
    mActionReportCustom = NULL;
    mActionReportAuto = NULL;

    mSystemMenu = NULL;
    mActionRefresh = NULL;
    mActionReStart = NULL;
    mActionExit = NULL;

    mAboutMenu = NULL;
    mActionAbout = NULL;

    //mThreadClientServer = NULL;
    mView = NULL;

    mUserId = 1;
    mDBMsg = new TDataBaseMsg();
    mDBMsg->fIsAvailable();
    //mRunMode = new TRunMode();
    mHint = new DialogHint(this);

}

bool Runer::fDB(TDataBaseMsg *DBMsg)   //创建数据库
{
    mDataBase = TDataBaseService::fCreateDataBase(DBMsg);

    if(mDataBase)
    {
        mDataBase->fInit(DBMsg->fGetHost(),
                           DBMsg->fGetDBName(),
                           DBMsg->fGetUserName(),
                           DBMsg->fGetPassWord());
    }

    return true;
}

void Runer::fAddGraphics(int ServiceId, int NodeId,  TGraphics *Graphics)
{
    qDebug() << "fAddGraphics" << NodeId;

    if(Graphics)
    {
        QMap<int, QList<TGraphics*> > &NodeList = mGraphicsMap[ServiceId];
        QList<TGraphics *> &GraphicsList = NodeList[NodeId];

        GraphicsList.push_back(Graphics);
    }
}

void Runer::fInitService()
{
    qDebug() << "Runer::fInitService() at" << QThread::currentThread();

    foreach(TService *Service, mServiceList)
    {
        qDebug() << "Services:" << Service->fGetName();

        TThreadServer *ThreadServer = NULL;
        TThreadConnection *ThreadConnection = NULL;

        // 创建线程以及协议、连接
        if(Service->fIsServer())
        {
            ThreadServer = new TThreadServer(Service, this);

            if(ThreadServer)
            {
                ThreadServer->fInit();
            }
        }
        else
        {
            ThreadConnection = new TThreadConnection(Service, this);

            //qDebug() << "ThreadConnection" << ThreadConnection;

            if(ThreadConnection)
            {
                ThreadConnection->fInit();
            }
        }

        TProtocol* Protocol = Service->fGetProtocol();

        //qDebug() << "Runer::fInitService() Protocol = " << Protocol;

        if(Protocol)
        {
            // 绑定至数据库操作
            connect(Protocol, SIGNAL(mValueCome(int,int,double,quint64)),
                    this, SLOT(fDataCom(int,int,double,quint64)));

            // 绑定至页面变化
            foreach (TScene *Scene, mSceneList)
            {
                QList<TGraphics *>ItemList;
                Scene->fGetItemsList(ItemList);

                foreach(TGraphics *Graphics, ItemList)
                {
                    if(Protocol->fGetServiceId() == Graphics->fGetServiceId()
                            && Graphics->fGetServiceId() > 0
                            && Graphics->fGetNodeId() > 0)
                    {
                        // 协议解析到的数据，发送给图形
                        /*connect(Protocol, SIGNAL(mDataCome(int,double,quint64)),
                                Graphics, SLOT(fRecvData(int,double,quint64)));

                        // 图形将数据乘以一个系数，发送回协议
                        connect(Graphics, SIGNAL(mValueChanged(int,int,double,quint64,double)),
                                Protocol, SLOT(fValueCome(int,int,double,quint64,double)));
                        */

                        fAddGraphics(Graphics->fGetServiceId(), Graphics->fGetNodeId(), Graphics);

                        // 绑定至数据库操作
                        /*connect(Graphics, SIGNAL(mValueCome(int,int,double,quint64)),
                                this, SLOT(fDoSql(int,int,double,quint64)));
                        */
                    }
                }

                // 绑定至控件操作
                QList<TWidgets *>WidgetsList;
                Scene->fGetItemsList(WidgetsList);

                foreach(TWidgets *Widgets, WidgetsList)
                {
                    if(Protocol->fGetServiceId() == Widgets->fGetServiceId()
                            && Widgets->fGetServiceId() > 0)
                    {
                        qDebug() << "SendTo connect " << (void *)Widgets << "to" << (void *)Protocol;
                        connect(Widgets, SIGNAL(mSendTo(QObject*,int,double,int,QString)), Protocol, SLOT(fSendTo(QObject*,int,double,int,QString)));
                    }
                }
            }
        }// end of if(Protocol)

        // 启动线程，并保存至列表
        if(ThreadServer)
        {
            ThreadServer->fStart();

            mThreadServerList.push_back(ThreadServer);
        }

        if(ThreadConnection)
        {
            ThreadConnection->fStart();

            mThreadConnectionList.push_back(ThreadConnection);
        }
    }

    //QTimer::singleShot(5000, mSceneList.first(), SLOT(update()));
}

bool Runer::fLoadFromFile()
{
    qDebug() << "Runer::fLoadFromFile()";

    // 解析Service
    mXml.fAnalyService(mServiceList);

    //解析界面图形
    return mXml.fAnalyScene(mSceneList);
}

bool Runer::fUpdateDB()
{
    TDataBaseService::fClear(mDataBase);
    TDataBaseService::fSaveService(mDataBase, mServiceList);
    TDataBaseService::fSaveScene(mDataBase, mSceneList);
    //TDataBaseService::fSaveLinkage(mDataBase, mLinkageList);

    return true;
}

bool Runer::fLoadFromDataBase()
{
    TDataBaseService::fLoadService(mDataBase, mServiceList);
    TDataBaseService::fLoadScene(mDataBase, mSceneList);
    //TDataBaseService::fLoadLinkage(mDataBase, mLinkageList);

    return true;
}

void Runer::fInitLinkage(const QList<TLinkage *> &LinkageList)
{
    foreach(TLinkage *Linkage, LinkageList)
    {
        TGraphics *SrcGraphics = NULL;
        TGraphics *DstGraphics = NULL;

        //qDebug() << "fInitLinkage" << Linkage->mSrcServiceId << Linkage->mSrcNodeId << Linkage->mDstServiceId << Linkage->mDstNodeId;

        foreach(TScene *Scene, mSceneList)
        {
            QList<TGraphics *>GraphicsList;
            Scene->fGetItemsList(GraphicsList);

            foreach(TGraphics *Graphics, GraphicsList)
            {
                if(Graphics->fGetServiceId() == Linkage->mSrcServiceId
                        && Graphics->fGetNodeId() == Linkage->mSrcNodeId)
                {
                    SrcGraphics = Graphics;
                }

                if(Graphics->fGetServiceId() == Linkage->mDstServiceId
                        && Graphics->fGetNodeId() == Linkage->mDstNodeId)
                {
                    DstGraphics = Graphics;
                }

                if(SrcGraphics && DstGraphics)
                {
                    break;
                }
            }

            if(SrcGraphics && DstGraphics)
            {
                break;
            }
        }

        if(SrcGraphics && DstGraphics)
        {
            //qDebug() << "connect SrcGraphics" << SrcGraphics->fGetNodeID() << "DstGraphics" << DstGraphics->fGetNodeID();
            connect(SrcGraphics, SIGNAL(mSendLinkage(bool,int)), DstGraphics, SLOT(fRecvLinkage(bool,int)));
        }
    }
}

Runer::~Runer()
{
}

void Runer::fDataCom(int ServiceId, int NodeId, double Value, quint64 Time)
{
    //qDebug() << "Runer::fDataCom" << ServiceId << NodeId << Value;

    if(mGraphicsMap.count(ServiceId) > 0)
    {
        QMap<int, QList<TGraphics*> > &NodeList = mGraphicsMap[ServiceId];

        if(NodeList.count(NodeId) > 0)
        {
            QList<TGraphics *> &GraphicsList = NodeList[NodeId];

            foreach(TGraphics *Graphics, GraphicsList)
            {
                //qDebug() << "Graphics->fUpdateValue(Value, Time)";
                Graphics->fUpdateValue(Value, Time);
            }
        }
    }

// 客户端模式，不更新数据库
#ifndef DEF_RUN_AS_CLIENT
    if(mDataBase)
    {
        /*mDataBase->fDoSql(tr("UPDATE data1 SET val = ") + QString::number(Value)
                           + tr(" WHERE service_id = ") + QString::number(ServiceId)
                           + tr(" AND node_id = ") + QString::number(NodeId));*/

        mDataBase->fDoSql(tr("UPDATE data1 SET val = %1, time = %2 WHERE service_id = %3 AND node_id = %4")
                            .arg(Value)
                            .arg(Time)
                            .arg(ServiceId)
                            .arg(NodeId)
                            );

        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                              .arg(ServiceId)
                              .arg(NodeId)
                              .arg(Value)
                              .arg(Time)
                              );
    }
#endif
}

void Runer::fGotoLink(int LinkId)
{
    TButton *button = qobject_cast<TButton *>(sender());
    if(!button)
    {
        return;
    }
    //qDebug()<<button->isShowTimer;
    if(button->isShowTimer) {
        DialogTimer *Alarm;
        TScene *myscene=qobject_cast<TScene *>(mView->scene());
        if(senceToTImer.contains(myscene))
        {
            Alarm=senceToTImer.value(myscene);
        }
        else
        {
            Alarm = new DialogTimer(myscene, this);
            senceToTImer.insert(myscene, Alarm);
            connect(Alarm, SIGNAL(saveDateTimer()), this, SLOT(saveDataTimer()));
        }

        Alarm->show();

        return;
    }
    foreach(TScene *Scene, mSceneList)
    {
        //qDebug() << "LinkId" << LinkId << Scene->mId;


        if(LinkId == Scene->fGetId())
        {
            if(button->width==0||button->heigth==0)
            {
                mView->setScene(Scene);
                return;
            }
            if((QApplication::desktop()->width()==button->width)&&(QApplication::desktop()->height()==button->heigth))
            {
                //QGraphi *mView
                 mView->setScene(Scene);
            }
            else
            {
                TView *mView=new TView(0);
                mView->setAttribute(Qt::WA_DeleteOnClose);
                mView->setScene(Scene);
                double width=button->width;
                double height=button->heigth;
                mView->scale(width/QApplication::desktop()->width(), height/QApplication::desktop()->height());
                mView->setMinimumSize(button->width, button->heigth);
                mView->show();
            }
        }
    }
}

void Runer::fGetAlarm(double Value, bool upordown, qint64 Time)
{
    Q_UNUSED(upordown)
    TGraphics *Graphics = qobject_cast<TGraphics *>(sender());

    //qDebug() << "fGetAlarm" << (void *)Graphics;
    if(Graphics)
    {
        qDebug() << "fGetAlarm" << (void *)Graphics;
        QString dateTime=QDateTime::fromMSecsSinceEpoch(Time).toString();

        this->textBrowser->append(Graphics->fGetHint()+":"+Value+ "  "+dateTime);
        QTextStream warout;    //警告


        QFile file("waringlog.txt");
        if (file.size() > 1000000)
        {
            QFileInfo fileInfo("waringlog.txt");
            QDir dir (fileInfo.absoluteDir());

            if (dir.remove("waringlog.txt"))
                qDebug() << "Removed old log successfully";
            else
                qWarning() << "Unable to remove old log file";
        }
        file.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text);
        warout.setDevice(&file);

        warout<<Graphics->fGetHint()+"::"+Value+ "::"+dateTime+"\r\n";

        sound->play();
        if(!alarmdialog)
            alarmdialog=new AlarmDialog(this);
        alarmdialog->show();
        alarmdialog->activateWindow();
        alarmdialog->setTitle(Graphics->fGetHint()+"::"+Value);
    }

}

void Runer::fAlarm()
{
    if(onlinestatus)
        onlinestatus->show();
}

void Runer::fLogin()
{
    DialogLogin *Login = new DialogLogin(false, 0);


    Login->exec();

    delete Login;
}
void Runer::fAccredit()
{
    QString regist=QInputDialog::getText(this, tr("软件授权"), tr("请输入注册码"));
    if(!regist.isEmpty())
    {
        if(regist==CustomProtocol::getMd5Hash(CustomProtocol::get_localmachine_mac()))
        {
            QSettings setting("inis", QSettings::IniFormat);
            setting.setIniCodec("UTF-8");
            setting.setValue("time", 0);
            setting.setValue("registNum", setting.value("registNum", 0).toInt()+1);
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("注册码不正确"));

        }
    }
}
void Runer::fStart()
{
     QSettings setting("inis", QSettings::IniFormat);
     setting.setIniCodec("UTF-8");
     setting.setValue("start", true);
}

void Runer::fCreateUser()
{
    DialogLogin *Login = new DialogLogin(this);

    Login->exec();

    delete Login;
}

void Runer::fClose()
{
    qDebug() << "Runer::fClose()";

    fSaveTimerToInit(sceneToTImer);
    if(curverPie)
        delete curverPie;
    if(senderOrder)
    {
        delete senderOrder;
    }
    thread->quit();
    thread->wait();

    reciverThread->stop();
    reciverThread->exit(0);
    reciverThread->wait();

    timerSend->stop();
    timerSend->exit(0);
    timerSend->wait();

    delete thread;
    delete reciverThread;
    delete timerSend;
    if(ThreadClient)
    {
        ThreadClient->fStop();

        ThreadClient->deleteLater();
    }

    // stop
    foreach(TThreadConnection *ThreadConnection, mThreadConnectionList)
    {
        qDebug() << "ThreadConnection->isRunning()" << ThreadConnection->isRunning();
        if(ThreadConnection->isRunning())
        {
            ThreadConnection->fStop();
        }

        qDebug() << "mThreadConnectionList remove one";

        mThreadConnectionList.removeOne(ThreadConnection);
        ThreadConnection->deleteLater();
    }

    foreach(TThreadServer *ThreadServer, mThreadServerList)
    {
        if(ThreadServer->isRunning())
        {
            ThreadServer->fStop();
        }
    }



    // db
    if(mDataBase)
    {
        mDataBase->fClose();
        delete mDataBase;
        mDataBase = NULL;
    }
}

void Runer::closeEvent(QCloseEvent *event)
{
    qDebug() << "Runer::closeEvent()";

    if(0 == QMessageBox::information(this, tr("提示"), tr("是否退出本系统"), tr("是"), tr("否")))
    {
        fClose();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void Runer::fLatelyData()
{
    /*DialogData *mData = new DialogData(mDataBase, 1, this);

    if(mData)
    {
        mData->exec();

        delete mData;
    }*/
    ShowHistory history(mSceneList, this);
    history.setMinimumSize(300, 200);
    history.exec();
}

void Runer::fRealCurve()
{
    QTextStream warout;    //警告
    QFile file("waringlog.txt");
    qDebug()<<file.open(QIODevice::ReadWrite|QIODevice::Text);
    warout.setDevice(&file);
    QTextBrowser *textedit=new QTextBrowser(0);
    textedit->setAttribute(Qt::WA_DeleteOnClose);
    textedit->setWindowTitle(tr("查看警告信息"));

    textedit->setPlainText(warout.readAll());
    textedit->setMinimumSize(800, 500);
    textedit->setFont(QFont("微软雅黑", 12));
    textedit->show();
}

void Runer::fHistoryCurve()
{
    QTextStream dropout;    //历史
    QFile file("droplog.txt");
    qDebug()<<file.open(QIODevice::ReadWrite|QIODevice::Text);
    dropout.setDevice(&file);

    QTextBrowser *textedit=new QTextBrowser(0);
    textedit->setAttribute(Qt::WA_DeleteOnClose);
    textedit->setPlainText(dropout.readAll());
    textedit->setMinimumSize(800, 500);
    textedit->setWindowTitle(tr("查看历史信息"));
    textedit->setFont(QFont("微软雅黑", 12));
    textedit->show();
}
void Runer::fCurve()
{
    if(curverPie)
    {
        curverPie->show();
    }
    else
    {
        curverPie=new MainWidget();  //曲线init
        curverPie->setWindowTitle(tr("实时图表"));
        foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
        {
           curverPie->addLineInterface(timer.TimerId);
        }
        curverPie->addPieInterface("pie");
    }
}

void Runer::fReportCustom()
{
    /*DialogReport *Report = new DialogReport(this);

    if(Report)
    {
        Report->exec();

        delete Report;
    }*/
    if(!mDataBase) {
        QMessageBox::warning(this, tr("导出报表"), tr("没有配置数据库"));
        return;
    }
    ShowHistory history(mSceneList, this, true);
    history.setMinimumSize(300, 200);
    history.exec();
}
void Runer::updateScense()
{
    //关闭县城

    if(senderOrder)
    {
        delete senderOrder;
    }
    thread->quit();
    thread->wait();

    reciverThread->stop();
    reciverThread->exit(0);
    reciverThread->wait();

    timerSend->stop();
    timerSend->exit(0);
    timerSend->wait();

    delete thread;
    delete reciverThread;
    delete timerSend;

    // 清空Scene列表
    foreach(TScene *Scene, mSceneList)
    {
        mSceneList.removeOne(Scene);
        delete Scene;
    }

    // 清空服务列表
    foreach(TService *Service, mServiceList)
    {
        mServiceList.removeOne(Service);
        delete Service;
    }
    {
        mSceneList.clear();
        serIdToServer.clear();
        groupToGraphics.clear();
        mServiceList.clear();
        timerMapToGroup.clear();
        sceneToTImer.clear();
        if(curverPie)
            delete curverPie;
        curverPie=nullptr;
    }
    // 打开关键文件
    qDebug() << QApplication::applicationDirPath() + tr("/main.xml");

    if(!mXml.fOpen(QApplication::applicationDirPath() + tr("/main.xml")))
    {
        qDebug()<<"fdsg";
        QMessageBox::warning(this, tr("初始化"), tr("解析关键文件发生错误"));
        return;
    }


    // 解析数据库信息
//    mXml.fAnalyDB(mDBMsg);
////    if(!mDataBase)
////        fDB(mDBMsg);        //创建数据库

    fLoadFromFile();
    if(!mServiceList.isEmpty())
    {
        foreach (TService *server, mServiceList) {
            qDebug()<<server->fGetId();
            serIdToServer.insert(server->fGetId(), server);
        }
    }

    // 绑定页面
    if(!mSceneList.isEmpty())
    {
        mView->setScene(mSceneList.first());
    }

    foreach(TScene *Scene, mSceneList)
    {
        QList<TItemEx *>ItemExList;
        Scene->fGetItemsList(ItemExList);

        foreach(TItemEx *ItemEx, ItemExList)
        {
            // 设定可操作画布
            ItemEx->fSetHostScene(Scene);
            connect(ItemEx, &TItemEx::sendData, this, &Runer::handleData);
        }

        QList<TGraphics *>GraphicsList;
        Scene->fGetItemsList(GraphicsList);

        foreach(TGraphics *Graphics, GraphicsList)
        {
            if(NULL == Graphics)
            {
                continue;
            }
           if(groupToGraphics.contains(Graphics->fGetGroupId()))
           {
               groupToGraphics.value(Graphics->fGetGroupId())->append(Graphics);
           }
           else
           {
               if(Graphics->fGetGroupId()!=0) {
               QList<TGraphics *> *list=new QList<TGraphics *>;
               list->append(Graphics);
               groupToGraphics.insert(Graphics->fGetGroupId(), list);
               }
           }

            Graphics->fSetDragAble(false);
            Graphics->fSetSelectAble(false);
            Graphics->fSetMoveAble(false);
            Graphics->setAcceptHoverEvents(true);

        }

        QList<TWidgets *>WidgetsList;
        Scene->fGetItemsList(WidgetsList);

        foreach(TWidgets *Widgets, WidgetsList)
        {
            if(Widgets)
            {
                Widgets->fSetDragAble(false);
                Widgets->fSetSelectAble(false);
                Widgets->fSetMoveAble(false);
                Widgets->setAcceptHoverEvents(true);

            }
        }
    }

    if(mDataBase&&mDataBase->fIsOpen())
    {
        //ResultData = mDataBase->fDoSqlResult(tr("SELECT service_id,node_id,val FROM data1"));
        TMySQL * mysql=static_cast<TMySQL *>(mDataBase);
        if(mysql)
        {
        foreach(int id, groupToGraphics.keys())
        {
            QList<TGraphics *> * list=groupToGraphics.value(id);
            QString sql=tr("create table %1 (").arg(id);
            int j=0;
            for(int i=0; i<list->size()-1; i++)
            {
                sql+="value"+QString::number(i)+" double, ";
                j++;
            }
            sql+="value"+QString::number(j)+" double)";                   //插入数据表
            //mysql->qsqlExec(QString("drop table id exists%1").arg(id));
            if(!mysql->qsqlExec(sql))                    //插入数据表
            {
                mysql->qsqlExec(QString("drop table mysql%1").arg(id));
                mysql->qsqlExec(sql);
                qDebug()<<"插入数据表失败";
            }
        }
        }
    }

    //qDebug() << "mDataBase->fDoSqlResult finish";

    // 解析完成,添加动作，刷新页面，绑定信号和槽
    foreach(TScene *Scene, mSceneList)
    {
        QList<TGraphics *>ItemList;
        Scene->fGetItemsList(ItemList);

        //qDebug() << "flush scene" << Scene->fGetName();

        foreach(TGraphics *Graphics, ItemList)
        {
            if(NULL == Graphics)
            {
                continue;
            }

            connect(Graphics, SIGNAL(mGotoLink(int)), this, SLOT(fGotoLink(int)));
            connect(Graphics, SIGNAL(mStartAlarm(double, bool, qint64)), this, SLOT(fGetAlarm(double,bool ,qint64)));
            connect(Graphics, SIGNAL(sendContral(TButton::Control)), this, SLOT(reciverContral(TButton::Control)));

        }

        connect(Scene, SIGNAL(mClickItem(TGraphics*)), this, SLOT(fClick(TGraphics*)));

        QList<TButton *>buttonItemList;
        Scene->fGetItemsList(buttonItemList);
        foreach(TButton *button, buttonItemList)
        {
            if(NULL == button)
            {
                continue;
            }
            connect(button, SIGNAL(mGotoLink(int)), this, SLOT(fGotoLink(int)));   //界面跳转
            connect(button, SIGNAL(sendContral(TButton::Control)), this, SLOT(reciverContral(TButton::Control)));

        }
    }

    qDebug() << "fInitService(); in Runner";
    #ifndef DEF_RUN_AS_CLIENT
    fInitService();
    #else

    #endif
    /*curverPie=new MainWidget();  //曲线init
    curverPie->setWindowTitle(tr("实时图表"));
    foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
    {
       curverPie->addLineInterface(timer.TimerId);
    }
    curverPie->addPieInterface("pie");*/
    fupdateFromInitTImer(this->sceneToTImer);

    /*******************开启发送县城 ********************/
    senderOrder= new SenderOrder();  //初始化发送线程
    connect(this, &Runer::sendData, senderOrder, &SenderOrder::sendOrder);

    thread=new AnalysisThread();
    thread->start();
    reciverThread=new ReciverHandle();
    reciverThread->start();

    timerSend=new TimerSendThread();

    timerSend->start();

}

void Runer::fHistoryData()
{
    #if 0
    // 生成数据库历史数据
    long long Time1 = 1375575291000;
    long long Time2 = 1375611291000;
    for(int i = 0; i < 131; i++)
    {
        QTime time;
        time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);

        int x=qrand()% 5;

        // 1
        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(1)
                          .arg(1)
                          .arg(Time1 - x * 20000)
                          );

        // 1
        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(4)
                          .arg(1)
                          .arg(Time1 - x * 20000)
                          );
        //
        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(5)
                          .arg(1)
                          .arg(Time1 - x * 20000)
                          );


        //
        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(6)
                          .arg(1)
                          .arg(Time1 - x * 20000)
                          );

        //
        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(3)
                          .arg(0)
                          .arg(Time1 - x * 20000)
                          );


        ///////////////////

        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(1)
                          .arg(1)
                          .arg(Time2 + x * 20000)
                          );

        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(4)
                          .arg(1)
                          .arg(Time2 + x * 20000)
                          );
        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(5)
                          .arg(1)
                          .arg(Time2 + x * 20000)
                          );

        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(6)
                          .arg(1)
                          .arg(Time2 + x * 20000)
                          );

        mDataBase->fDoSql(tr("INSERT INTO data2 (service_id, node_id, val, time) VALUES (%1, %2, %3, %4)")
                          .arg(1)
                          .arg(3)
                          .arg(0)
                          .arg(Time2 + x * 20000)
                          );


        Time1 += 86400000;
        Time2 += 86400000;
    }
    #endif

    DialogData *mData = new DialogData(mDataBase, 2, this);

    if(mData)
    {
        mData->exec();

        delete mData;
    }
}

void Runer::fClick(TGraphics *Graphics)
{
    //qDebug() << (void *)mHint << (void *)Item;

    if(mHint && Graphics)
    {
        QString StrHint = Graphics->fGetHint();

        if(StrHint.length() > 0)
        {
            mHint->fSetHint(StrHint);
            mHint->exec();
        }
    }
}

void Runer::handleData(int groupId, double value)
{
    foreach(TScene *sence, mSceneList)
    {
        QList<TGraphics *> list;
        sence->fGetItemsList(list);
        foreach(TGraphics *graphics,list)
        {
            if(graphics->fGetGroupId()==groupId)
            {
                TService *server= serIdToServer.value(graphics->fGetServiceId());
                if(server==nullptr)
                {
                    continue;
                }
                double tempvalue=swapShowNum(graphics->showStart, graphics->showStop, value);

                if(tempvalue != 1)
                {
                    emit sendData(server, graphics->startAddr, graphics->fGetMainId(), graphics->fGetNodeId(), tempvalue*100);

                }
                else
                    emit sendData(server, graphics->startAddr, graphics->fGetMainId(), graphics->fGetNodeId(), tempvalue);
            }
        }
    }
}
double Runer::swapShowNum(double start, double stop,double value)
{
    double spaceNum=stop-start;
    if(spaceNum<=0) {
        return value;
    }
    double a=10.0/spaceNum;
    return a*(value-start);
}

void Runer::insertDatatoDB()
{
    //qDebug()<<"inset data";
    if(curverPie&&Runer::runner->curverPie->isVisible())
    {
    foreach(TScene::TimerStruct timer, Runer::runner->CurverStructList)
    {
        //timer.value=qrand();
        curverPie->setLineValue(timer.TimerId, timer.TimerId, timer.TimerId, QDateTime::currentDateTime().toString(Qt::LocaleDate), QString::number(timer.value));
    }
    QStringList piename, pieValue;
    foreach(TScene::TimerStruct timer, Runer::runner->PieStructList)
    {
        piename.append(timer.TimerId);
        pieValue.append(QString::number(timer.value));
    }

    curverPie->setPieValue(tr("饼图"), tr("饼图"), piename, pieValue);
    }

    if(mDataBase&&mDataBase->fIsOpen())
    {
        TMySQL * mysql=static_cast<TMySQL *>(mDataBase);
        if(mysql)
        {
        foreach(int id, groupToGraphics.keys())
        {
            QList<TGraphics *> * list=groupToGraphics.value(id);
            QString sql=tr("insert into %1%2 values(").arg("mysql").arg(id);
            for(int i=0; i<list->size()-1; i++)
            {
                sql+=QString::number(list->at(i)->fGetValue())+", ";
            }
            sql+=QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch())+")";
            //qDebug()<<sql<<list->size();
            if(!mysql->qsqlExec(sql))                    //插入数据表
            {
                //mysql->qsqlExec(QString("drop table id exists%1").arg(id));
                //qDebug()<<"插入数据表失败";
            }

        }
        }
    }
}
void Runer::reciverContral(TButton::Control contral)
{
    foreach(TService *server, mServiceList) {
        if(server->fGetName()==contral.servername) {
            if(contral.writeNum != 1)
            {
                emit sendData(server, contral.startAddr,contral.mainId , contral.devId, contral.writeNum*100);
            }
            else
            {
                emit sendData(server, contral.startAddr,contral.mainId , contral.devId, contral.writeNum);
            }
        }
    }
}
void Runer::Abort()
{
    QMessageBox::about(this, tr("北京宏昌润达科技发展有限公司"), tr("本软件版权所有北京宏昌润达科技发展有限公司\r\n                     版本号：v3.0"));
}
void Runer::saveDataTimer()
{

    DialogTimer *Alarm = qobject_cast<DialogTimer *>(sender());
    if(!Alarm)
    {
        return;
    }
    qDebug()<<"Alarm->mSave"<<Alarm->mSave;
    if(Alarm->mSave) {
        //qDebug()<<"save timer";
        QMutexLocker{&mutex};
        if(sceneToTImer.contains(Alarm->myscene))
        {
            sceneToTImer.remove(Alarm->myscene);
        }
        this->sceneToTImer.insert(Alarm->myscene, Alarm->timerMapToGroup);
        fSaveTimerToInit(sceneToTImer);

    }
    else {
    //qDebug()<<"delete alarm";
    delete Alarm;
    senceToTImer.remove(Alarm->myscene);
    }
}
void Runer::fSaveTimerToInit(QHash<TScene *, QHash<QString, QList<TimerOnOff> > > &sceneToTImer)
{

    QSettings settings("SceneToTimer", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.clear();
    settings.beginWriteArray("sceneToTimer");
    int keysizeA=0;
    for(QHash<TScene *, QHash<QString, QList<TimerOnOff>>>::iterator ia=sceneToTImer.begin(); ia!=sceneToTImer.end(); ia++)
    {
        settings.setArrayIndex(keysizeA);
        settings.setValue("SceneId", ia.key()->fGetId());
        keysizeA++;

        settings.beginWriteArray("Timer");
        int keysizeB=0;
        for(QHash<QString, QList<TimerOnOff>>::iterator j=ia.value().begin(); j!=ia.value().end(); j++)
        {
            settings.setArrayIndex(keysizeB);
            settings.setValue("TimerId", j.key());
            keysizeB++;

            settings.beginWriteArray("TimerOnOff");
            QList<TimerOnOff> timelist= j.value();
            for(int i=0; i<timelist.count(); i++)
            {
                    settings.setArrayIndex(i);
                    settings.setValue("day", timelist.at(i).day);
                    settings.setValue("time", timelist.at(i).time);
                    settings.setValue("onOff", timelist.at(i).onOff);

            }
            settings.endArray();
        }
        settings.endArray();

    }
    settings.endArray();
}
void Runer::fupdateFromInitTImer(QHash<TScene *, QHash<QString, QList<TimerOnOff> > > &sceneToTImer)
{
    QSettings settings("SceneToTimer", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    int keysizeA=settings.beginReadArray("sceneToTimer");
    for(int a=0; a<keysizeA; a++)
    {
        TScene* Scene=nullptr;
        settings.setArrayIndex(a);
        int sceneId=settings.value("SceneId").toInt();
        foreach(TScene* scent, this->mSceneList) {
            if(scent->fGetId()==sceneId) {
                Scene=scent;
                break;
            }
        }
        QHash<QString, QList<TimerOnOff>> TImerScent;
        int keysizeB=settings.beginReadArray("Timer");
        for(int b=0; b<keysizeB; b++)
        {
            settings.setArrayIndex(b);
            QString Timer=settings.value("TimerId").toString();

            int keysizeC=settings.beginReadArray("TimerOnOff");
            QList<TimerOnOff> timerOnOff;
            for(int c=0; c<keysizeC; c++)
            {
                settings.setArrayIndex(c);
                struct TimerOnOff timeron;
                timeron.day=settings.value("day").toInt();
                timeron.time=settings.value("time").toTime();
                timeron.onOff=settings.value("onOff").toInt();
                timerOnOff.append(timeron);
            }
            TImerScent.insert(Timer, timerOnOff);
            settings.endArray();
        }

        settings.endArray();
        sceneToTImer.insert(Scene, TImerScent);
    }
    settings.endArray();
}


