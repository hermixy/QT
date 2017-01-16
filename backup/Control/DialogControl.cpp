#include "DialogControl.h"

#include <QDir>
#include <QDebug>

void DialogControl::fInit()
{
    mItem = NULL;

    mGroupBoxItem = NULL;
    mLabelNameShow = NULL;
    mEditName = NULL;

    mGroupBoxWidgets = NULL;
    mLabelServiceShow = NULL;
    mComboBoxService = NULL;
    mLabelJsShow = NULL;
    mComboBoxJs = NULL;
    mLabelSubShow = NULL;
    mTable = NULL;
    mLabelSceneShow = NULL;
    mComboBoxScene = NULL;
    mLabelWidgetsShow = NULL;
    mComboBoxWidgets = NULL;

    mButtonAdd = NULL;
    mButtonDel = NULL;

    mButtonOk = NULL;
    mButtonClose = NULL;
}

void DialogControl::fCreateItem()
{
    if(NULL == mItem)
    {
        return;
    }

    //qDebug() << "DialogControl::fCreateItem()";

    mGroupBoxItem = new QGroupBox(tr(""));
    mLabelNameShow = TBase::fLabel(tr("名称"), tr("mLabelNameShow"));
    mEditName = TBase::fLineEdit(tr("mEditName"));
    //mEditName->setText(mItem->fGetNameId());

    QGridLayout *Layout = new QGridLayout(mGroupBoxItem);

    Layout->addWidget(mLabelNameShow);
    Layout->addWidget(mEditName);
}

void DialogControl::fCreateWidgets()
{
    TWidgets *Widgets = dynamic_cast<TWidgets *>(mItem);

    if(NULL == Widgets)
    {
        return;
    }

    //qDebug() << "DialogControl::fCreateWidgets()";

    mGroupBoxWidgets = new QGroupBox(tr(""));
    mLabelServiceShow = TBase::fLabel(tr("选择服务"), tr("mLabelServerShow"));
    mComboBoxService = TBase::fComboBox(tr("mComboBoxService"));

    foreach(TService *Service, mServiceList)
    {
        mComboBoxService->addItem(Service->fGetName(), QVariant(Service->fGetId()));
    }

    mComboBoxService->setCurrentIndex(mComboBoxService->findData(QVariant(Widgets->fGetServId())));
    mLabelJsShow = TBase::fLabel(tr("脚本"), tr("mLabelJsShow"));
    mComboBoxJs = TBase::fComboBox(tr("mComboBoxJs"));

    QDir mJsDir(QApplication::applicationDirPath() + tr("/js/"));
    QStringList jsFileNames = mJsDir.entryList(QStringList("*.fWidgets"), QDir::Files);
    mComboBoxJs->addItem(tr(""));
    mComboBoxJs->addItems(jsFileNames);
    mComboBoxJs->setCurrentIndex(mComboBoxJs->findText(Widgets->mJsWidgets.fGetName()));

    mLabelSubShow = TBase::fLabel(tr("绑定的子控件"), tr("mLabelSubShow"));
    mTable = new TTableView(tr("mTable"));
    mTable->fAddCol(tr("名称"));
    mTable->fAddCol(tr("类型"));
    for(int i = 0; i < Widgets->fGetWidgetsSum(); i++)
    {
        TItem *Item = Widgets->fGetWidgets(i);

        if(Item)
        {
            QList<QString> RowData;
            //RowData.push_back(Item->fGetNameId());
            RowData.push_back(QString::number(Item->fGetType()));
            mTable->fAddRow(RowData);
        }
    }

    mLabelSceneShow = TBase::fLabel(tr("画布"), tr("mLabelSceneShow"));
    mComboBoxScene = TBase::fComboBox(tr("mComboBoxScene"));
    mComboBoxScene->setMinimumWidth(100);

    foreach(TScene* Scene, mSceneList)
    {
        mComboBoxScene->addItem(Scene->fGetName());
    }

    connect(mComboBoxScene, SIGNAL(currentIndexChanged(int)), this, SLOT(fSceneIndexChanged(int)));

    mLabelWidgetsShow = TBase::fLabel(tr("控件"), tr("mLabelWidgetsShow"));
    mComboBoxWidgets = TBase::fComboBox(tr("mComboBoxWidgets"));
    mComboBoxWidgets->setMinimumWidth(100);

    fSceneIndexChanged(0);

    mButtonAdd = TBase::fButton(tr("添加"), tr("mButtonAdd"));
    connect(mButtonAdd, SIGNAL(clicked()), this, SLOT(fAdd()));
    mButtonDel = TBase::fButton(tr("删除"), tr("mButtonDel"));
    connect(mButtonDel, SIGNAL(clicked()), this, SLOT(fDel()));

    QGridLayout *LayoutTop = new QGridLayout();
    LayoutTop->addWidget(mLabelServiceShow, 0, 0);
    LayoutTop->addWidget(mComboBoxService, 0, 1);
    LayoutTop->addWidget(mLabelJsShow, 1, 0);
    LayoutTop->addWidget(mComboBoxJs, 1, 1);
    LayoutTop->addWidget(mLabelSubShow, 2, 0);
    LayoutTop->addWidget(mTable, 3, 0, 1, 2);

    QGridLayout *LayoutSub = new QGridLayout();
    LayoutSub->addWidget(mLabelSceneShow, 0, 0);
    LayoutSub->addWidget(mComboBoxScene, 0, 1);
    LayoutSub->addWidget(mLabelWidgetsShow, 1, 0);
    LayoutSub->addWidget(mComboBoxWidgets, 1, 1);

    QVBoxLayout *LayoutEdit = new QVBoxLayout();
    LayoutEdit->addStretch();
    LayoutEdit->addWidget(mButtonAdd);
    LayoutEdit->addStretch();
    LayoutEdit->addWidget(mButtonDel);
    LayoutEdit->addStretch();

    QHBoxLayout *LayoutMid = new QHBoxLayout();
    LayoutMid->addStretch();
    LayoutMid->addLayout(LayoutSub);
    LayoutMid->addStretch();
    LayoutMid->addLayout(LayoutEdit);
    LayoutMid->addStretch();

    QVBoxLayout *Layout = new QVBoxLayout(mGroupBoxWidgets);
    Layout->addLayout(LayoutTop);
    Layout->addLayout(LayoutMid);
}

