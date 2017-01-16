#ifndef GONG_H
#define GONG_H

#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/Connection/Com/Com.h"
//#include "../Scada/Src/Protocol/Protocol.h"
#include "../Scada/Src/Protocol/Modbus/Modbus.h"
#include "../Scada/Src/Action/Action.h"
#include "../Scada/Src/Thread/ThreadConnection.h"

#include "../Scada/Src/Graphics/GraphicsPub.h"
#include "../Scada/Src/Item/Item.h"
#include "../Scada/Src/Scene/Scene.h"
#include "../Scada/Src/View/View.h"
#include "../Scada/Src/Base/Base.h"
#include "../Scada/Src/Clock/Clock.h"
#include "../Scada/Src/Scale/Scale.h"
#include "../Scada/Src/LightView/LightView.h"

#include "GongConfig/GongConfig.h"

#include <QMainWindow>
#include <QtWidgets/QMenuBar>

class Gong : public QMainWindow
{
    Q_OBJECT
public:
    Gong(QWidget *parent = 0);
    ~Gong();

private slots:
    void fDoStep();
    void fStart();
    void fStop();
    void fConfig();
    void fConfigCom();

    void fStepAngleChanged(double Angle);
    void fStepLengthChanged(double Length);
    void fDataCome(int NodeId, double Value, quint64 Time);

private:
    QList<TGongConfig *> mConfigList;
    QMenuBar *mMenuBar;

    QMenu *mUserMenu;
    TAction *mActionLogin;
    TAction *mActionUser;
    TAction *mActionPermission;
    TAction *mActionDuty;

    QMenu *mConfigMenu;
    TAction *mActionConfig;
    TAction *mActionConfigCom;

    double mCurrentAngle;
    QLabel *mLabelAngle;
    QLineEdit *mLineEditAngle;
    QLabel *mLabelLength1;
    QLabel *mLabelLength2;
    QLineEdit *mLineEditLength1;
    QLineEdit *mLineEditLength2;
    QLabel *mLabelStepAngleShow;
    QLabel *mLabelStepLengthShow;
    QDoubleSpinBox *mSpinBoxAngleStep;
    QLabel *mLabelStepAngle;
    QDoubleSpinBox *mSpinBoxLengthStep;
    QLabel *mLabelStepLength;
    QPushButton *mButtonLeft;
    QPushButton *mButtonRight;
    QPushButton *mButtonStop;
    QPushButton *mButtonStart;

    TLightView *mLight;
    QLabel *mLabelAlarm;

    TArc *mGong;
    TArc *mGongXiao;
    TRect *mHuaKuai;
    TRect *mHuaKuaiXiao;
    TLine *mGan;
    TRect *mZuan;
    TGroup *mSheBei;
    TText *mTextLength;
    TText *mTextSpeed;

    TScene *mScene;
    TView *mView;

    TCom *mCom;
    TComData *mComData;

    const double mPi;
    const double mRadius;

    TScale *mScale1;
    TScale *mScale2;
    TScale *mScale3;
    TScale *mScale4;

    QLabel *mLableState;
    TClock *mClock;
    TModbus *mModbus;
    TConnection *mConnection;
    TThreadConnection *mThreadConnection;

    void fCreateMenu();
    TAction* fCreateAction(const QString Text);
    void fUpdateWidgets();
    void fUpdateModebus();
};

#endif // GONG_H
