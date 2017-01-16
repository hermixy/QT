#ifndef V6SERVICE_H
#define V6SERVICE_H

//#include "../tcp/tcpserver.h"
//#include "../threadv6/thread.h"
//#include "../table/table.h"

#include <QMainWindow>

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
//#include <QtWidgets/QListView>
//#include <Qtwidgets/QTableView>

class V6Service : public QMainWindow
{
    Q_OBJECT
public:
    V6Service(QWidget *parent = 0);

private slots:
    void fServerStart();
    void fServerStop();
    void fNewSocket(const QString Name, const QString IP, const QString Port);
    void fDelSocket(const QString Name, const QString IP, const QString Port);
    void fRecvData(const QString Name, const QString IP, const QString Port, const QString pcRecv);
    void fSendData();
    void fComboBoxSendChanged(int Index);

private:
    TTcpServer *mServer;
    TThreadV6 *mThreadv6;

    QLabel *mLabelSoftName;
    QLabel *mLabelCompany;
    QPushButton *mPushButtonStart;
    QPushButton *mPushButtonStop;
    QLabel *mLabelIP;
    QTextEdit *mTextEditIP;
    QLabel *mLabelPort;
    QSpinBox *mSpinBoxPort;
    //QListView *mListView;
    QTextEdit *mTextEditMsg;

    TTable *mTableSocket;
    QComboBox *mComboBoxSend;
    QTextEdit *mTextEditSend;
    QPushButton *mButtonSend;

    void fAddRunMsg(const QString &mStr);
};

#endif // V6SERVICE_H