DialogControl::DialogControl(TItem *Item, QList<TScene *> &SceneList,
                             const QList<TService *> &ServiceList, QWidget *parent) : QDialog(parent)
{
    if(NULL == Item)
    {
        return;
    }

    fInit();

    mItem = Item;
    mSceneList = SceneList;
    mServiceList = ServiceList;

    fCreateItem();
    fCreateWidgets();

    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *LayoutMain = new QVBoxLayout(this);

    if(mGroupBoxItem)
    {
        LayoutMain->addWidget(mGroupBoxItem);
    }

    if(mGroupBoxWidgets)
    {
        LayoutMain->addWidget(mGroupBoxWidgets);
    }

    LayoutMain->addSpacing(10);
    LayoutMain->addLayout(mLayoutButton);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(LayoutMain);
    setWindowTitle(tr("控件属性"));
    setFixedSize(sizeHint());
}

void DialogControl::fSceneIndexChanged(int Index)
{
    if(Index < 0 && Index >= mSceneList.length())
    {
        return;
    }

    mComboBoxWidgets->clear();

    QList<TItem *>ItemList;
    mSceneList.at(Index)->fGetItemsList(ItemList);

    foreach(TItem *Item, ItemList)
    {
        //QString NameId = Item->fGetNameId();

        //if(NameId.length() > 0)
        {
            //mComboBoxWidgets->addItem(NameId);
        }
    }
}

void DialogControl::fAdd()
{
    TItem *Item = TScene::fGetItemByNameId(mSceneList, mComboBoxWidgets->currentText());

    if(Item)
    {
        QList<QString> RowData;
        //RowData.push_back(Item->fGetNameId());
        RowData.push_back(QString::number(Item->fGetType()));
        mTable->fAddRow(RowData);
    }
}

void DialogControl::fDel()
{
    mTable->fDelRow(mTable->fGetCurrentIndex());
}

void DialogControl::fApply()
{
    if(mItem)
    {
        //mItem->fSetNameId(mEditName->text());
    }

    TWidgets *Widgets = dynamic_cast<TWidgets *>(mItem);

    if(Widgets)
    {
        Widgets->fSetServId(TBase::fGetComboBoxCurrentData(mComboBoxService));
        Widgets->mJsWidgets.fSetName(mComboBoxJs->currentText());

        Widgets->fClearWidgets();

        for(int i = 0; i < mTable->fGetRowSum(); i++)
        {
            Widgets->fAddWidgets(TScene::fGetItemByNameId(mSceneList, mTable->fGetCell(i, 0)));
        }
    }

    close();
}

DialogControl::~DialogControl()
{}
