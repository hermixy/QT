#include "DialogGongConfig.h"
#include "../Scada/Src/TableView/TableService.h"
#include "../Scada/Src/File/File.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

DialogConfigGong::DialogConfigGong(QList<TGongConfig *> &ConfigList, QWidget *parent) : QDialog(parent)
{
    mSave = false;

    foreach(TGongConfig *Config, ConfigList)
    {
        mList.push_back(new TGongConfig(Config));
    }

    mTable = new TTableView(tr("mTable"));
    mTable->fAddCol(tr("编号"));
    mTable->fAddCol(tr("名称"));
    mTable->fAddCol(tr("地址"));
    mTable->fAddCol(tr("通道"));
    mTable->fAddCol(tr("零值"));
    mTable->fAddCol(tr("满值"));
    mTable->fAddCol(tr("量程"));
    mTable->fAddCol(tr("单位"));
    mTable->fAddCol(tr("报警上限"));
    mTable->fAddCol(tr("报警下限"));
    mTable->fAddCol(tr("显示类型"));
    mTable->fAddCol(tr("系数"));
    mTable->fAddCol(tr("计算周期"));
    mTable->fAddCol(tr("过冲时间"));

    foreach(TGongConfig *Config, mList)
    {
        QList<QString>RowData;
        RowData.push_back(QString::number(Config->mId));
        RowData.push_back(Config->mName);
        RowData.push_back(QString::number(Config->mAddr));
        RowData.push_back(QString::number(Config->mChan));
        RowData.push_back(QString::number(Config->mZero));
        RowData.push_back(QString::number(Config->mFull));
        RowData.push_back(QString::number(Config->mRange));
        RowData.push_back(Config->mUnit);
        RowData.push_back(QString::number(Config->mAlarmUpper));
        RowData.push_back(QString::number(Config->mAlarmLower));
        RowData.push_back(QString::number(Config->mDisplayType));
        RowData.push_back(QString::number(Config->mCoe));
        RowData.push_back(QString::number(Config->mCycle));
        RowData.push_back(QString::number(Config->mTime));
        mTable->fAddRow(RowData);
    }

    connect(mTable, SIGNAL(mClick(int,int)), this, SLOT(fTableClick(int,int)));

    ///////////////////////////////////////////
    mLabelId = TBase::fLabel(tr("编号"), tr("mLabelId"));
    mSpinBoxId = TBase::fSpinBox(tr("mSpinBoxId"), 0, 102400);
    mLabelName = TBase::fLabel(tr("名称"), tr("mLabelName"));
    mLineEditName = TBase::fLineEdit(tr("mLineEditName"));
    mLabelAddr = TBase::fLabel(tr("地址"), tr("mLabelAddr"));
    mSpinBoxAddr = TBase::fSpinBox(tr("mSpinBoxAddr"), 0, 102400);
    mLabelChannel = TBase::fLabel(tr("通道"), tr("mLabelChannel"));
    mSpinBoxChannel = TBase::fSpinBox(tr("mSpinBoxChannel"), 0, 102400);
    mLabelZero = TBase::fLabel(tr("零值"), tr("mLabelZero"));
    mSpinBoxZero = TBase::fDoubleSpinBox(tr("mSpinBoxZero"), -102400, 102400);
    mLabelFull = TBase::fLabel(tr("满值"), tr("mLabelFull"));
    mSpinBoxFull = TBase::fDoubleSpinBox(tr("mSpinBoxFull"), -102400, 102400);
    mLabelRange = TBase::fLabel(tr("量程"), tr("mLabelRange"));    
    mSpinBoxRange = TBase::fDoubleSpinBox(tr("mSpinBoxRange"), 0, 102400);
    mLabelUnit = TBase::fLabel(tr("单位"), tr("mLabelUnit"));
    mLineEditUnit = TBase::fLineEdit(tr("mLineEditUnit"));

    mLabelAlarmUpper = TBase::fLabel(tr("报警上限"), tr("mLabelAlarmUpper"));
    mSpinBoxAlarmUpper = TBase::fDoubleSpinBox(tr("mSpinBoxAlarmUpper"), 0, 102400);
    mLabelAlarmLower = TBase::fLabel(tr("报警下限"), tr("mLabelAlarmLower"));
    mSpinBoxAlarmLower = TBase::fDoubleSpinBox(tr("mSpinBoxAlarmLower"), 0, 102400);
    mLabelCoe = TBase::fLabel(tr("系数"), tr("mLabelCoe"));
    mSpinBoxCoe = TBase::fDoubleSpinBox(tr("mSpinBoxCoe"), 0.0, 102400.0);
    mLabelDisplayType = TBase::fLabel(tr("显示类型"), tr("mLabelDisplayType"));
    mSpinBoxDisplayType = TBase::fSpinBox(tr("mSpinBoxDisplayType"), 0, 102400);
    mLabelCycle = TBase::fLabel(tr("计算周期"), tr("mLabelCycle"));
    mSpinBoxCycle = TBase::fDoubleSpinBox(tr("mSpinBoxCycle"), 0.0, 102400.0);
    mLabelTime = TBase::fLabel(tr("过冲时间"), tr("mLabelTime"));
    mSpinBoxTime = TBase::fDoubleSpinBox(tr("mSpinBoxTime"), 0.0, 102400.0);

    QGridLayout *mLayoutMsg = new QGridLayout();
    mLayoutMsg->addWidget(mLabelId, 0, 0);
    mLayoutMsg->addWidget(mSpinBoxId, 0, 1);
    mLayoutMsg->addWidget(mLabelName, 0, 2);
    mLayoutMsg->addWidget(mLineEditName, 0, 3);
    mLayoutMsg->addWidget(mLabelAddr, 0, 4);
    mLayoutMsg->addWidget(mSpinBoxAddr, 0, 5);
    mLayoutMsg->addWidget(mLabelChannel, 0, 6);
    mLayoutMsg->addWidget(mSpinBoxChannel, 0, 7);
    mLayoutMsg->addWidget(mLabelZero, 1, 0);
    mLayoutMsg->addWidget(mSpinBoxZero, 1, 1);
    mLayoutMsg->addWidget(mLabelFull, 1, 2);
    mLayoutMsg->addWidget(mSpinBoxFull, 1, 3);
    mLayoutMsg->addWidget(mLabelRange, 1, 4);
    mLayoutMsg->addWidget(mSpinBoxRange, 1, 5);
    mLayoutMsg->addWidget(mLabelUnit, 1, 6);
    mLayoutMsg->addWidget(mLineEditUnit, 1, 7);
    mLayoutMsg->addWidget(mLabelAlarmUpper, 2, 0);
    mLayoutMsg->addWidget(mSpinBoxAlarmUpper, 2, 1);
    mLayoutMsg->addWidget(mLabelAlarmLower, 2, 2);
    mLayoutMsg->addWidget(mSpinBoxAlarmLower, 2, 3);
    mLayoutMsg->addWidget(mLabelDisplayType, 2, 4);
    mLayoutMsg->addWidget(mSpinBoxDisplayType, 2, 5);
    mLayoutMsg->addWidget(mLabelCoe, 2, 6);
    mLayoutMsg->addWidget(mSpinBoxCoe, 2, 7);
    mLayoutMsg->addWidget(mLabelCycle, 3, 0);
    mLayoutMsg->addWidget(mSpinBoxCycle, 3, 1);
    mLayoutMsg->addWidget(mLabelTime, 3, 2);
    mLayoutMsg->addWidget(mSpinBoxTime, 3, 3);
    ///////////////////////////////////////////
    mButtonAdd = TBase::fButton(tr("添加"), tr("mButtonAdd"), 75, 24);
    connect(mButtonAdd, SIGNAL(clicked()), this, SLOT(fAdd()));
    mButtonEdit = TBase::fButton(tr("编辑"), tr("mButtonEdit"), 75, 24);
    connect(mButtonEdit, SIGNAL(clicked()), this, SLOT(fEdit()));
    mButtonDel = TBase::fButton(tr("删除"), tr("mButtonDel"), 75, 24);
    connect(mButtonDel, SIGNAL(clicked()), this, SLOT(fDel()));
    mButtonLoad = TBase::fButton(tr("导入"), tr("mButtonLoad"), 75, 24);
    connect(mButtonLoad, SIGNAL(clicked()), this, SLOT(fLoad()));
    mButtonOut = TBase::fButton(tr("导出"), tr("mButtonOut"), 75, 24);
    connect(mButtonOut, SIGNAL(clicked()), this, SLOT(fOut()));
    mButtonOK = TBase::fButton(tr("保存"), tr("mButtonOK"), 75, 24);
    connect(mButtonOK, SIGNAL(clicked()), this, SLOT(fOK()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 24);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mLayoutButtonEdit = new QVBoxLayout();
    mLayoutButtonEdit->addWidget(mButtonAdd);
    mLayoutButtonEdit->addWidget(mButtonEdit);
    mLayoutButtonEdit->addWidget(mButtonDel);

    QHBoxLayout *mLayoutLocal = new QHBoxLayout();
    mLayoutLocal->addLayout(mLayoutMsg);
    mLayoutLocal->addSpacing(10);
    mLayoutLocal->addLayout(mLayoutButtonEdit);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonLoad);
    mLayoutButton->addWidget(mButtonOut);
    mLayoutButton->addWidget(mButtonOK);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new  QVBoxLayout(this);
    mLayoutMain->addWidget(mTable);
    mLayoutMain->addLayout(mLayoutLocal);
    mLayoutMain->addSpacing(15);
    mLayoutMain->addLayout(mLayoutButton);

    // 设置默认
    mButtonClose->setFocus();
    mButtonClose->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonClose->setShortcut(Qt::Key_Enter);
    mButtonClose->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("配置管理"));
    setFixedSize(sizeHint());
}

