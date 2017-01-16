#include "DialogPlay.h"

#include <QDir>
#include <QDebug>

void DialogPlay::fInit()
{
    // 配置
    mGroupBoxConfig = NULL;
    mLabelNameIdShow = NULL;
    mLineEditNameId = NULL;
    mLabelServiceShow = NULL;
    mComboBoxService = NULL;
    mLabelHintShow = NULL;
    mTextEditHint = NULL;
    mLabelUserDataName1Show = NULL;
    mLineEditUserDataName1 = NULL;
    mSpinBoxUserData1 = NULL;
    mLabelUserDataName2Show = NULL;
    mLineEditUserDataName2 = NULL;
    mSpinBoxUserData2 = NULL;

    // 控件
    mGroupBoxWidgets = NULL;
    mLabelWidgetsJsShow = NULL;
    mComboBoxWidgetsJs = NULL;

    // 图形
    mGroupBoxGraphics = NULL;
    mLabelValueShow = NULL;
    mSpinBoxValue = NULL;
    mLabelCoeShow = NULL;
    mSpinBoxCoe = NULL;

    // 值脚本
    mLabelNodeIdShow = NULL;
    mSpinBoxNodeId = NULL;
    mLabelNodeIdText = NULL;
    mLabelPlayJsShow = NULL;
    mComboBoxPlayJs = NULL;

    // 自动
    mLabelAutoShow = NULL;
    mLabelAutoTimeShow = NULL;
    mSpinBoxAutoTime = NULL;

    // 窗口
    mButtonOk = NULL;
    mButtonClose = NULL;
}

bool DialogPlay::fCreateItemEx(const QList<TService *> &ServiceList)
{
    TItemEx *ItemEx = dynamic_cast<TItemEx *>(mItem);

    if(NULL == ItemEx)
    {
        return false;
    }

    // 配置
    mGroupBoxConfig = new QGroupBox(tr(""));
    mLabelNameIdShow = TBase::fLabel(tr("名称"), tr("mLabelNameIdShow"));
    mLineEditNameId = TBase::fLineEdit(tr("mLineEditNameId"));
    mLineEditNameId->setText(ItemEx->fGetNameId());
    mLabelServiceShow = TBase::fLabel(tr("选择服务"), tr("mLabelServerShow"));
    mComboBoxService = TBase::fComboBox(tr("mComboBoxService"));

    foreach(TService *Service, ServiceList)
    {
        mComboBoxService->addItem(Service->fGetName(), QVariant(Service->fGetId()));
    }

    mComboBoxService->setCurrentIndex(mComboBoxService->findData(QVariant(ItemEx->fGetServiceId())));

    mLabelHintShow = TBase::fLabel(tr("提示信息"), tr("mLabelHintShow"));
    mTextEditHint = TBase::fTextEdit(tr("mTextEditHint"));
    mTextEditHint->setMaximumHeight(mLabelHintShow->sizeHint().height() * 2);
    mTextEditHint->setText(ItemEx->fGetHint());

    mLabelUserDataName1Show = TBase::fLabel(tr("配置1"), tr("mLabelUserDataName1Show"));
    mLineEditUserDataName1 = TBase::fLineEdit(tr("mLineEditUserDataName1"));
    mSpinBoxUserData1 = TBase::fDoubleSpinBox(tr("mSpinBoxUserData1"), -102400.0, 102400.0);
    mLabelUserDataName2Show = TBase::fLabel(tr("配置2"), tr("mLabelUserDataName2Show"));
    mLineEditUserDataName2 = TBase::fLineEdit(tr("mLineEditUserDataName2"));
    mSpinBoxUserData2 = TBase::fDoubleSpinBox(tr("mSpinBoxUserData2"), -102400.0, 102400.0);

    mLineEditUserDataName1->setText(ItemEx->fGetUserDataName1());
    mSpinBoxUserData1->setValue(ItemEx->fGetUserData1());
    mLineEditUserDataName2->setText(ItemEx->fGetUserDataName2());
    mSpinBoxUserData2->setValue(ItemEx->fGetUserData2());

    QGridLayout *LayoutConfig = new QGridLayout();
    LayoutConfig->addWidget(mLabelNameIdShow, 0, 0);
    LayoutConfig->addWidget(mLineEditNameId, 0, 1, 1, 2);
    LayoutConfig->addWidget(mLabelServiceShow, 1, 0);
    LayoutConfig->addWidget(mComboBoxService, 1, 1, 1, 2);
    LayoutConfig->addWidget(mLabelHintShow, 2, 0);
    LayoutConfig->addWidget(mTextEditHint, 2, 1, 1, 2);
    LayoutConfig->addWidget(mLabelUserDataName1Show, 3, 0);
    LayoutConfig->addWidget(mLineEditUserDataName1, 3, 1);
    LayoutConfig->addWidget(mSpinBoxUserData1, 3, 2);
    LayoutConfig->addWidget(mLabelUserDataName2Show, 4, 0);
    LayoutConfig->addWidget(mLineEditUserDataName2, 4, 1);
    LayoutConfig->addWidget(mSpinBoxUserData2, 4, 2);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxConfig);
    LayoutTotal->addLayout(LayoutConfig);
    LayoutTotal->addStretch();

    return true;
}

