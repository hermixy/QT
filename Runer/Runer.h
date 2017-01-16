#ifndef RUNER_H
#define RUNER_H

//#include "../Scada/Src/RunMode/RunMode.h"
#include "../Scada/Dialog/Hint/DialogHint.h"
#include "../Scada/Src/DataBase/DataBasePub.h"
#include "../Scada/Src/Xml/XmlReader.h"
#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/Thread/ThreadServer.h"
#include "../Scada/Src/Thread/ThreadConnection.h"
#include "../Scada/Src/Thread/ThreadClient.h"
#include "../Scada/Src/Thread/analysisthread.h"
#include "../Scada/Src/Thread/reciverhandle.h"
#include "../Scada/Src/Thread/senderorder.h"
#include "../Scada/Src/Action/Action.h"
#include "../Scada/Dialog/Timer/timerframeitem1.h"
#include "../Scada/Src/Thread/timersendthread.h"

#include <QtXml/QDomDocument>
#include <QMainWindow>
#include <QTextBrowser>
#include <QSound>
class AlarmDialog;
class TItem;
class TScene;
class TView;
class DialogTimer;
class TGroup;
class QDomElement;
class OnlineStatus;
#include <QtWidgets/QMenuBar>
class MainWidget;
struct MyLoginData {
    bool permission;
    QString userName;
    QString password;
};
class Runer : public QMainWindow
{
    Q_OBJECT
public:
    Runer(QWidget *parent = 0, bool isAdmin=false);
    ~Runer();
    static Runer *runner;
signals:
    void sendData(TService *server, quint16 startAddr, int mainId, int secondId, int value);         //发送数据到处理线程

public slots:
    void fGotoLink(int LinkId);
    void fGetAlarm(double Value, bool upordown, qint64 Time);

    void insertDatatoDB();
    int excel(QTableView *tableView);
    void reciverContral(TButton::Control contral);
     void handleData(int groupId, double value);
     double swapShowNum(double start, double stop, double value);
     void saveDataTimer();

private slots:
    void fAlarm();
    void fLogin();
    void fAccredit();
    void fStart();
    void fCreateUser();
    void fDataCom(int ServiceId, int NodeId, double Value, quint64 Time);
    void fLatelyData();
    void fHistoryData();
    void fRealCurve();
    void fHistoryCurve();
    void fCurve();
    void fReportCustom();
    void fClick(TGraphics *Graphics);
    void updateScense();
    void Abort();
    void fSaveTimerToInit(QHash<TScene *, QHash<QString, QList<TimerOnOff>>> &sceneToTImer);
    void fupdateFromInitTImer(QHash<TScene *, QHash<QString, QList<TimerOnOff>>> &sceneToTImer);
public slots:


public:
    QList<TScene::TimerStruct> CurverStructList;
    QList<TScene::TimerStruct> PieStructList;

    QHash<TScene * ,DialogTimer *> senceToTImer;
    QMap<int, QMap<int, QList<TGraphics*> > >mGraphicsMap;
    QMap<int, QList<TGraphics *>* > groupToGraphics;
    QTimer *timer;
    //QLabel *lableTime; //实时显示时间

    DialogHint *mHint;
    int mUserId;
    QHash<QString, QList<TimerOnOff>> timerMapToGroup; //定时设置1-7
    QHash<TScene *, QHash<QString, QList<TimerOnOff>>> sceneToTImer;
    TDataBase *mDataBase;                //数据库
    QList<TProtocol *>mProtocolList;
    QList<TTcpServer *>mServerList;
    QList<TConnection *>mConnectionList;

    QMenuBar *mMenuBar;
    QMenu *mUserMenu;
    TAction *mActionLogin;
    TAction *mActionUser;
    TAction *mActionPermission;
    TAction *mActionRole;

    QMenu *mDataMenu;
    TAction *mActionDataLately;
    TAction *mActionDataHistory;
    TAction *mActionDataRealCurve;
    TAction *mActionDataHistoryCurve;

    QMenu *mAlarmMenu;
    TAction *mActionAlarm;
    TAction *mActionIgnoreAlarm;

    QMenu *mReportMenu;
    TAction *mActionReportCustom;
    TAction *mActionReportAuto;

    QMenu *mSystemMenu;
    TAction *mActionRefresh;
    TAction *mActionReStart;
    TAction *mActionExit;

    QMenu *mAboutMenu;
    TAction *mActionAbout;

    TThreadClient *ThreadClient;
    SenderOrder *senderOrder;
    QList<TThreadServer*> mThreadServerList;
    QList<TThreadConnection*> mThreadConnectionList;
    QList<TScene*> mSceneList;             //场景列表

    TView *mView;

    TXmlReader mXml;
    QList<TService *> mServiceList;        //服务列表
    QMap<int, TService *> serIdToServer;  //服务号到服务
    TDataBaseMsg *mDBMsg;
    QMutex mutex;
    QTextBrowser *textBrowser=nullptr;

    bool userAccess=false;

    void fInit();
    bool fLoadFromFile();
    bool fUpdateDB();
    bool fLoadFromDataBase();
    bool fDB(TDataBaseMsg *DBMsg);

    void fCreateMenu();
    TAction* fCreateAction(const QString Text);
    AlarmDialog *alarmdialog;

    void fClose();
    void closeEvent(QCloseEvent *event);

    void fInitService();
    void fAddGraphics(int ServiceId, int NodeId,  TGraphics *Graphics);
    void fInitLinkage(const QList<TLinkage *> &LinkageList);

    OnlineStatus *onlinestatus=nullptr;//在线状态

    QSound *sound;
    int isalarm=0;
    bool isAdmins=false;
    MainWidget *curverPie=nullptr;
    /************************县城***************/
    AnalysisThread *thread;
    ReciverHandle *reciverThread;

    TimerSendThread *timerSend;
};

#endif // RUNER_H