DialogConfigGong::~DialogConfigGong()
{
    foreach(TGongConfig *Config, mList)
    {
        mList.removeOne(Config);
        delete Config;
    }
}

void DialogConfigGong::fAdd()
{
    int Id = mSpinBoxId->value();
    QString Name = mLineEditName->text();
    int Addr = mSpinBoxAddr->value();
    int Chan = mSpinBoxChannel->value();
    double Zero = mSpinBoxZero->value();
    double Full = mSpinBoxFull->value();
    double Range = mSpinBoxRange->value();
    QString Unit = mLineEditUnit->text();
    double AlarmUpper = mSpinBoxAlarmUpper->value();
    double AlarmLower = mSpinBoxAlarmLower->value();
    int DisplayType = mSpinBoxDisplayType->value();
    double Coe = mSpinBoxCoe->value();
    double Cycle = mSpinBoxCycle->value();
    double Time = mSpinBoxTime->value();

    QList<QString> RowData;
    RowData.push_back(QString::number(Id));
    RowData.push_back(Name);
    RowData.push_back(QString::number(Addr));
    RowData.push_back(QString::number(Chan));
    RowData.push_back(QString::number(Zero));
    RowData.push_back(QString::number(Full));
    RowData.push_back(QString::number(Range));
    RowData.push_back(Unit);
    RowData.push_back(QString::number(AlarmUpper));
    RowData.push_back(QString::number(AlarmLower));
    RowData.push_back(QString::number(DisplayType));
    RowData.push_back(QString::number(Coe));
    RowData.push_back(QString::number(Cycle));
    RowData.push_back(QString::number(Time));
    mTable->fAddRow(RowData);

    TGongConfig *Config = new TGongConfig();

    if(Config)
    {
        Config->mId = Id;
        Config->mName = Name;
        Config->mAddr = Addr;
        Config->mChan = Chan;
        Config->mZero = Zero;
        Config->mFull = Full;
        Config->mRange = Range;
        Config->mUnit = Unit;
        Config->mAlarmUpper = AlarmUpper;
        Config->mAlarmLower = AlarmLower;
        Config->mDisplayType = DisplayType;
        Config->mCoe = Coe;
        Config->mCycle = Cycle;
        Config->mTime = Time;

        mList.push_back(Config);
    }
}