bool DialogPlay::fCreateWidgets()
{
    TWidgets *Widgets = dynamic_cast<TWidgets *>(mItem);

    if(NULL == Widgets)
    {
        return false;
    }

    //qDebug() << "Widgets" << (void *)Widgets;

    mGroupBoxWidgets = new QGroupBox(tr(""));
    mLabelWidgetsJsShow = TBase::fLabel(tr("执行脚本"), tr("mLabelWidgetsJsShow"));
    mComboBoxWidgetsJs = TBase::fComboBox(tr("mComboBoxWidgetsJs"));

    QDir mJsDir(QApplication::applicationDirPath() + tr("/js/"));
    QStringList jsFileNames = mJsDir.entryList(QStringList("*.fWidgets"), QDir::Files);

    mComboBoxWidgetsJs->addItem(tr(""));
    mComboBoxWidgetsJs->addItems(jsFileNames);

    //qDebug() << "Widgets->mJsWidgets.fGetName()" << Widgets->mJsWidgets.fGetName();

    mComboBoxWidgetsJs->setCurrentIndex(mComboBoxWidgetsJs->findText(Widgets->mJsWidgets.fGetName()));

    QGridLayout *LayoutWidgets = new QGridLayout();
    LayoutWidgets->addWidget(mLabelWidgetsJsShow, 0, 0);
    LayoutWidgets->addWidget(mComboBoxWidgetsJs, 0, 1);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxWidgets);
    LayoutTotal->addLayout(LayoutWidgets);
    LayoutTotal->addStretch();

    return true;
}

bool DialogPlay::fCreateGraphics()
{
    TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);

    if(NULL == Graphics)
    {
        return false;
    }

    mGroupBoxGraphics = new QGroupBox(tr(""));
    mLabelValueShow = TBase::fLabel(tr("默认值"), tr("mLabelValueShow"));
    mSpinBoxValue = TBase::fDoubleSpinBox(tr("mSpinBoxValue"), -102400.0, 102400.0);
    mSpinBoxValue->setDecimals(6);
    mSpinBoxValue->setValue(Graphics->fGetValue());
    mLabelCoeShow = TBase::fLabel(tr("系数"), tr("mLabelCoeShow"));
    mSpinBoxCoe = TBase::fDoubleSpinBox(tr("mSpinBoxCoe"), -102400.0, 102400.0);
    mSpinBoxCoe->setDecimals(6);
    mSpinBoxCoe->setValue(Graphics->fGetCoe());

    mLabelNodeIdShow = TBase::fLabel(tr("设备地址"), tr("mLabelNodeIdShow"));
    mSpinBoxNodeId = TBase::fSpinBox(tr("mSpinBoxNodeId"), 0, 102400000);
    mSpinBoxNodeId->setValue(Graphics->fGetNodeId());
    mLabelNodeIdText = TBase::fLabel(tr("(0 表示无动画)"), tr("mLabelNodeIdText"));

    mLabelPlayJsShow = TBase::fLabel(tr("值脚本"), tr("mLabelPlayJsShow"));
    mComboBoxPlayJs = TBase::fComboBox(tr("mComboBoxPlayJs"));

    QDir mJsDir(QApplication::applicationDirPath() + tr("/js/"));
    QStringList jsFileNames = mJsDir.entryList(QStringList("*.fPlay"), QDir::Files);

    mComboBoxPlayJs->addItem(tr(""));
    mComboBoxPlayJs->addItems(jsFileNames);

    /*TJs *JsPlay = Graphics->fGetJs(TGraphics::JS_VALUE);

    if(JsPlay)
    {
        mComboBoxPlayJs->setCurrentIndex(mComboBoxPlayJs->findText(JsPlay->fGetName()));
    }

    mLabelAutoShow = TBase::fLabel(tr("定时脚本"), tr("mLabelAutoShow"));
    mLabelAutoTimeShow = TBase::fLabel(tr("自动间隔(ms)"), tr("mLabelAutoTimeShow"));
    mSpinBoxAutoTime = TBase::fSpinBox(tr("mSpinBoxAutoTime"), 0, 100000000, 75, 22);
    mSpinBoxAutoTime->setValue(Graphics->fGetAutoTime());
    mComboBoxAutoJs = TBase::fComboBox(tr("mComboBoxAutoJs"));
    jsFileNames = mJsDir.entryList(QStringList("*.fAuto"), QDir::Files);
    mComboBoxAutoJs->addItem(tr(""));
    mComboBoxAutoJs->addItems(jsFileNames);

    TJs *JsAuto = Graphics->fGetJs(TGraphics::JS_AUTO);

    if(JsAuto)
    {
        mComboBoxAutoJs->setCurrentIndex(mComboBoxAutoJs->findText(JsAuto->fGetName()));
    }
*/
    QGridLayout *mLayoutGraphics = new QGridLayout();
    mLayoutGraphics->addWidget(mLabelValueShow, 0, 0);
    mLayoutGraphics->addWidget(mSpinBoxValue, 0, 1, 1, 2);
    mLayoutGraphics->addWidget(mLabelCoeShow, 1, 0);
    mLayoutGraphics->addWidget(mSpinBoxCoe, 1, 1, 1, 2);
    mLayoutGraphics->addWidget(mLabelNodeIdShow, 2, 0);
    mLayoutGraphics->addWidget(mSpinBoxNodeId, 2, 1);
    mLayoutGraphics->addWidget(mLabelNodeIdText, 2, 2);
    mLayoutGraphics->addWidget(mLabelPlayJsShow, 3, 0);
    mLayoutGraphics->addWidget(mComboBoxPlayJs, 3, 1, 1, 2);

    mLayoutGraphics->addWidget(mLabelAutoShow, 4, 0);
    mLayoutGraphics->addWidget(mSpinBoxAutoTime, 4, 1);
    mLayoutGraphics->addWidget(mLabelAutoTimeShow, 4, 2);
    mLayoutGraphics->addWidget(mComboBoxAutoJs, 5, 1, 1, 2);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxGraphics);
    LayoutTotal->addLayout(mLayoutGraphics);
    LayoutTotal->addStretch();

    return true;
}

