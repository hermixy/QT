#include "DialogLinkage.h"

#include <QMessageBox>
#include <QDebug>

DialogLinkage::DialogLinkage(const QList<TScene *> &SceneList, const QList<TService *> &ServiceList,
                             QList<TLinkage *> &LinkageList, QWidget *parent) : QDialog(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);

    mSave = false;

    //qDebug() << "DialogLinkage" << "LinkageList" << LinkageList.length();

    foreach(TLinkage *Linkage, LinkageList)
    {
        mLinkageList.push_back(new TLinkage(Linkage));
    }

    if(SceneList.length() <= 0 || ServiceList.length() <= 0)
    {
        mInit = false;
        return;
    }

    mInit = true;

    mLabelTableShow = TBase::fLabel(tr("已添加条件："), tr("mLabelTableShow"));

    mTable = new TTableView(tr("mTabel"));
    mTable->fAddCol(tr("源服务"));
    mTable->fAddCol(tr("源节点"));
    mTable->fAddCol(tr("目标服务"));
    mTable->fAddCol(tr("目标节点"));
    mTable->fSetHeadLength(0, 120);
    mTable->fSetHeadLength(1, 60);
    mTable->fSetHeadLength(2, 120);
    mTable->fSetHeadLength(3, 60);
    mTable->fUpdateFixWidth();

    foreach(TLinkage *Linkage, mLinkageList)
    {
        QList<QString> RowData;

        bool FindServiceName = false;
        foreach(TService *Service, ServiceList)
        {
            if(Service->fGetId() == Linkage->mSrcServiceId)
            {
                // 找到了服务名称
                FindServiceName = true;
                RowData.push_back(Service->fGetName());
            }
        }

        // 未找到服务名称
        if(!FindServiceName)
        {
            RowData.push_back(tr("未知ID:") + QString::number(Linkage->mSrcServiceId));
        }

        RowData.push_back(QString::number(Linkage->mSrcNodeId));

        /*if(Linkage->mSrcJs)
        {
            RowData.push_back(Linkage->mSrcJs->fGetName());
        }
        else
        {
            RowData.push_back(tr("未知"));
        }*/

        foreach(TService *Service, ServiceList)
        {
            if(Service->fGetId() == Linkage->mDstServiceId)
            {
                // 找到了服务名称
                FindServiceName = true;
                RowData.push_back(Service->fGetName());
            }
        }

        // 未找到服务名称
        if(!FindServiceName)
        {
            RowData.push_back(tr("未知ID:") + QString::number(Linkage->mDstServiceId));
        }

        RowData.push_back(QString::number(Linkage->mDstNodeId));

        /*if(Linkage->mDstJs)
        {
            RowData.push_back(Linkage->mDstJs->fGetName());
        }
        else
        {
            RowData.push_back(tr("未知"));
        }*/

        mTable->fAddRow(RowData);
    }

    connect(mTable, SIGNAL(mClick(int,int)), this, SLOT(fTableClick(int,int)));

    //qDebug() << "src";

    // src
    mLabelSrcServiceShow = TBase::fLabel(tr("源服务"), tr("mLabelSrcServiceShow"));
    mComboBoxSrcService = TBase::fComboBox(tr("mComboBoxSrcService"));
    mLabelSrcNodeId = TBase::fLabel(tr("源节点"), tr("mLabelSrcNodeId"));
    mComboBoxSrcNodeId = TBase::fComboBox(tr("mComboBoxSrcNodeId"));
    mLabelSrcNodeIdText = TBase::fLabel(tr("设置动画中指定了地址的节点"), tr("mLabelSrcNodeIdText"));

    //qDebug() << "dst";

    // dst
    mLabelDstServiceShow = TBase::fLabel(tr("目标服务"), tr("mLabelDstServiceShow"));
    mComboBoxDstService = TBase::fComboBox(tr("mComboBoxDstService"));
    mLabelDstNodeId = TBase::fLabel(tr("目标节点"), tr("mLabelDstNodeId"));
    mComboBoxDstNodeId = TBase::fComboBox(tr("mComboBoxDstNodeId"));
    mLabelDstNodeIdText = TBase::fLabel(tr("设置动画中指定了地址的节点"), tr("mLabelDstNodeIdText"));

    //qDebug() << "ServiceList";
    foreach(TService *Service, ServiceList)
    {
        mComboBoxSrcService->addItem(Service->fGetName(), QVariant(Service->fGetId()));
        mComboBoxDstService->addItem(Service->fGetName(), QVariant(Service->fGetId()));
    }

    //qDebug() << "SceneList";
    foreach(TScene *Scene, SceneList)
    {
        QList<TGraphics *>GraphicsList;
        Scene->fGetItemsList(GraphicsList);

        foreach(TGraphics *Graphics, GraphicsList)
        {
            int NodeId = Graphics->fGetNodeId();

            if(NodeId > 0)
            {
                mComboBoxSrcNodeId->addItem(QString::number(NodeId));
                mComboBoxDstNodeId->addItem(QString::number(NodeId));
            }
        }
    }

    //增改删
    mButtonAdd = TBase::fButton(tr("增加"), tr("mButtonAdd"), 50, 24);
    connect(mButtonAdd, SIGNAL(clicked()), this, SLOT(fAdd()));
    mButtonEdit = TBase::fButton(tr("编辑"), tr("mButtonEdit"), 50, 24);
    connect(mButtonEdit, SIGNAL(clicked()), this, SLOT(fEdit()));
    mButtonDel = TBase::fButton(tr("删除"), tr("mButtonDel"), 50, 24);
    connect(mButtonDel, SIGNAL(clicked()), this, SLOT(fDel()));

    // 确定取消
    mButtonOk = TBase::fButton(tr("确定"), tr("mPushButtonOK"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fOkClicked()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mPushButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mLayoutTable = new QVBoxLayout();
    mLayoutTable->addWidget(mLabelTableShow, 0, Qt::AlignLeft);
    mLayoutTable->addWidget(mTable);

    QGridLayout *mLayoutEditMsg = new QGridLayout();
    mLayoutEditMsg->addWidget(mLabelSrcServiceShow, 0, 0);
    mLayoutEditMsg->addWidget(mComboBoxSrcService, 0, 1, 1, 2);
    mLayoutEditMsg->addWidget(mLabelSrcNodeId, 1, 0);
    mLayoutEditMsg->addWidget(mComboBoxSrcNodeId, 1, 1);
    mLayoutEditMsg->addWidget(mLabelSrcNodeIdText, 1, 2);
    mLayoutEditMsg->addWidget(mLabelDstServiceShow, 2, 0);
    mLayoutEditMsg->addWidget(mComboBoxDstService, 2, 1, 1, 2);
    mLayoutEditMsg->addWidget(mLabelDstNodeId, 3, 0);
    mLayoutEditMsg->addWidget(mComboBoxDstNodeId, 3, 1);
    mLayoutEditMsg->addWidget(mLabelDstNodeIdText, 3, 2);

    QVBoxLayout *mLayoutEditButton = new QVBoxLayout();
    mLayoutEditButton->addWidget(mButtonAdd);
    mLayoutEditButton->addWidget(mButtonEdit);
    mLayoutEditButton->addWidget(mButtonDel);

    QHBoxLayout *mLayoutEdit = new QHBoxLayout();
    mLayoutEdit->addStretch();
    mLayoutEdit->addLayout(mLayoutEditMsg);
    mLayoutEdit->addStretch();
    mLayoutEdit->addLayout(mLayoutEditButton);
    mLayoutEdit->addStretch();

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addStretch();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addSpacing(20);
    mLayoutButton->addWidget(mButtonClose);
    mLayoutButton->addStretch();

    QVBoxLayout *mLayoutMain = new QVBoxLayout();
    mLayoutMain->addLayout(mLayoutTable);
    mLayoutMain->addLayout(mLayoutEdit);
    mLayoutMain->addLayout(mLayoutButton);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("设置联动"));
    setFixedSize(sizeHint());
}

