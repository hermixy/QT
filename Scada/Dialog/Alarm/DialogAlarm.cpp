#include "DialogAlarm.h"
//#include <QDateTime>

DialogAlarm::DialogAlarm(TDataBase *DataBase, int UserId, QWidget *parent) : QDialog(parent)
{
    mUserId = UserId;
    mDataBase = DataBase;

    mTable = new TTableView(tr("mTabel"));
    mTable->fAddCol(tr("时间"));
    mTable->fAddCol(tr("类型"));
    mTable->fAddCol(tr("信息"));
    mTable->fAddCol(tr("处理人"));
    mTable->fAddCol(tr("处理时间"));

    mTable->fSetHeadLength(0, 150);
    mTable->fSetHeadLength(1, 50);
    mTable->fSetHeadLength(2, 50);
    mTable->fSetHeadLength(3, 100);
    mTable->fSetHeadLength(4, 150);
    mTable->fUpdateFixWidth();

    fUpdateTable();

    mButtonIgnoreAll = TBase::fButton(tr("处理所有"), tr("mButtonIgnoreAll"), 75, 36);
    connect(mButtonIgnoreAll, SIGNAL(clicked()), this, SLOT(fIgnoreAll()));
    mButtonIgnore = TBase::fButton(tr("处理选中"), tr("mButtonIgnore"), 75, 36);
    connect(mButtonIgnore, SIGNAL(clicked()), this, SLOT(fIgnore()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mButtonClose"), 50, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonIgnoreAll);
    mLayoutButton->addWidget(mButtonIgnore);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new QVBoxLayout(this);
    mLayoutMain->addWidget(mTable);
    mLayoutMain->addSpacing(10);
    mLayoutMain->addLayout(mLayoutButton);

    // 设置默认
    mButtonClose->setFocus();
    mButtonClose->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonClose->setShortcut(Qt::Key_Enter);
    mButtonClose->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("报警"));
    setFixedSize(sizeHint());
}

DialogAlarm::~DialogAlarm()
{}

void DialogAlarm::fIgnore()
{
    int index = mTable->fGetCurrentIndex();

    //qDebug() << "DialogAlarm::fIgnore()" << index;

    if(index >= 0 && index < mAlarmMsg.length())
    {
        QList<QVariant> ResultLine = mAlarmMsg.at(index);

        QDateTime DateTime = QDateTime::currentDateTime();
        QString sTime = DateTime.toString("yyyy-MM-dd HH:mm:ss");

        bool OkSql = mDataBase->fDoSql(tr("UPDATE alarm SET user_id = %1, ignore_time = \"%2\" WHERE id = %3")
                              .arg(mUserId)
                              .arg(sTime)
                              .arg(ResultLine.at(0).toInt())
                            );

        if(OkSql)
        {
            mTable->fSetCell(index, 3, QString::number(mUserId));
            mTable->fSetCell(index, 4, sTime);
        }
    }
}

void DialogAlarm::fIgnoreAll()
{
    if(NULL == mDataBase)
    {
        return;
    }

    QDateTime DateTime = QDateTime::currentDateTime();
    QString sTime = DateTime.toString("yyyy-MM-dd HH:mm:ss");

    bool OkSql = mDataBase->fDoSql(tr("UPDATE alarm SET user_id = %1, ignore_time = \"%2\" WHERE user_id IS NULL OR user_id <= 0")
                          .arg(mUserId)
                          .arg(sTime)
                        );

    if(OkSql)
    {
        mTable->fClear();
        fUpdateTable();
    }
}

void DialogAlarm::fUpdateTable()
{
    if(mDataBase)
    {
        mLineSum = 5;
        mAlarmMsg = mDataBase->fDoSqlResult(tr("SELECT alarm.id, alarm.time, alarm.val, user.name, alarm.ignore_time FROM alarm LEFT JOIN user ON alarm.user_id = user.id ORDER BY id"));

        /*for(int i = 0; i + mLineSum <= mAlarmMsg.length(); i += mLineSum)
        {
            QList<QString> Data;
            //Data.push_back(QString::number(i / LineSum + 1));
            Data.push_back(QDateTime::fromMSecsSinceEpoch(mAlarmMsg.at(i + 1).toDouble()).toString("yyyy-MM-dd HH:mm:ss.zzz"));
            Data.push_back(tr(" "));
            Data.push_back(QString::number(mAlarmMsg.at(i + 2).toDouble()));
            Data.push_back(QString::number(mAlarmMsg.at(i + 3).toInt()));
            Data.push_back(mAlarmMsg.at(i + 4).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            mTable->fAddRow(Data);
        }*/

        foreach(QList<QVariant> ResultLine, mAlarmMsg)
        {
            //qDebug() << "ResultLine.count()" << ResultLine.count();
            if(ResultLine.count() < 5)
            {
                continue;
            }

            QList<QString> Data;
            Data.push_back(QDateTime::fromMSecsSinceEpoch(ResultLine.at(1).toDouble()).toString("yyyy-MM-dd HH:mm:ss.zzz"));
            Data.push_back(tr(" "));
            Data.push_back(QString::number(ResultLine.at(2).toDouble()));
            Data.push_back(ResultLine.at(3).toString());
            Data.push_back(ResultLine.at(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            mTable->fAddRow(Data);
        }
    }
}