bool DialogPlay::fSaveItemEx()
{
    TItemEx *ItemEx = dynamic_cast<TItemEx *>(mItem);

    if(NULL == ItemEx)
    {
        return false;
    }

    ItemEx->fSetNameId(mLineEditNameId->text());
    ItemEx->fSetServiceId(TBase::fGetComboBoxCurrentData(mComboBoxService));
    ItemEx->fSetHint(mTextEditHint->toPlainText());

    ItemEx->fSetUserDataName1(mLineEditUserDataName1->text());
    ItemEx->fSetUserData1(mSpinBoxUserData1->value());
    ItemEx->fSetUserDataName2(mLineEditUserDataName2->text());
    ItemEx->fSetUserData2(mSpinBoxUserData2->value());

    return true;
}

bool DialogPlay::fSaveWidgets()
{
    TWidgets *Widgets = dynamic_cast<TWidgets *>(mItem);

    if(NULL == Widgets)
    {
        return false;
    }

    Widgets->mJsWidgets.fSetName(mComboBoxWidgetsJs->currentText());

    return true;
}

bool DialogPlay::fSaveGraphics()
{
    TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);

    if(NULL == Graphics)
    {
        return false;
    }

    Graphics->fSetValue(mSpinBoxValue->value());
    Graphics->fSetCoe(mSpinBoxCoe->value());
    Graphics->fSetNodeId(mSpinBoxNodeId->value());

    /*TJs *JsValue = Graphics->fCreateJs(TGraphics::JS_VALUE);

    if(JsValue)
    {
        JsValue->fSetName(mComboBoxPlayJs->currentText());
    }

    Graphics->fSetAutoTime(mSpinBoxAutoTime->value());
    TJs *JsAuto = Graphics->fCreateJs(TGraphics::JS_AUTO);

    if(JsAuto)
    {
        JsAuto->fSetName(mComboBoxAutoJs->currentText());
    }
*/
    return true;
}

DialogPlay::DialogPlay(TItem *Item, const QList<TService *> &ServiceList, QWidget *parent) : QDialog(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);

    mItem = Item;

    if(NULL == Item)
    {
        mInit = false;
        return;
    }

    mInit = true;

    fInit();
    fCreateItemEx(ServiceList);
    fCreateWidgets();
    fCreateGraphics();

    mButtonOk = TBase::fButton(tr("确定"), tr("mPushButtonOK"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fOkClicked()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mPushButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOk);
    mLayoutButton->addSpacing(20);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new QVBoxLayout();

    QTabWidget *TabWidget = new QTabWidget();

    if(mGroupBoxConfig)
    {
        TabWidget->addTab(mGroupBoxConfig, tr("扩展"));
    }

    if(mGroupBoxWidgets)
    {
        TabWidget->addTab(mGroupBoxWidgets, tr("控件"));
    }

    if(mGroupBoxGraphics)
    {
        TabWidget->addTab(mGroupBoxGraphics, tr("图形"));
    }

    mLayoutMain->addWidget(TabWidget);
    mLayoutMain->addLayout(mLayoutButton);

    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("设置图形属性"));
    setFixedSize(sizeHint());
}

DialogPlay::~DialogPlay()
{}

void DialogPlay::fOkClicked()
{
    fSaveItemEx();
    fSaveWidgets();
    fSaveGraphics();

    close();
}

