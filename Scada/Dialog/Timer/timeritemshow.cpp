#include "timeritemshow.h"
#include "timeritemedit.h"

TimerItemShow::TimerItemShow(QList<TScene::TimerStruct> &timerstruct, QString name, QWidget *parent):QDialog(parent)
{
    this->timerstruct=timerstruct;

    mGroupBoxServiceList = new QGroupBox(tr(""));
    mLabelServiceListShow = TBase::fLabel(name, tr("mLabelServiceListShow"));
    mTableServiceList = new TTableView(tr("mTableServiceList"));
    mTableServiceList->fAddCol(tr("名称Id"));
    mTableServiceList->fAddCol(tr("服务Id"));
    mTableServiceList->fAddCol(tr("主设备号"));
    mTableServiceList->fAddCol(tr("通道号"));
    mTableServiceList->fAddCol(tr("寄存器起始地址"));
    //mTableServiceList->fAddCol(tr("写入数值"));
    mTableServiceList->fSetHeadLength(0, 100);
    mTableServiceList->fSetHeadLength(1, 100);
    mTableServiceList->fSetHeadLength(2, 100);
    mTableServiceList->fSetHeadLength(3, 100);
    mTableServiceList->fSetHeadLength(4, 100);
    //mTableServiceList->fSetHeadLength(5, 100);
    mTableServiceList->setMinimumWidth(600);                //设定最小宽度

    foreach(TScene::TimerStruct Service, timerstruct)
    {
        fAddServiceToTable(Service);
    }

   // connect(mTableServiceList, SIGNAL(mClick(int,int)), this, SLOT(fServiceListChanged(int,int)));

    QVBoxLayout *LayoutTable = new QVBoxLayout(mGroupBoxServiceList);
    LayoutTable->addWidget(mLabelServiceListShow);
    LayoutTable->addWidget(mTableServiceList);

    ///////

    mGroupBoxControl = new QGroupBox(tr(""));
    mButtonAdd = TBase::fButton(tr("添加"), tr("mButtonAdd"));
    mButtonAdd->setFixedWidth(mButtonAdd->sizeHint().height() * 2);
    connect(mButtonAdd, SIGNAL(clicked()), this, SLOT(fAdd()));
    mButtonEdit = TBase::fButton(tr("修改"), tr("mButtonEdit"));
    mButtonEdit->setFixedWidth(mButtonEdit->sizeHint().height() * 2);
    connect(mButtonEdit, SIGNAL(clicked()), this, SLOT(fEdit()));
    mButtonDel = TBase::fButton(tr("删除"), tr("mButtonDel"));
    mButtonDel->setFixedWidth(mButtonDel->sizeHint().height() * 2);
    connect(mButtonDel, SIGNAL(clicked()), this, SLOT(fDel()));

    QVBoxLayout *LayoutControl = new QVBoxLayout(mGroupBoxControl);
    LayoutControl->addWidget(mButtonAdd);
    LayoutControl->addWidget(mButtonEdit);
    LayoutControl->addWidget(mButtonDel);

    ////

    QHBoxLayout *LayoutTop = new QHBoxLayout();
    LayoutTop->addWidget(mGroupBoxServiceList);
    LayoutTop->addWidget(mGroupBoxControl);
    //LayoutTop->addWidget(mGroupBoxServiceMsg);

    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fOk()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonClose);
    mLayoutButton->addStretch();

    QVBoxLayout *LayoutMain = new QVBoxLayout(this);
    LayoutMain->addLayout(LayoutTop);
    LayoutMain->addStretch();
    LayoutMain->addLayout(mLayoutButton);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(LayoutMain);
    setWindowTitle(name);
    //setFixedSize(sizeHint());
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

TimerItemShow::~TimerItemShow()
{

}

void TimerItemShow::fAddServiceToTable(TScene::TimerStruct timerstruct)
{
    QList<QString>RowData;
    RowData.push_back(timerstruct.TimerId);
    RowData.push_back(QString::number(timerstruct.serverId));
    RowData.push_back(QString::number(timerstruct.mainId));
    RowData.push_back(QString::number(timerstruct.devId));
    RowData.push_back(QString::number(timerstruct.startAddr, 16));
    //RowData.push_back(QString::number(timerstruct.writeNum));

    mTableServiceList->fAddRow(RowData);
}
void TimerItemShow::fOk()
{
    mSave = true;
    close();
}
void TimerItemShow::fAdd()
{
    TimerItemEdit *ServiceEdit = new TimerItemEdit(nullptr ,this);

    if(ServiceEdit)
    {
        ServiceEdit->exec();

        if(ServiceEdit->mSave)
        {
            timerstruct.push_back(ServiceEdit->timerstruct);
            fAddServiceToTable(ServiceEdit->timerstruct);
        }

        delete ServiceEdit;
    }
}

void TimerItemShow::fEdit()
{
    int Index = mTableServiceList->fGetCurrentIndex();

    if(Index < 0 || Index >= mTableServiceList->fGetRowSum()
        || Index >= timerstruct.length())
    {
        return;
    }

    TScene::TimerStruct Service = timerstruct.at(Index);

    TimerItemEdit *ServiceEdit = new TimerItemEdit(&Service ,this);

    if(ServiceEdit)
    {
        ServiceEdit->exec();

        if(ServiceEdit->mSave)
        {
            Service=ServiceEdit->timerstruct;
            timerstruct.replace(Index, ServiceEdit->timerstruct);

            // 更新表格
            mTableServiceList->fSetCell(Index, 0, Service.TimerId);
            mTableServiceList->fSetCell(Index, 1, QString::number(Service.serverId));
            mTableServiceList->fSetCell(Index, 2, QString::number(Service.mainId));
            mTableServiceList->fSetCell(Index, 3, QString::number(Service.devId));
            mTableServiceList->fSetCell(Index, 4, QString::number(Service.startAddr, 16));
            //mTableServiceList->fSetCell(Index, 5, QString::number(Service.writeNum));
        }

        delete ServiceEdit;
    }
}

void TimerItemShow::fDel()
{
    int index = mTableServiceList->fGetCurrentIndex();
    if(index<0)
    {
        return;
    }
    timerstruct.removeAt(index);
    mTableServiceList->fDelRow(index);
}