void DialogConfigGong::fEdit()
{
    int Index = mTable->fGetCurrentIndex();

    if(Index >= 0)
    {
        int Id = mSpinBoxId->value();
        QString Name = mLineEditName->text();
        int Addr = mSpinBoxAddr->value();
        int Chan = mSpinBoxChannel->value();
        double Zero = mSpinBoxZero->value();
        double Full = mSpinBoxFull->value();
        double Range = mSpinBoxRange->value();
        QString Unit = mLineEditUnit->text();
        double AlarmUpper = mSpinBoxAlarmUpper->value();
        double AlarmLower = mSpinBoxAlarmLower->value();
        int DisplayType = mSpinBoxDisplayType->value();
        double Coe = mSpinBoxCoe->value();
        double Cycle = mSpinBoxCycle->value();
        double Time = mSpinBoxTime->value();

        mTable->fSetCell(Index, 0, QString::number(Id));
        mTable->fSetCell(Index, 1, Name);
        mTable->fSetCell(Index, 2, QString::number(Addr));
        mTable->fSetCell(Index, 3, QString::number(Chan));
        mTable->fSetCell(Index, 4, QString::number(Zero));
        mTable->fSetCell(Index, 5, QString::number(Full));
        mTable->fSetCell(Index, 6, QString::number(Range));
        mTable->fSetCell(Index, 7, Unit);
        mTable->fSetCell(Index, 8, QString::number(AlarmUpper));
        mTable->fSetCell(Index, 9, QString::number(AlarmLower));
        mTable->fSetCell(Index, 10, QString::number(DisplayType));
        mTable->fSetCell(Index, 11, QString::number(Coe));
        mTable->fSetCell(Index, 12, QString::number(Cycle));
        mTable->fSetCell(Index, 13, QString::number(Time));

        TGongConfig *Config = mList.at(Index);

        if(Config)
        {
            Config->mId = Id;
            Config->mName = Name;
            Config->mAddr = Addr;
            Config->mChan = Chan;
            Config->mZero = Zero;
            Config->mFull = Full;
            Config->mRange = Range;
            Config->mUnit = Unit;
            Config->mAlarmUpper = AlarmUpper;
            Config->mAlarmLower = AlarmLower;
            Config->mDisplayType = DisplayType;
            Config->mCoe = Coe;
            Config->mCycle = Cycle;
            Config->mTime = Time;

            mList.push_back(Config);
        }
    }
}

