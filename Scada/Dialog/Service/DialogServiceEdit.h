#ifndef DIALOG_SERVICE_EDIT_H
#define DIALOG_SERVICE_EDIT_H

#include "../Scada/Src/Service/Service.h"
#include "../Scada/Src/TableView/TableView.h"
#include "../Scada/Src/Base/Base.h"

class DialogServiceEdit : public QDialog
{
    Q_OBJECT
public:
    DialogServiceEdit(TService *Service, QWidget *parent = 0);
    ~DialogServiceEdit();

    bool mSave;
    int ipNum=1;
    TService *mService;

private slots:
    void fOk();

    void fPortChanged(int index);
    void fAgreeMentChanged(int index);
    void fSystemChanged(int index);

    void fRtuAdd();
    void fRtuDel();

private:

    QGroupBox *mGroupBoxServiceMsg;
    QLabel* mLabelServiceNameShow;
    QLineEdit* mLineEditServiceName;
    QLabel* mLabelPortShow;
    QComboBox* mComboBoxPort;
    QLabel* mLabelPortMode;
    QComboBox* mComboBoxPortMode;
    QLabel *mLabelRecvModeShow;
    QComboBox *mComboBoxRecvMode;
    QLabel* mLabelAgreeMentShow;
    QComboBox* mComboBoxAgreeMent;

    // 网络IP Port
    QGroupBox *mGroupBoxNetMsg;
    QLabel *mLabelIpShow;
    QLineEdit *mLineEditIp;

    QLabel *mLabelIPNum;
    QLineEdit *mLineEditIpNum;

    QLabel* mLabelNetPortShow;
    QSpinBox *mSpinBoxNetPort;
    // 客户端
    QGroupBox *mGroupBoxClientMsg;
    // 服务器
    QGroupBox *mGroupBoxServerMsg;
    QLabel *mLabelIoThreadShow;
    QSpinBox *mSpinBoxIoThread;
    QLabel *mLabelProcessThreadShow;
    QSpinBox *mSpinBoxProcessThread;

    // 串口
    QGroupBox *mGroupBoxComMsg;
    QLabel *mLabelSystemShow;
    QComboBox *mComboBoxSystem;
    QLabel *mLabelComNumShow;
    QSpinBox *mSpinBoxComNum;
    QLabel *mLabelComNameShow;
    QLineEdit *mLineEditComName;
    QLabel *mLabelComRateShow;
    QComboBox *mComboBoxComRate;
    QLabel *mLabelComDataShow;
    QComboBox *mComboBoxComData;
    QLabel *mLabelComStopShow;
    QComboBox *mComboBoxComStop;
    QLabel *mLabelComCheckShow;
    QComboBox *mComboBoxComCheck;

    // Iec104
    QGroupBox *mGroupBoxIec104Msg;
    QLabel *mLabelIec104Addr;
    QSpinBox *mSpinBoxIec104Addr;
    QLabel *mLabelIec104UnMaxSumShow;
    QSpinBox *mSpinBoxIec104UnMaxSum;
    QLabel *mLabelIec104UnMaxTimeShow;
    QSpinBox *mSpinBoxIec104UnMaxTime;
    QLabel *mLabelIec104SendReasonShow;
    QComboBox *mComboBoxIec104SendReason;
    QLabel *mLabelIec104AppAddrShow;
    QComboBox *mComboBoxIec104AppAddr;
    QLabel *mLabelIec104MsgAddrShow;
    QComboBox *mComboBoxIec104MsgAddr;
    QLabel *mLabelIec104TimeLenShow;
    QComboBox *mComboBoxIec104TimeLen;

    // Modebus RTU
    QGroupBox *mGroupBoxRtuMsg;
    QLabel *mLableRtuListShow;
    TTableView *mTableRtuList;
    QLabel *mLabelRtuHostShow;
    QSpinBox *mSpinBoxRtuHost;
    QLabel *mLabelRtuFunShow;
    QSpinBox *mSpinBoxRtuFun;
    QLabel *mLabelRtuStartAddrShow;
    QSpinBox *mSpinBoxRtuStartAddr;
    QLabel *mLabelRtuRegCountShow;
    QSpinBox *mSpinBoxRtuRegCount;
    QLabel *mLabelRtuDataLenShow;
    QSpinBox *mSpinBoxRtuDataLen;
    QLabel *mLabelRtuTypeShow;
    QSpinBox *mSpinBoxRtuType;

    QPushButton *mButtonRtuAdd;
    QPushButton *mButtonRtuDel;

    QLabel *mLabelResult;

    QPushButton* mButtonOk;
    QPushButton* mButtonClose;

    void fInit();
    void fCreateIec104();
    void fCreateModebusRtu();

    bool fCheckServiceMsg();
    void fSetServiceMsg();

    void fCreateNet();
    void fCreateServer();
    void fCreateClient();
    void fCreateCom();
};

#endif // DIALOG_SERVICE_EDIT_H