DialogLinkage::~DialogLinkage()
{
    while(mLinkageList.count() > 0)
    {
        TLinkage *Linkage = mLinkageList.first();
        mLinkageList.removeFirst();

        delete Linkage;
    }

    if(mTable)
    {
        mTable->fClear();
    }
}

bool DialogLinkage::fCheckMsg()
{
    if(mComboBoxSrcService->currentIndex() < 0
            || mComboBoxSrcNodeId->currentIndex() < 0)
    {
        return false;
    }

    if(mComboBoxDstService->currentIndex() < 0
            || mComboBoxDstNodeId->currentIndex() < 0)
    {
        return false;
    }

    return true;
}

void DialogLinkage::fAdd()
{
    if(fCheckMsg())
    {
        QList<QString> RowData;
        RowData.push_back(mComboBoxSrcService->currentText());
        RowData.push_back(mComboBoxSrcNodeId->currentText());
        //RowData.push_back(mComboBoxSrcJs->currentText());
        RowData.push_back(mComboBoxDstService->currentText());
        RowData.push_back(mComboBoxDstNodeId->currentText());
        //RowData.push_back(mComboBoxDstJs->currentText());

        mTable->fAddRow(RowData);

        TLinkage *Linkage = new TLinkage();

        if(Linkage)
        {
            Linkage->mSrcServiceId = TBase::fGetComboBoxCurrentData(mComboBoxSrcService);
            Linkage->mSrcNodeId = mComboBoxSrcNodeId->currentText().toInt();

            /*if(Linkage->mSrcJs)
            {
                Linkage->mSrcJs->fSetDir(QApplication::applicationDirPath());
                Linkage->mSrcJs->fSetName(mComboBoxSrcJs->currentText());
            }*/

            Linkage->mDstServiceId = TBase::fGetComboBoxCurrentData(mComboBoxDstService);
            Linkage->mDstNodeId = mComboBoxDstNodeId->currentText().toInt();

            /*if(Linkage->mDstJs)
            {
                Linkage->mDstJs->fSetDir(QApplication::applicationDirPath());
                Linkage->mDstJs->fSetName(mComboBoxDstJs->currentText());
            }*/

            mLinkageList.push_back(Linkage);
        }
    }
}