void DialogConfigGong::fDel()
{
    int Index = mTable->fGetCurrentIndex();

    qDebug() << "DialogConfigGong::fDel()" << Index;

    if(Index >= 0 && Index < mList.length())
    {
        mTable->fDelRow(Index);

        TGongConfig *Config = mList.at(Index);

        if(Config)
        {
            mList.removeOne(Config);
            delete Config;
        }
    }
}

void DialogConfigGong::fOK()
{
    mSave = true;
    close();
}

void DialogConfigGong::fTableClick(int Row, int Col)
{
    Q_UNUSED(Col);

    //qDebug() << "DialogConfigGong::fTableClick" << Row << Col;

    if(Row >= 0 && Row < mList.length())
    {
        TGongConfig *Config = mList.at(Row);

        if(NULL == Config)
        {
            return;
        }

        //qDebug() << "Config->mChan" << Config->mChan;

        mSpinBoxId->setValue(Config->mId);
        mLineEditName->setText(Config->mName);
        mSpinBoxAddr->setValue(Config->mAddr);
        mSpinBoxChannel->setValue(Config->mChan);
        mSpinBoxZero->setValue(Config->mZero);
        mSpinBoxFull->setValue(Config->mFull);
        mSpinBoxRange->setValue(Config->mRange);
        mLineEditUnit->setText(Config->mUnit);
        mSpinBoxAlarmUpper->setValue(Config->mAlarmUpper);
        mSpinBoxAlarmLower->setValue(Config->mAlarmLower);
        mSpinBoxDisplayType->setValue(Config->mDisplayType);
        mSpinBoxCoe->setValue(Config->mCoe);
        mSpinBoxCycle->setValue(Config->mCycle);
        mSpinBoxTime->setValue(Config->mTime);
    }
}

void DialogConfigGong::fLoad()
{
    QString mFileDir = QFileDialog::getOpenFileName(this, tr("打开"));

    //qDebug() << "mFileDir" << mFileDir;

    if(tr("") == mFileDir)
    {
        return;
    }

    mTable->fClear();

    TGongConfig::fGetConfigFromFile(mFileDir, mList);

    foreach(TGongConfig *Config, mList)
    {
        QList<QString>StrList;
        StrList.push_back(QString::number(Config->mId));
        StrList.push_back(Config->mName);
        StrList.push_back(QString::number(Config->mAddr));
        StrList.push_back(QString::number(Config->mChan));
        StrList.push_back(QString::number(Config->mZero));
        StrList.push_back(QString::number(Config->mFull));
        StrList.push_back(QString::number(Config->mRange));
        StrList.push_back(Config->mUnit);
        StrList.push_back(QString::number(Config->mAlarmUpper));
        StrList.push_back(QString::number(Config->mAlarmLower));
        StrList.push_back(QString::number(Config->mDisplayType));
        StrList.push_back(QString::number(Config->mCoe));
        StrList.push_back(QString::number(Config->mCycle));
        StrList.push_back(QString::number(Config->mTime));
        mTable->fAddRow(StrList);
    }
}

void DialogConfigGong::fOut()
{
    TTableService::fOutToCsv(mTable, this);
}
