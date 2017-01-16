#include "DialogData.h"
#include "../../Src/File/File.h"
#include "../../Src/TableView/TableService.h"

#include <QMessageBox>

DialogData::DialogData(TDataBase *DataBase, int TabelNum, QWidget *parent) : QDialog(parent)
{
    mTabelNum = TabelNum;
    mDataBase = DataBase;

    mLabelStart = TBase::fLabel(tr("从"), tr("mLabelStart"));
    mLabelStop = TBase::fLabel(tr("到"), tr("mLabelStop"));
    mDateTimeStart = TBase::fDateTimeEdit(tr("mDateTimeStart"), -12);
    mDateTimeStop = TBase::fDateTimeEdit(tr("mDateTimeStop"));
    mCheckBoxAll = TBase::fCheckBox(tr("全部"), tr("mCheckBoxAll"));
    connect(mCheckBoxAll, SIGNAL(clicked(bool)), this, SLOT(fAllTime(bool)));
    mButtonSearch = TBase::fButton(tr("搜索"), tr("mButtonSearch"));
    connect(mButtonSearch, SIGNAL(clicked()), this, SLOT(fSearch()));

    QHBoxLayout *mLayoutTop = new QHBoxLayout();
    mLayoutTop->addStretch();
    mLayoutTop->addWidget(mLabelStart);
    mLayoutTop->addWidget(mDateTimeStart);
    mLayoutTop->addWidget(mLabelStop);
    mLayoutTop->addWidget(mDateTimeStop);
    mLayoutTop->addStretch();
    mLayoutTop->addWidget(mCheckBoxAll);
    mLayoutTop->addStretch();
    mLayoutTop->addWidget(mButtonSearch);
    mLayoutTop->addStretch();

    mTable = new TTableView(tr("mTabel"));
    mTable->fAddCol(tr("服务"));
    mTable->fAddCol(tr("节点"));
    mTable->fAddCol(tr("时间"));
    mTable->fAddCol(tr("数据"));

    mTable->fSetHeadLength(0, 200);
    mTable->fSetHeadLength(1, 200);
    mTable->fSetHeadLength(2, 180);
    mTable->fSetHeadLength(3, 70);
    mTable->fUpdateFixWidth();

    mButtonOut = TBase::fButton(tr("导出"), tr("mButtonOut"), 75, 36);
    connect(mButtonOut, SIGNAL(clicked()), this, SLOT(fOut()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOut);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new QVBoxLayout(this);
    mLayoutMain->addLayout(mLayoutTop);
    mLayoutMain->addWidget(mTable);
    mLayoutMain->addSpacing(10);
    mLayoutMain->addLayout(mLayoutButton);

    // 设置默认
    mButtonClose->setFocus();
    mButtonClose->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonClose->setShortcut(Qt::Key_Enter);
    mButtonClose->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);

    switch(mTabelNum)
    {
    case 1:
        setWindowTitle(tr("最新数据"));
        break;

    case 2:
        setWindowTitle(tr("历史数据"));
        break;

    default:
        setWindowTitle(tr("数据"));
        break;
    }

    setFixedSize(sizeHint());
}

void DialogData::fAllChecked()
{
    mDateTimeStart->setDateTime(QDateTime::fromMSecsSinceEpoch(0));
    mDateTimeStop->setDateTime(QDateTime::currentDateTime());
    mDateTimeStart->setEnabled(false);
    mDateTimeStop->setEnabled(false);
}

void DialogData::fAllTime(bool Checked)
{
    if(Checked)
    {
        fAllChecked();
    }
    else
    {
        mDateTimeStart->setDateTime(QDateTime::currentDateTime().addMonths(-12));
        mDateTimeStop->setDateTime(QDateTime::currentDateTime());
        mDateTimeStart->setEnabled(true);
        mDateTimeStop->setEnabled(true);
    }
}

DialogData::~DialogData()
{
}

void DialogData::fSearch()
{
    mTable->fClear();

    if(mCheckBoxAll->isChecked())
    {
        fAllChecked();
    }

    fUpdateTable(mDateTimeStart->dateTime().toMSecsSinceEpoch(),
                 mDateTimeStop->dateTime().toMSecsSinceEpoch());
}

void DialogData::fOut()
{
    TTableService::fOutToCsv(mTable, this);
}

void DialogData::fUpdateTable(qint64 StartTime, qint64 StopTime)
{
    if(mDataBase)
    {
        QList< QList<QVariant> >Result = mDataBase->fDoSqlResult(tr("SELECT service.name, node.name, data%1.time, data%1.val FROM data%1, service, node WHERE data%1.service_id = service.id AND data%1.node_id = node.id AND data%1.time >= %2 AND data%1.time <= %3").arg(mTabelNum).arg(StartTime).arg(StopTime));

        foreach(QList<QVariant> ResultLine, Result)
        {
            if(ResultLine.count() < 4)
            {
                continue;
            }

            QList<QString> Data;
            Data.push_back(ResultLine.at(0).toString());
            Data.push_back(ResultLine.at(1).toString());
            Data.push_back(QDateTime::fromMSecsSinceEpoch(ResultLine.at(2).toDouble()).toString("yyyy-MM-dd HH:mm:ss.zzz"));
            Data.push_back(QString::number(ResultLine.at(3).toDouble()));
            mTable->fAddRow(Data);
        }
    }
}