void DialogLinkage::fEdit()
{
    int Index = mTable->fGetCurrentIndex();

    if(Index >= 0 && Index < mLinkageList.length())
    {
        TLinkage *Linkage = mLinkageList.at(Index);

        if(Linkage)
        {
            Linkage->mSrcServiceId = TBase::fGetComboBoxCurrentData(mComboBoxSrcService);
            Linkage->mSrcNodeId = mComboBoxSrcNodeId->currentText().toInt();

            Linkage->mDstServiceId = TBase::fGetComboBoxCurrentData(mComboBoxDstService);
            Linkage->mDstNodeId = mComboBoxDstNodeId->currentText().toInt();

            // 更新表格
            mTable->fSetCell(Index, 0, mComboBoxSrcService->currentText());
            mTable->fSetCell(Index, 1, mComboBoxSrcNodeId->currentText());
            mTable->fSetCell(Index, 2, mComboBoxDstService->currentText());
            mTable->fSetCell(Index, 3, mComboBoxDstNodeId->currentText());
        }
    }
}

void DialogLinkage::fDel()
{
    int Index = mTable->fGetCurrentIndex();

    if(Index >= 0 && Index < mLinkageList.length())
    {
        //qDebug() << "DialogLinkage::fDel" << mTable->fCurrentIndex();
        TLinkage *Linkage = mLinkageList.at(Index);
        if(Linkage)
        {
            mLinkageList.removeOne(Linkage);
            delete Linkage;
        }

        mTable->fDelRow(Index);
    }
}

void DialogLinkage::fTableClick(int Row, int Col)
{
    Q_UNUSED(Col);

    if(Row >= 0 && Row < mLinkageList.length())
    {
        TLinkage *Linkage = mLinkageList.at(Row);

        if(Linkage)
        {
            mComboBoxSrcService->setCurrentIndex(mComboBoxSrcService->findData(QVariant(Linkage->mSrcServiceId)));
            mComboBoxSrcNodeId->setCurrentIndex(mComboBoxSrcNodeId->findText(QString::number(Linkage->mSrcNodeId)));

            mComboBoxDstService->setCurrentIndex(mComboBoxDstService->findData(QVariant(Linkage->mDstServiceId)));
            mComboBoxDstNodeId->setCurrentIndex(mComboBoxDstNodeId->findText(QString::number(Linkage->mDstNodeId)));
        }
    }
}

void DialogLinkage::fOkClicked()
{
    mSave = true;
    close();
}

