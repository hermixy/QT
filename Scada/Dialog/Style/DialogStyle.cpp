#include "DialogStyle.h"

#include "../Scada/Src/Graphics/GraphicsPub.h"
#include "../Scada/Src/Widgets/WidgetsPub.h"
#include "modorder.h"

#ifdef DEF_RUN_IN_EMBEDDED
#include <QDesktopWidget>
#endif

#include <QDebug>

void DialogStyle::fInit()
{
    mItem = NULL;

    //坐标
    mGroupBoxPos = NULL;
    //mLayoutPos = NULL;
    mLabelStartX = NULL;
    mLabelStartY = NULL;
    mlabelStopX = NULL;
    mLabelStopY = NULL;
    mLabelCenterX = NULL;
    mSpinBoxCenterX = NULL;
    mLabelCenterY = NULL;
    mSpinBoxCenterY = NULL;
    mLabelZ = NULL;
    mSpinBoxStartX = NULL;
    mSpinBoxStartY = NULL;
    mSpinBoxStopX = NULL;
    mSpinBoxStopY = NULL;
    mSpinBoxZ = NULL;
    mLabelRotateAngleShow = NULL;
    mSpinBoxRotateAngle = NULL;
    mLabelRotateXShow = NULL;
    mSpinBoxRotateX = NULL;
    mLabelRotateYShow = NULL;
    mSpinBoxRotateY = NULL;

    //样式
    mGroupBoxStyle = NULL;
    //mLayoutStyle = NULL;
    mLabelLineStyleShow = NULL;
    mComboBoxLineStyle = NULL;
    mLabelLineWidthShow = NULL;
    mLabelLineWidth = NULL;
    mSpinBoxLineWidth = NULL;
    mLabelLineColorShow = NULL;
    mLabelLineColor = NULL;
    mButtonLineColor = NULL;
    mLabelBackColorShow = NULL;
    mLabelBackColor = NULL;
    mButtonBackColor = NULL;
    mLabelLineAlpha = NULL;
    mSpinBoxLineAlpha = NULL;
    mLabelBackAlpha = NULL;
    mSpinBoxBackAlpha = NULL;
    mLinkList=NULL;

    // 文字
    mGroupBoxText = NULL;
    //mLayoutText = NULL;
    mLabelTextShow = NULL;
    mTextEditText = NULL;
    mLabelFontFamilyShow = NULL;
    mComboBoxFontFamily = NULL;
    mLabelFontSizeShow = NULL;
    mSpinBoxFontSize = NULL;
    mLabelSpacingShow = NULL;
    mSpinBoxSpacing = NULL;
    mCheckBoxFontBlob = NULL;
    mCheckBoxFontItalic = NULL;
    mCheckBoxFontUnderLine = NULL;
    mCheckBoxFontStrikeOut = NULL;

    // 值文字
    mGroupBoxValueText = NULL;
    //mLayoutValueText = NULL;
    mLabelModeShow = NULL;
    mComboBoxTextMode = NULL;
    mLabelIntLenShow = NULL;
    mSpinBoxTextIntLen = NULL;
    mLabelPointLenShow = NULL;
    mSpinBoxTextPointLen = NULL;

    // 图片
    mGroupBoxPixmap = NULL;
    //mLayoutPixmap = NULL;
    mLabelPixmapDirShow = NULL;
    mLineEditPixmapDir = NULL;
    mButtonPixmap = NULL;

    // 角度
    mGroupBoxAngle = NULL;
    //mLayoutAngle = NULL;
    mLabelArcMode = NULL;
    mComboBoxArcMode = NULL;
    mLabelStartAngleShow = NULL;
    mSpinBoxStartAngle = NULL;
    mLabelCoverAngleShow = NULL;
    mSpinBoxCoverAngle = NULL;

    // 表格
    mGroupBoxTable = NULL;
    mTableView = NULL;
    mLabelShow = NULL;
    mLabelTableRowShow = NULL;
    mSpinBoxTableRow = NULL;
    mLabelTableRowLenShow = NULL;
    mSpinBoxRowLen = NULL;
    mLabelTableColShow = NULL;
    mSpinBoxTableCol = NULL;
    mLabelShow = NULL;

    // 按钮
    mGroupBoxButton = NULL;
    mLabelButtonTextShow = NULL;
    mEditButtonText = NULL;

    // 仪表盘
    mGroupBoxMeter = NULL;
    mLabelMeterModeShow = NULL;
    mComboBoxMeterMode = NULL;
    mLabelScaleSumShow = NULL;
    mSpinBoxScaleSum = NULL;
    mLabelMeterStartAngleShow = NULL;
    mSpinBoxMeterStartAngle = NULL;
    mLabelMeterStepAngleShow = NULL;
    mSpinBoxMeterStepAngle = NULL;
    mLabelMeterStartValueShow = NULL;
    mSpinBoxMeterStartValue = NULL;
    mLabelMeterStepValueShow = NULL;
    mSpinBoxMeterStepValue = NULL;
    mLabelMeterPointerColorShow = NULL;
    mLabelMeterPointerColor = NULL;
    mButtonMeterPointColor = NULL;

    // 进度条
    mGroupBoxProcessBar = NULL;
    mLabelProcessBarModeShow = NULL;
    mComboBoxProcessBarMode = NULL;
    mLabelProcessBarScaleSumShow = NULL;
    mSpinBoxProcessBarScaleSum = NULL;
    mLabelProcessBarStartValueShow = NULL;
    mSpinBoxProcessBarStartValue = NULL;
    mLabelProcessBarStepValueShow = NULL;
    mSpinBoxProcessBarStepValue = NULL;
    mLabelProcessBarValueColorShow = NULL;
    mLabelProcessBarValueColor = NULL;
    mButtonProcessBarValueColor = NULL;

    // 窗口
    mButtonOK = NULL;
    mButtonCancel = NULL;
}

bool DialogStyle::fCreateItem(const QList<TService *> &ServiceList)
{
    TItem *Item = mItem;

    if(NULL == Item)
    {
        return false;
    }

    // 坐标
    //mGroupBoxPos = new QGroupBox(tr("<<< 位置 >>>"));
    mGroupBoxPos = new QGroupBox(tr(""));
    mLabelStartX = TBase::fLabel(tr("起点X"), tr("mLabelStartX"));
    mSpinBoxStartX = TBase::fDoubleSpinBox(tr("mSpinBoxStartX"), -10240.0f, 10240.0f);
    mSpinBoxStartX->setValue(mItem->fGetStartX());
    mLabelStartY = TBase::fLabel(tr("起点Y"), tr("mLabelStartY"));
    mSpinBoxStartY = TBase::fDoubleSpinBox(tr("mSpinBoxStartY"), -10240.0f, 10240.0f);
    mSpinBoxStartY->setValue(mItem->fGetStartY());
    mlabelStopX = TBase::fLabel(tr("终点X"), tr("mlabelStopX"));
    mSpinBoxStopX = TBase::fDoubleSpinBox(tr("mSpinBoxStopX"), -10240.0f, 10240.0f);
    mSpinBoxStopX->setValue(mItem->fGetStopX());
    mLabelStopY = TBase::fLabel(tr("终点Y"), tr("mLabelStopY"));
    mSpinBoxStopY = TBase::fDoubleSpinBox(tr("mSpinBoxStopY"), -10240.0f, 10240.0f);
    mSpinBoxStopY->setValue(mItem->fGetStopY());

    // 中心
    mLabelCenterX = TBase::fLabel(tr("中心X"), tr("mLabelCenterX"));
    mSpinBoxCenterX = TBase::fDoubleSpinBox(tr("mSpinBoxCenterX"), -10240.0f, 10240.0f);
    mSpinBoxCenterX->setValue(Item->fGetCurrentCenterX());
    mLabelCenterY = TBase::fLabel(tr("中心Y"), tr("mLabelCenterY"));
    mSpinBoxCenterY = TBase::fDoubleSpinBox(tr("mSpinBoxCenterY"), -10240.0f, 10240.0f);
    mSpinBoxCenterY->setValue(Item->fGetCurrentCenterY());

    mLabelZ = TBase::fLabel(tr("图层Z"), tr("mLabelZ"));
    mSpinBoxZ = TBase::fSpinBox(tr("mSpinBoxZ"), -10240, 10240);
    mSpinBoxZ->setValue(mItem->fGetZValue());
    mSpinBoxZ->setMinimumSize(mSpinBoxStopX->sizeHint());
    mLabelRotateAngleShow = TBase::fLabel(tr("旋转角度"), tr("mLabelRotateAngleShow"));
    mSpinBoxRotateAngle = TBase::fSpinBox(tr("mSpinBoxRotateAngle"), 0, 360);
    mSpinBoxRotateAngle->setValue(mItem->fGetRotateAngle());
    mSpinBoxRotateAngle->setMinimumSize(mSpinBoxStopX->sizeHint());
    mLabelRotateXShow = TBase::fLabel(tr("旋转中心X"), tr("mLabelRotateXShow"));
    mSpinBoxRotateX = TBase::fDoubleSpinBox(tr("mSpinBoxRotateX"), -10240.0f, 10240.0f);
    mSpinBoxRotateX->setValue(Item->fGetRotateCenterX());
    mLabelRotateYShow = TBase::fLabel(tr("旋转中心Y"), tr("mLabelRotateYShow"));
    mSpinBoxRotateY = TBase::fDoubleSpinBox(tr("mSpinBoxRotateY"), -10240.0f, 10240.0f);
    mSpinBoxRotateY->setValue(Item->fGetRotateCenterY());

    QGridLayout *LayoutPos = new QGridLayout();
    LayoutPos->addWidget(mLabelStartX, 0, 0);
    LayoutPos->addWidget(mSpinBoxStartX, 0, 1);
    LayoutPos->addWidget(mLabelStartY, 0, 2);
    LayoutPos->addWidget(mSpinBoxStartY, 0, 3);
    LayoutPos->addWidget(mlabelStopX, 1, 0);
    LayoutPos->addWidget(mSpinBoxStopX, 1, 1);
    LayoutPos->addWidget(mLabelStopY, 1, 2);
    LayoutPos->addWidget(mSpinBoxStopY, 1, 3);
    LayoutPos->addWidget(mLabelCenterX, 2, 0);
    LayoutPos->addWidget(mSpinBoxCenterX, 2, 1);
    LayoutPos->addWidget(mLabelCenterY, 2, 2);
    LayoutPos->addWidget(mSpinBoxCenterY, 2, 3);
    LayoutPos->addWidget(mLabelZ, 3, 0);
    LayoutPos->addWidget(mSpinBoxZ, 3, 1);
    LayoutPos->addWidget(mLabelRotateAngleShow, 3, 2);
    LayoutPos->addWidget(mSpinBoxRotateAngle, 3, 3);
    LayoutPos->addWidget(mLabelRotateXShow, 4, 0);
    LayoutPos->addWidget(mSpinBoxRotateX, 4, 1);
    LayoutPos->addWidget(mLabelRotateYShow, 4, 2);
    LayoutPos->addWidget(mSpinBoxRotateY, 4, 3);

    QVBoxLayout *LayoutTotalPos = new QVBoxLayout(mGroupBoxPos);
    LayoutTotalPos->addLayout(LayoutPos);
    LayoutTotalPos->addStretch();

    // 样式
    //mGroupBoxStyle = new QGroupBox(tr("<<< 样式 >>>"));
    mGroupBoxStyle = new QGroupBox(tr(""));
    mLabelLineStyleShow = TBase::fLabel(tr("线型"), tr("mLabelLineStyleShow"));
    mComboBoxLineStyle  = TBase::fLineStyleComboBox();
    mComboBoxLineStyle->setCurrentIndex(mItem->fGetLineStyle());
    mLabelLineWidthShow = TBase::fLabel(tr("线宽"), tr("mLabelLineWeighShow"));
    mLabelLineWidth     = TBase::fLabel(tr(""), tr("mLabelLineWidth"));
    mLabelLineWidth->setMinimumSize(mLabelLineWidthShow->sizeHint());
    mLabelLineWidth->setMaximumSize(mLabelLineWidthShow->sizeHint() * 2);
    mSpinBoxLineWidth   = TBase::fSpinBox(tr("mSpinBoxLineWidth"), 1, 100);
    connect(mSpinBoxLineWidth, SIGNAL(valueChanged(int)), this, SLOT(fChooseLineWidth(int)));
    mSpinBoxLineWidth->setValue(mItem->fGetLineWidth());
    TBase::fColorLabel(mLabelLineWidth, Qt::black, mLabelLineWidth->width(), mItem->fGetLineWidth());
    mLabelLineColorShow = TBase::fLabel(tr("线色"), tr("mLabelLineColorShow"));
    mLabelLineColor     = TBase::fLabel(tr(""), tr("mLabelLineColor"));
    mLabelLineColor->setMinimumSize(mLabelLineColorShow->sizeHint());
    mLabelLineColor->setMaximumSize(mLabelLineColorShow->sizeHint() * 2);
    mButtonLineColor    = TBase::fButton(tr("浏览"), tr("mButtonLineColor"));
    connect(mButtonLineColor, SIGNAL(clicked()), this, SLOT(fChooseLineColor()));
    mLineColor = mItem->fGetLineColor();
    TBase::fColorLabel(mLabelLineColor, mLineColor);
    mLabelBackColorShow = TBase::fLabel(tr("填充"), tr("mLabelBackColorShow"));
    mLabelBackColor     = TBase::fLabel(tr(""), tr("mLabelBackColor"));
    mLabelBackColor->setMinimumSize(mLabelBackColorShow->sizeHint());
    mLabelBackColor->setMaximumSize(mLabelBackColorShow->sizeHint() * 2);
    mButtonBackColor    = TBase::fButton(tr("浏览"), tr("mButtonBackColor"));
    connect(mButtonBackColor, SIGNAL(clicked()), this, SLOT(fChooseBackColor()));
    mBackColor = mItem->fGetBackGroundColor();
    TBase::fColorLabel(mLabelBackColor, mBackColor);
    mLabelLineAlpha     = TBase::fLabel(tr("透明度"), tr("mLabelLineAlpha"));
    mSpinBoxLineAlpha   = TBase::fSpinBox(tr("mSpinBoxLineAlpha"), 0, 255);
    mSpinBoxLineAlpha->setValue(mItem->fGetLineColor().alpha());
    mLabelBackAlpha     = TBase::fLabel(tr("透明度"), tr("mLabelBackAlpha"));
    mSpinBoxBackAlpha   = TBase::fSpinBox(tr("mSpinBoxBackAlpha"), 0, 255);
    mSpinBoxBackAlpha->setValue(mItem->fGetBackGroundColor().alpha());

    QGridLayout *mLayoutStyle = new QGridLayout();
    mLayoutStyle->addWidget(mLabelLineStyleShow, 0, 0, 1, 1, Qt::AlignLeft);
    mLayoutStyle->addWidget(mComboBoxLineStyle, 0, 1, 1, 2);
    mLayoutStyle->addWidget(mLabelLineWidthShow, 0, 3, 1, 1, Qt::AlignLeft);
    mLayoutStyle->addWidget(mLabelLineWidth, 0, 4);
    mLayoutStyle->addWidget(mSpinBoxLineWidth, 0, 5);
    mLayoutStyle->addWidget(mLabelLineColorShow, 2, 0, 1, 1, Qt::AlignLeft);
    mLayoutStyle->addWidget(mLabelLineColor, 2, 1);
    mLayoutStyle->addWidget(mButtonLineColor, 2, 2);
    mLayoutStyle->addWidget(mLabelLineAlpha, 2, 3);
    mLayoutStyle->addWidget(mSpinBoxLineAlpha, 2, 4, 1, 2);
    mLayoutStyle->addWidget(mLabelBackColorShow, 3, 0, 1, 1, Qt::AlignLeft);
    mLayoutStyle->addWidget(mLabelBackColor, 3, 1);
    mLayoutStyle->addWidget(mButtonBackColor, 3, 2);
    mLayoutStyle->addWidget(mLabelBackAlpha, 3, 3);
    mLayoutStyle->addWidget(mSpinBoxBackAlpha, 3, 4, 1, 2);

    QVBoxLayout *LayoutTotalStyle = new QVBoxLayout(mGroupBoxStyle);
    LayoutTotalStyle->addLayout(mLayoutStyle);
    LayoutTotalStyle->addStretch();

    //服务
    //mGroupBoxStyle = new QGroupBox(tr("<<< 扩展属性 >>>"));

    TItemEx *ItemEx = dynamic_cast<TItemEx *>(mItem);

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
    mLabelModbusPro= TBase::fLabel(tr("选择协议"), tr("mLabelServerShow"));
    mComboBoxModbusPro= TBase::fComboBox(tr("mComboBoxModbusPro"));

    mComboBoxModbusPro->addItem("modbus rtu");
    mComboBoxModbusPro->addItem("modbus ascii");

    //crcCheckBox->setText("CRC低位在前");


    mComboBoxService->setCurrentIndex(mComboBoxService->findData(QVariant(ItemEx->fGetServiceId())));

    mLabelHintShow = TBase::fLabel(tr("报警描述"), tr("mLabelHintShow"));
    mTextEditHint = TBase::fTextEdit(tr("mTextEditHint"));
    mTextEditHint->setMaximumHeight(mLabelHintShow->sizeHint().height() * 2);
    mTextEditHint->setText(ItemEx->fGetHint());

    mLabelSubDeviceNum=TBase::fLabel(tr("通道"), tr("mLabelSubDeviceNum"));
    mLineEditSubDeviceNum=TBase::fSpinBox(tr("mLineEditSubDeviceNum"), 0, 200000);

    mLabelMainDeviceNum=TBase::fLabel(tr("设备号"), tr("mLabelMainDeviceNum"));
    mLineEditMainDeviceNum=TBase::fSpinBox(tr("mLineEditMainDeviceNum"), 0, 200000);

    mLabelGroupId=TBase::fLabel(tr("组号 "), tr("mLabelGroupId"));
    mSpinBoxGroupId=TBase::fSpinBox(tr("mSpinBoxGroupId"), 0, 1000);
    mGroupName=TBase::fLabel(tr("组名 "), tr("mGroupName"));
    mLabelGroupName=TBase::fLineEdit(tr("mLabelGroupId"));
    mStartAddr=TBase::fLabel(tr("起始地址"), tr("mStartAddr"));
    mLineEditStartAddr=TBase::fLineEdit(tr("mLineEditStartAddr"));


    mLabelGreaterEqual=TBase::fLabel(tr("大于"), tr("mLabelGreaterEqual"));
    mLineEditGreaterEqual=TBase::fDoubleSpinBox(tr("mLineEditGreaterEqual"), -200000, 200000);
    mLabelGreaterWrite=TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite"));
    mGroupNum=TBase::fSpinBox(tr("mGroupNum"), 0, 20000);;
    mLineEditWrite=TBase::fDoubleSpinBox(tr("mLineEditWrite"), -200000, 200000);;

    mLabelLess=TBase::fLabel(tr("小于"), tr("mLabelLess"));
    mLineEditLess=TBase::fDoubleSpinBox(tr("mLineEditLess"), -200000, 200000);
    mLabelLessWrite=TBase::fLabel(tr("写入"), tr("mLabelLessWrite"));
    mGroupLessNum=TBase::fSpinBox(tr("mGroupLessNum"), 0, 200000);
    mLineEditLessWrite=TBase::fDoubleSpinBox(tr("mLineEditLessWrite"), -200000, 200000);

    mLabelShowRange=TBase::fLabel(tr("显示范围:"), tr("mLabelShowRange"));
    mLabelStartRange=TBase::fLabel(tr("开始范围:"), tr("mLabelStartRange"));
    mLabelStartRange->setAlignment(Qt::AlignRight);
    msipnBoxStartRange=TBase::fDoubleSpinBox(tr("msipnBoxStartRange"), -200000, 200000);
    msipnBoxStartRange->setValue(ItemEx->showStart);
    mLabelStopRange=TBase::fLabel(tr("结束范围:"), tr("mLabelStopRange"));
    msipnBoxStopRange=TBase::fDoubleSpinBox(tr("msipnBoxStopRange"), -200000, 200000);;
    msipnBoxStopRange->setValue(ItemEx->showStop);

    QGridLayout *LayoutConfig = new QGridLayout();
    LayoutConfig->addWidget(mLabelNameIdShow, 0, 0);
    LayoutConfig->addWidget(mLineEditNameId, 0, 1);

    LayoutConfig->addWidget(mLabelServiceShow, 0, 2);
    LayoutConfig->addWidget(mComboBoxService, 0, 3);
    LayoutConfig->addWidget(mLabelModbusPro, 0, 4);
    LayoutConfig->addWidget(mComboBoxModbusPro, 0, 5);
    //LayoutConfig->addWidget(crcCheckBox, 0, 6);

    LayoutConfig->addWidget(mLabelHintShow, 2, 0);
    LayoutConfig->addWidget(mTextEditHint, 2, 1);

    TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);

    if(Graphics)
    {

        mComboBoxModbusPro->setCurrentText(Graphics->threeDevName);
        //crcCheckBox->setChecked(Graphics->crcPosition);
        LayoutConfig->addWidget(mLabelSubDeviceNum, 2, 2);
        LayoutConfig->addWidget(mLineEditSubDeviceNum, 2, 3);
        LayoutConfig->addWidget(mStartAddr, 2, 4);
        LayoutConfig->addWidget(mLineEditStartAddr, 2, 5);

        LayoutConfig->addWidget(mLabelMainDeviceNum, 3, 0);
        LayoutConfig->addWidget(mLineEditMainDeviceNum, 3, 1);
        LayoutConfig->addWidget(mLabelGroupId, 3, 2);
        LayoutConfig->addWidget(mSpinBoxGroupId, 3, 3);
        LayoutConfig->addWidget(mGroupName, 3, 4);

        LayoutConfig->addWidget(mLabelGroupName, 3, 5);
        LayoutConfig->addWidget(mLabelGreaterEqual, 4, 0);
        LayoutConfig->addWidget(mLineEditGreaterEqual, 4, 1);
        LayoutConfig->addWidget(mLabelGreaterWrite, 4, 2);
        LayoutConfig->addWidget(mGroupNum, 4, 3);
        LayoutConfig->addWidget(mLineEditWrite, 4, 4);
        LayoutConfig->addWidget(mLabelLess, 5, 0);
        LayoutConfig->addWidget(mLineEditLess, 5, 1);
        LayoutConfig->addWidget(mLabelLessWrite, 5, 2);
        LayoutConfig->addWidget(mGroupLessNum, 5, 3);
        LayoutConfig->addWidget(mLineEditLessWrite, 5, 4);

        mLineEditSubDeviceNum->setValue(Graphics->fGetNodeId());
        mLineEditMainDeviceNum->setValue(Graphics->fGetMainId());
        mSpinBoxGroupId->setValue(Graphics->fGetGroupId());
        mLabelGroupName->setText(Graphics->fGetGroupName());
        mLineEditStartAddr->setText(QString::number(Graphics->startAddr, 16));
        mLinkList=new QGroupBox(tr(""));
        QGridLayout *linkLayout = new QGridLayout();
        int i=0;
        int Mynum=0;
        foreach (TGraphics::link link, Graphics->greaterEqual) {
            linkLayout->addWidget(TBase::fLabel(tr("大于"), tr("mLabelGreaterEqual")), i, 0);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.currntNum), tr("mLabelGreaterEqual")), i, 1);

            linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.groupId), tr("mLabelGreaterWrite")), i, 3);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 4);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.writeNum), tr("mLabelGreaterWrite")), i, 5);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            button->setObjectName("1:"+QString::number(Mynum));
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));


            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("1:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 6);
            linkLayout->addWidget(button , i, 7);
            Mynum++;
            i++;
        }

        foreach (TGraphics::link link, Graphics->lessThan) {
            linkLayout->addWidget(TBase::fLabel(tr("小于"), tr("mLabelGreaterEqual")), i, 0);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.currntNum), tr("mLabelGreaterEqual")), i, 1);
            linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.groupId), tr("mLabelGreaterWrite")), i, 3);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 4);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.writeNum), tr("mLabelGreaterWrite")), i, 5);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            button->setObjectName("2:"+QString::number(Mynum));
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));

            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("2:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 6);

            linkLayout->addWidget(button , i, 7);
            Mynum++;
            i++;
        }

        QVBoxLayout *LinkTotal = new QVBoxLayout(mLinkList);
        LinkTotal->addLayout(linkLayout);
        LinkTotal->addStretch();

    }
    TButton *Button = dynamic_cast<TButton *>(mItem);
    if(Button!=NULL)
    {
        LayoutConfig->addWidget(mLabelGreaterWrite, 3, 0);
        LayoutConfig->addWidget(mGroupNum, 3, 1);
        LayoutConfig->addWidget(mLineEditWrite, 3, 2);

        LayoutConfig->addWidget(mLabelSubDeviceNum, 4, 0);
        LayoutConfig->addWidget(mLineEditSubDeviceNum, 4, 1);
        LayoutConfig->addWidget(mStartAddr, 4, 2);
        LayoutConfig->addWidget(mLineEditStartAddr, 4, 3);
        LayoutConfig->addWidget(mLabelMainDeviceNum, 5, 0);
        LayoutConfig->addWidget(mLineEditMainDeviceNum, 5, 1);
        LayoutConfig->addWidget(mLabelLessWrite, 5, 2);
        LayoutConfig->addWidget(mLineEditLessWrite, 5, 3);

        mLinkList=new QGroupBox(tr(""));
        QGridLayout *linkLayout = new QGridLayout();
        int i=0;
        int Mynum=0;
        foreach (TButton::link link, Button->greaterEqual) {
            linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), i, 0);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.groupId), tr("mLabelGreaterWrite")), i, 1);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.writeNum), tr("mLabelGreaterWrite")), i, 3);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            button->setObjectName("3:"+QString::number(Mynum));
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));

            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("3:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 4);


            linkLayout->addWidget(button , i, 5);
            Mynum++;
            i++;
        }
        Mynum=0;
        foreach(TButton::Control contral, Button->control)
        {
            linkLayout->addWidget(TBase::fLabel(tr("服务"), tr("mLabelGreaterWrite")), i, 0);
            linkLayout->addWidget(TBase::fLabel(contral.servername, tr("mLabelGreaterWrite")), i, 1);

            linkLayout->addWidget(TBase::fLabel(tr("主设备"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(contral.mainId), tr("mLabelGreaterWrite")), i, 3);
            linkLayout->addWidget(TBase::fLabel(tr("通道号"), tr("mLabelGreaterWrite")), i, 4);
            linkLayout->addWidget(TBase::fLabel(QString::number(contral.devId), tr("mLabelGreaterWrite")), i, 5);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 6);
            linkLayout->addWidget(TBase::fLabel(QString::number(contral.writeNum), tr("mLabelGreaterWrite")), i, 7);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));
            button->setObjectName("4:"+QString::number(Mynum));

            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("4:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 8);

            linkLayout->addWidget(button , i, 9);
            Mynum++;
            i++;
        }

        QVBoxLayout *LinkTotal = new QVBoxLayout(mLinkList);
        LinkTotal->addLayout(linkLayout);
        LinkTotal->addStretch();

    }
    //显示范围
    LayoutConfig->addWidget(mLabelShowRange, 6, 0);
    LayoutConfig->addWidget(mLabelStartRange, 6, 1);
    LayoutConfig->addWidget(msipnBoxStartRange, 6, 2);
    LayoutConfig->addWidget(mLabelStopRange, 6, 3);
    LayoutConfig->addWidget(msipnBoxStopRange, 6, 4);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxConfig);
    LayoutTotal->addLayout(LayoutConfig);
    LayoutTotal->addStretch();

    //mGroupBoxStyle = new QGroupBox(tr("<<< 联动列表 >>>"));



    return true;
}

bool DialogStyle::fCreateAngle()
{
    TArc *Arc = dynamic_cast<TArc *>(mItem);

    if(NULL == Arc)
    {
        return false;
    }

    //mGroupBoxAngle = new QGroupBox(tr("<<< 角度 >>>"));
    mGroupBoxAngle = new QGroupBox(tr(""));
    mLabelArcMode = TBase::fLabel(tr("模式"), tr("mLabelArcMode"));
    mComboBoxArcMode = TBase::fComboBox(tr("mComboBoxArcMode"));
    mComboBoxArcMode->addItem(tr("Pie扇形"));
    mComboBoxArcMode->addItem(tr("Arc弧"));
    mComboBoxArcMode->addItem(tr("Chord弦"));
    mComboBoxArcMode->setCurrentIndex(Arc->fGetMode());
    mLabelStartAngleShow = TBase::fLabel(tr("起始角度:"), tr("mLabelStartAngleShow"));
    mSpinBoxStartAngle = TBase::fDoubleSpinBox(tr("mSpinBoxStartAngle"), 0, 360);
    mSpinBoxStartAngle->setValue(Arc->fGetStartAngle());
    mLabelCoverAngleShow = TBase::fLabel(tr("覆盖角度:"), tr("mLabelCoverAngleShow"));
    mSpinBoxCoverAngle = TBase::fDoubleSpinBox(tr("mSpinBoxCoverAngle"), 1, 360);
    mSpinBoxCoverAngle->setValue(Arc->fGetCoverAngle());

    QGridLayout *mLayoutAngle = new QGridLayout();
    mLayoutAngle->addWidget(mLabelArcMode, 0, 0);
    mLayoutAngle->addWidget(mComboBoxArcMode, 0 , 1);
    mLayoutAngle->addWidget(mLabelStartAngleShow, 1, 0);
    mLayoutAngle->addWidget(mSpinBoxStartAngle, 1, 1);
    mLayoutAngle->addWidget(mLabelCoverAngleShow, 2, 0);
    mLayoutAngle->addWidget(mSpinBoxCoverAngle, 2, 1);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxAngle);
    LayoutTotal->addLayout(mLayoutAngle);
    LayoutTotal->addStretch();

    return true;
}

bool DialogStyle::fCreateTable()
{
    TTable *Table = dynamic_cast<TTable *>(mItem);

    if(NULL == Table)
    {
        return false;
    }

    int RowSum = Table->fGetRowSum();
    int ColSum = Table->fGetColSum();

    //mGroupBoxTable = new QGroupBox(tr("<<< 表格 >>>"));
    mGroupBoxTable = new QGroupBox(tr(""));
    mTableView = new TTableView(tr("mTableView"));
    mTableView->fSetSelectRows(false);
    mTableView->fSetEditAble(true);
    mTableView->fSetVEdit(false);
    mTableView->fSetSize(RowSum, ColSum);
    mTableView->setMinimumWidth(50);
    mTableView->setMaximumWidth(250);
    //mTableView->setMinimumHeight(50);
    //mTableView->setMaximumHeight(100);

    for(int i = 0; i < ColSum; i++)
    {
        mTableView->fSetHeadLength(i, Table->fGetColLength(i));

        for(int j = 0; j < ColSum; j++)
        {
            mTableView->fSetCell(i, j, Table->fGetCell(i, j));
        }
    }

    mLabelTableRowShow = TBase::fLabel(tr("行数"), tr("mLabelTableRowShow"));
    mSpinBoxTableRow = TBase::fSpinBox(tr("mSpinBoxTableRow"), 0, 256);
    mSpinBoxTableRow->setValue(RowSum);

    mLabelTableRowLenShow = TBase::fLabel(tr("行高"), tr("mLabelTableRowLenShow"));
    mSpinBoxRowLen = TBase::fSpinBox(tr("mSpinBoxRowLen"), 1, 256);
    mSpinBoxRowLen->setValue(Table->fGetRowLength());

    mLabelTableColShow = TBase::fLabel(tr("列数"), tr("mLabelTableColShow"));
    mSpinBoxTableCol = TBase::fSpinBox(tr("mSpinBoxTableCol"), 0, 256);
    mSpinBoxTableCol->setValue(ColSum);

    connect(mSpinBoxRowLen, SIGNAL(valueChanged(int)), this, SLOT(fRowLengthChange(int)));
    connect(mSpinBoxTableRow, SIGNAL(valueChanged(int)), this, SLOT(fTableSizeChange(int)));
    connect(mSpinBoxTableCol, SIGNAL(valueChanged(int)), this, SLOT(fTableSizeChange(int)));

    fRowLengthChange(Table->fGetRowLength());
    mLabelShow = TBase::fLabel(tr("可以通过拖动，改变列宽"), tr("mLabelShow"));

    QHBoxLayout *LayoutTop = new QHBoxLayout();
    LayoutTop->addStretch();
    LayoutTop->addWidget(mLabelTableRowShow);
    LayoutTop->addWidget(mSpinBoxTableRow);
    LayoutTop->addWidget(mLabelTableRowLenShow);
    LayoutTop->addWidget(mSpinBoxRowLen);
    LayoutTop->addStretch();
    LayoutTop->addWidget(mLabelTableColShow);
    LayoutTop->addWidget(mSpinBoxTableCol);
    LayoutTop->addWidget(mLabelShow);
    LayoutTop->addStretch();

    QVBoxLayout *Layout = new QVBoxLayout();
    Layout->addLayout(LayoutTop);
    Layout->addWidget(mTableView);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxTable);
    LayoutTotal->addLayout(Layout);
    LayoutTotal->addStretch();

    return true;
}

bool DialogStyle::fCreateButton()
{
    TButton *Button = dynamic_cast<TButton *>(mItem);

    if(NULL == Button)
    {
        return false;
    }

    //mGroupBoxButton = new QGroupBox(tr("<<< 按钮 >>>"));
    mGroupBoxButton = new QGroupBox(tr(""));
    mLabelButtonTextShow = TBase::fLabel(tr("文本"), tr("mLabelButtonTextShow"));
    mEditButtonText = TBase::fLineEdit(tr("mEditButtonText"));
    mEditButtonText->setText(Button->fGetText());
    mLabelButtonBeginTime=TBase::fLabel(tr("开始时间"), tr("mLabelButtonBeginTime"));
    mComBoxBeginTime=TBase::fComboBox(tr("mComBoxBeginTime"));

    mComBoxBeginTime->addItem("星期一", 1);
    mComBoxBeginTime->addItem("星期二", 2);
    mComBoxBeginTime->addItem("星期三", 3);
    mComBoxBeginTime->addItem("星期四", 4);
    mComBoxBeginTime->addItem("星期五", 5);
    mComBoxBeginTime->addItem("星期六", 6);
    mComBoxBeginTime->addItem("星期天", 7);
    mComBoxBeginTime->setCurrentIndex(Button->mBeginWeek-1);

    mTimeEditBeginTime=new QTimeEdit();
    mTimeEditBeginTime->setTime(Button->mBeginTime);
    mLabelButtonEndTime=TBase::fLabel(tr("结束时间"), tr("mLabelButtonEndTime"));

    mComBoxEndTime=TBase::fComboBox(tr("mComBoxEndTime"));

    mComBoxEndTime->addItem("星期一", 1);
    mComBoxEndTime->addItem("星期二", 2);
    mComBoxEndTime->addItem("星期三", 3);
    mComBoxEndTime->addItem("星期四", 4);
    mComBoxEndTime->addItem("星期五", 5);
    mComBoxEndTime->addItem("星期六", 6);
    mComBoxEndTime->addItem("星期天", 7);
    mComBoxEndTime->setCurrentIndex(Button->mEndWeek-1);

    mTimeEditEndTime=new QTimeEdit();
    mTimeEditEndTime->setTime(Button->mEndTime);

    mLabelFontSizeShow = TBase::fLabel(tr("字号"), tr("mLabelFontSizeShow"));
    mSpinBoxFontSize = TBase::fSpinBox(tr("mSpinBoxFontSize"), 0, 100);
    mSpinBoxFontSize->setValue(Button->fGetFontSize());
    

    mLabelButtonAutoSend=TBase::fLabel(tr("跳转显示比例"), tr("mLabelButtonAutoSend"));
    mComBoxOpenClose=TBase::fComboBox(tr("mComBoxOpenClose"));

    mSpinBoxViewWidth= TBase::fSpinBox(tr("mSpinBoxViewWidth"), 0, 20000);
    mSpinBoxViewWidth->setValue(Button->width);
    mLabelX=TBase::fLabel(tr("X"), tr("mLabelX"));
    mLabelX->setMaximumWidth(10);
    mSpinBoxViewHeigth=TBase::fSpinBox(tr("mSpinBoxViewHeigth"), 0, 20000);
    mSpinBoxViewHeigth->setValue(Button->heigth);

    QHBoxLayout *Layout3 = new QHBoxLayout();
    Layout3->addWidget(mLabelFontSizeShow);
    Layout3->addWidget(mSpinBoxFontSize);

    QHBoxLayout *Layout = new QHBoxLayout();
    Layout->addWidget(mLabelButtonTextShow);
    Layout->addWidget(mEditButtonText);

    QHBoxLayout *Layout1 = new QHBoxLayout();
    Layout1->addWidget(mLabelButtonAutoSend);
    Layout1->addWidget(mSpinBoxViewWidth);
    Layout1->addWidget(mLabelX);
    Layout1->addWidget(mSpinBoxViewHeigth);

    QHBoxLayout *Layout2 = new QHBoxLayout();

    mLabelPixmapButtonShow = TBase::fLabel(tr("背景图片"), tr("mLabelPixmapButtonShow"));
    mLineEditPixmapButtonDir = TBase::fLineEdit(tr("mLineEditPixmapButtonDir"));
    mTbuttonPixmap = TBase::fButton(tr("浏览"), tr("mTbuttonPixmap"));
    mLineEditPixmapButtonDir->setText(Button->mPixPath);
    Layout2->addWidget(mLabelPixmapButtonShow);
    Layout2->addWidget(mLineEditPixmapButtonDir);
    Layout2->addWidget(mTbuttonPixmap);

    QHBoxLayout *Layout4 = new QHBoxLayout();
    isShowTimre=new QCheckBox();
    isShowTimre->setText(tr("是否跳转到定时表"));
    isShowTimre->setChecked(Button->isShowTimer);
    Layout4->addWidget(isShowTimre);

    connect(mTbuttonPixmap, SIGNAL(clicked()), this, SLOT(fPixmapPathChange()));
    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxButton);
    LayoutTotal->addLayout(Layout);
    LayoutTotal->addLayout(Layout3);
    LayoutTotal->addLayout(Layout1);
    LayoutTotal->addLayout(Layout2);
    LayoutTotal->addLayout(Layout4);

    LayoutTotal->addStretch();


    return true;
}

bool DialogStyle::fSaveMeter()
{
    TMeter *Meter = dynamic_cast<TMeter *>(mItem);

    if(NULL == Meter)
    {
        return false;
    }

    Meter->fSetMode(mComboBoxMeterMode->currentIndex());
    Meter->fSetScaleSum(mSpinBoxScaleSum->value());
    Meter->fSetStartAngle(mSpinBoxMeterStartAngle->value());
    Meter->fSetStepAngle(mSpinBoxMeterStepAngle->value());
    Meter->fSetStartValue(mSpinBoxMeterStartValue->value());
    Meter->fSetStepValue(mSpinBoxMeterStepValue->value());
    Meter->fSetPointerColor(mMeterPointerColor);

    return true;
}

bool DialogStyle::fCreateMeter()
{
    TMeter *Meter = dynamic_cast<TMeter *>(mItem);

    if(NULL == Meter)
    {
        return false;
    }

    //mGroupBoxMeter = new QGroupBox(tr("<<< 仪表 >>>"));
    mGroupBoxMeter = new QGroupBox(tr(""));
    mLabelMeterModeShow = TBase::fLabel(tr("模式"), tr("mLabelMeterModeShow"));
    mComboBoxMeterMode = TBase::fComboBox(tr("mComboBoxMeterMode"));
    mComboBoxMeterMode->addItem(tr("顺时针"));
    mComboBoxMeterMode->addItem(tr("逆时针"));
    mComboBoxMeterMode->setCurrentIndex(Meter->fGetMode());
    mLabelScaleSumShow = TBase::fLabel(tr("刻度数"), tr("mLabelScaleSumShow"));
    mSpinBoxScaleSum = TBase::fSpinBox(tr("mSpinBoxScaleSum"), 1, 128);
    mSpinBoxScaleSum->setValue(Meter->fGetScaleSum());
    mLabelMeterStartAngleShow = TBase::fLabel(tr("起始角度"), tr("mLabelMeterStartAngleShow"));
    mSpinBoxMeterStartAngle = TBase::fDoubleSpinBox(tr("mSpinBoxMeterStartAngle"), 0, 360.0);
    mSpinBoxMeterStartAngle->setValue(Meter->fGetStartAngle());
    mLabelMeterStepAngleShow = TBase::fLabel(tr("阶梯角度"), tr("mLabelMeterStepAngleShow"));
    mSpinBoxMeterStepAngle = TBase::fDoubleSpinBox(tr("mSpinBoxMeterStepAngle"), 0, 360.0);
    mSpinBoxMeterStepAngle->setValue(Meter->fGetStepAngle());
    mLabelMeterStartValueShow = TBase::fLabel(tr("起始值"), tr("mLabelMeterStartValueShow"));
    mSpinBoxMeterStartValue = TBase::fDoubleSpinBox(tr("mSpinBoxMeterStartValue"), -1024.0, 1024.0);
    mSpinBoxMeterStartValue->setValue(Meter->fGetStartValue());
    mLabelMeterStepValueShow = TBase::fLabel(tr("阶梯值"), tr("mLabelMeterStepValueShow"));
    mSpinBoxMeterStepValue = TBase::fDoubleSpinBox(tr("mSpinBoxMeterStepValue"), -1024, 1024.0);
    mSpinBoxMeterStepValue->setValue(Meter->fGetStepValue());

    mLabelMeterPointerColorShow = TBase::fLabel(tr("指针颜色"), tr("mLabelMeterPointerColorShow"));
    mLabelMeterPointerColor = TBase::fLabel(tr(""), tr("mLabelMeterPointerColor"));
    mLabelMeterPointerColor->setMinimumSize(mLabelMeterPointerColorShow->sizeHint());
    mLabelMeterPointerColor->setMaximumSize(mLabelMeterPointerColorShow->sizeHint() * 2);
    mMeterPointerColor = Meter->fGetPointerColor();
    TBase::fColorLabel(mLabelMeterPointerColor, mMeterPointerColor);
    mButtonMeterPointColor = TBase::fButton(tr("浏览"), tr("mButtonMeterPointColor"));
    connect(mButtonMeterPointColor, SIGNAL(clicked()), this, SLOT(fChoosePointerColor()));

    QGridLayout *Layout = new QGridLayout();
    Layout->addWidget(mLabelMeterModeShow, 0, 0);
    Layout->addWidget(mComboBoxMeterMode, 0, 1, 1, 2);
    Layout->addWidget(mLabelScaleSumShow, 1, 0);
    Layout->addWidget(mSpinBoxScaleSum, 1, 1, 1, 2);
    Layout->addWidget(mLabelMeterStartAngleShow, 2, 0);
    Layout->addWidget(mSpinBoxMeterStartAngle, 2, 1, 1, 2);
    Layout->addWidget(mLabelMeterStepAngleShow, 3, 0);
    Layout->addWidget(mSpinBoxMeterStepAngle, 3, 1, 1, 2);
    Layout->addWidget(mLabelMeterStartValueShow, 4, 0);
    Layout->addWidget(mSpinBoxMeterStartValue, 4, 1, 1, 2);
    Layout->addWidget(mLabelMeterStepValueShow, 5, 0);
    Layout->addWidget(mSpinBoxMeterStepValue, 5, 1, 1, 2);
    Layout->addWidget(mLabelMeterPointerColorShow, 6, 0);
    Layout->addWidget(mLabelMeterPointerColor, 6, 1);
    Layout->addWidget(mButtonMeterPointColor, 6, 2);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxMeter);
    LayoutTotal->addLayout(Layout);
    LayoutTotal->addStretch();

    return true;
}

bool DialogStyle::fSaveProcessBar()
{
    TProcessBar *ProcessBar = dynamic_cast<TProcessBar *>(mItem);

    if(NULL == ProcessBar)
    {
        return false;
    }

    ProcessBar->fSetMode(mComboBoxProcessBarMode->currentIndex());
    ProcessBar->fSetScaleSum(mSpinBoxProcessBarScaleSum->value());
    ProcessBar->fSetStartValue(mSpinBoxProcessBarStartValue->value());
    ProcessBar->fSetStepValue(mSpinBoxProcessBarStepValue->value());
    ProcessBar->fSetValueColor(mProcessBarValueColor);

    return true;
}

bool DialogStyle::fCreateProcessBar()
{
    TProcessBar *ProcessBar = dynamic_cast<TProcessBar *>(mItem);

    if(NULL == ProcessBar)
    {
        return false;
    }

    //mGroupBoxProcessBar = new QGroupBox(tr("<<< 刻度条 >>>"));
    mGroupBoxProcessBar = new QGroupBox(tr(""));
    mLabelProcessBarModeShow = TBase::fLabel(tr("模式"), tr("mLabelProcessBarModeShow"));
    mComboBoxProcessBarMode = TBase::fComboBox(tr("mComboBoxProcessBarMode"));
    mComboBoxProcessBarMode->addItem(tr("居右"));
    mComboBoxProcessBarMode->addItem(tr("居上"));
    mComboBoxProcessBarMode->addItem(tr("居左"));
    mComboBoxProcessBarMode->addItem(tr("居下"));
    mComboBoxProcessBarMode->setCurrentIndex(ProcessBar->fGetMode());
    mLabelProcessBarScaleSumShow = TBase::fLabel(tr("刻度数"), tr("mLabelProcessBarScaleSumShow"));
    mSpinBoxProcessBarScaleSum = TBase::fSpinBox(tr("mSpinBoxProcessBarScaleSum"), 1, 128);
    mSpinBoxProcessBarScaleSum->setValue(ProcessBar->fGetScaleSum());
    mLabelProcessBarStartValueShow = TBase::fLabel(tr("起始值"), tr("mLabelProcessBarStartValueShow"));
    mSpinBoxProcessBarStartValue = TBase::fDoubleSpinBox(tr("mSpinBoxProcessBarStartValue"), -1024.0, 1024.0);
    mSpinBoxProcessBarStartValue->setValue(ProcessBar->fGetStartValue());
    mLabelProcessBarStepValueShow = TBase::fLabel(tr("阶梯值"), tr("mLabelProcessBarStepValueShow"));
    mSpinBoxProcessBarStepValue = TBase::fDoubleSpinBox(tr("mSpinBoxProcessBarStepValue"), -1024, 1024.0);
    mSpinBoxProcessBarStepValue->setValue(ProcessBar->fGetStepValue());
    mLabelProcessBarValueColorShow = TBase::fLabel(tr("内部颜色"), tr("mLabelProcessBarValueColorShow"));
    mLabelProcessBarValueColor = TBase::fLabel(tr(""), tr("mLabelProcessBarValueColor"));
    mLabelProcessBarValueColor->setMinimumSize(mLabelProcessBarValueColorShow->sizeHint());
    mLabelProcessBarValueColor->setMaximumSize(mLabelProcessBarValueColorShow->sizeHint() * 2);
    mProcessBarValueColor = ProcessBar->fGetValueColor();
    TBase::fColorLabel(mLabelProcessBarValueColor, mProcessBarValueColor);
    mButtonProcessBarValueColor = TBase::fButton(tr("浏览"), tr("mButtonProcessBarValueColor"));
    connect(mButtonProcessBarValueColor, SIGNAL(clicked()), this, SLOT(fChooseValueColor()));

    QGridLayout *Layout = new QGridLayout();
    Layout->addWidget(mLabelProcessBarModeShow, 0, 0);
    Layout->addWidget(mComboBoxProcessBarMode, 0, 1, 1, 2);
    Layout->addWidget(mLabelProcessBarScaleSumShow, 1, 0);
    Layout->addWidget(mSpinBoxProcessBarScaleSum, 1, 1, 1, 2);
    Layout->addWidget(mLabelProcessBarStartValueShow, 2, 0);
    Layout->addWidget(mSpinBoxProcessBarStartValue, 2, 1, 1, 2);
    Layout->addWidget(mLabelProcessBarStepValueShow, 3, 0);
    Layout->addWidget(mSpinBoxProcessBarStepValue, 3, 1, 1, 2);
    Layout->addWidget(mLabelProcessBarValueColorShow, 4, 0);
    Layout->addWidget(mLabelProcessBarValueColor, 4, 1);
    Layout->addWidget(mButtonProcessBarValueColor, 4, 2);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxProcessBar);
    LayoutTotal->addLayout(Layout);
    LayoutTotal->addStretch();

    return true;
}

void DialogStyle::fRowLengthChange(int Len)
{
    if(mTableView)
    {
        mTableView->fSetRowLength(Len);
    }
}

void DialogStyle::fTableSizeChange(int Value)
{
    Q_UNUSED(Value);

    mTableView->fSetSize(mSpinBoxTableRow->value(),
                         mSpinBoxTableCol->value());
}

bool DialogStyle::fSaveTable()
{
    TTable *Table = dynamic_cast<TTable *>(mItem);

    if(NULL == Table)
    {
        return false;
    }

    int RowSum = mSpinBoxTableRow->value();
    int ColSum = mSpinBoxTableCol->value();

    Table->fSetSize(RowSum, ColSum);
    Table->fSetRowLength(mSpinBoxRowLen->value());

    for(int i = 0; i < ColSum; i++)
    {
        Table->fSetColLength(i, mTableView->fGetColLength(i));

        for(int j = 0; j < RowSum; j++)
        {
            //qDebug() << j << i;
            Table->fSetCell(j, i, mTableView->fGetCell(j, i));
        }
    }

    return true;
}

bool DialogStyle::fSaveButton()
{
    TButton *Button = dynamic_cast<TButton *>(mItem);

    if(NULL == Button)
    {
        return false;
    }

    Button->fSetText(mEditButtonText->text());
    Button->width=mSpinBoxViewWidth->value();
    Button->heigth=mSpinBoxViewHeigth->value();
    Button->fSetFontSize(mSpinBoxFontSize->value());
    Button->isShowTimer=this->isShowTimre->isChecked();

    Button->mcomNum=mComBoxOpenClose->currentIndex();           //要发送的指令号  0、开   1、关
    Button->mBeginWeek=mComBoxBeginTime->currentData().toInt();
    Button->mBeginTime=  mTimeEditBeginTime->time();      //开始时间

    Button->mEndWeek=mComBoxEndTime->currentData().toInt();          //结束时间
    Button->mEndTime = mTimeEditEndTime->time();       //结束时间
    Button->mPixPath=mLineEditPixmapButtonDir->text();

    return true;
}

bool DialogStyle::fCreateText()
{
    TText* Text = dynamic_cast<TText *>(mItem);

    if(NULL == Text)
    {
        return false;
    }

    //mGroupBoxText = new QGroupBox(tr("<<< 文字 >>>"));
    mGroupBoxText = new QGroupBox(tr(""));
    mLabelTextShow = TBase::fLabel(tr("文本"), tr("mLabelTextShow"));
    mTextEditText= TBase::fTextEdit(tr("mTextEditText"));
    mTextEditText->setMaximumHeight(mLabelTextShow->sizeHint().height() * 2);
    mTextEditText->setText(Text->fGetText());
    mLabelFontFamilyShow = TBase::fLabel(tr("字体"), tr("mLabelFontFamilyShow"));
    /*mComboBoxFontFamily = TBase::fComboBox(tr("mComboBoxFontFamily"));
    mComboBoxFontFamily->addItem(tr("Times New Roman"));
    mComboBoxFontFamily->addItem(tr("Arial"));
    mComboBoxFontFamily->addItem(tr("宋体"));
    mComboBoxFontFamily->addItem(tr("黑体"));
    mComboBoxFontFamily->addItem(tr("隶书"));
    mComboBoxFontFamily->addItem(tr("幼圆"));
    mComboBoxFontFamily->addItem(tr("仿宋"));
    mComboBoxFontFamily->addItem(tr("华文行楷"));*/
    mComboBoxFontFamily = new QFontComboBox;

    mComboBoxFontFamily->setCurrentText((Text->fGetFontFamily()));
    mLabelFontSizeShow = TBase::fLabel(tr("字号"), tr("mLabelFontSizeShow"));
    mSpinBoxFontSize = TBase::fSpinBox(tr("mSpinBoxFontSize"), 0, 100);
    mSpinBoxFontSize->setValue(Text->fGetFontSize());
    mLabelSpacingShow = TBase::fLabel(tr("文字间距"), tr("mLabelSpacingShow"));
    mSpinBoxSpacing = TBase::fSpinBox(tr("mSpinBoxSpacing"), 0, 100);
    mSpinBoxSpacing->setValue(Text->fGetFontSpacing());
    mCheckBoxFontBlob = TBase::fCheckBox(tr("加粗"), tr("mCheckBoxFontBlob"));
    mCheckBoxFontBlob->setChecked(Text->fGetFontBlod());
    mCheckBoxFontItalic = TBase::fCheckBox(tr("倾斜"), tr("mCheckBoxFontItalic"));
    mCheckBoxFontItalic->setChecked(Text->fGetFontItalic());
    mCheckBoxFontUnderLine = TBase::fCheckBox(tr("下划线"), tr("mCheckBoxFontUnderLine"));
    mCheckBoxFontUnderLine->setChecked(Text->fGetFontUnderLine());
    mCheckBoxFontStrikeOut = TBase::fCheckBox(tr("删除线"), tr("mCheckBoxFontStrikeOut"));
    mCheckBoxFontStrikeOut->setChecked(Text->fGetFontStrikeOut());

    QGridLayout *mLayoutText = new QGridLayout();
    mLayoutText->addWidget(mLabelTextShow, 0, 0, 1, 1, Qt::AlignLeft);
    mLayoutText->addWidget(mTextEditText, 0, 1, 1, 3);
    mLayoutText->addWidget(mLabelFontFamilyShow, 1, 0, 1, 1, Qt::AlignLeft);
    mLayoutText->addWidget(mComboBoxFontFamily, 1, 1, 1, 3);
    mLayoutText->addWidget(mLabelFontSizeShow, 2, 0, 1, 1, Qt::AlignLeft);
    mLayoutText->addWidget(mSpinBoxFontSize, 2, 1);
    mLayoutText->addWidget(mLabelSpacingShow, 2, 2, 1, 1, Qt::AlignRight);
    mLayoutText->addWidget(mSpinBoxSpacing, 2, 3);
    mLayoutText->addWidget(mCheckBoxFontBlob, 3, 0, 1, 1, Qt::AlignRight);
    mLayoutText->addWidget(mCheckBoxFontUnderLine, 3, 1, 1, 1, Qt::AlignRight);
    mLayoutText->addWidget(mCheckBoxFontItalic, 3, 2, 1, 1, Qt::AlignRight);
    mLayoutText->addWidget(mCheckBoxFontStrikeOut, 3, 3, 1, 1, Qt::AlignRight);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxText);
    LayoutTotal->addLayout(mLayoutText);
    LayoutTotal->addStretch();

    return true;
}

bool DialogStyle::fCreateValueText()
{
    TValueText* ValueText = dynamic_cast<TValueText *>(mItem);

    if(NULL == ValueText)
    {
        return false;
    }

    //mGroupBoxValueText = new QGroupBox(tr("<<< 文字值 >>>"));
    mGroupBoxValueText = new QGroupBox(tr(""));
    mLabelModeShow = TBase::fLabel(tr("模式"), tr("mLabelModeShow"));
    mComboBoxTextMode = TBase::fComboBox(tr("mComboBoxTextMode"));
    mComboBoxTextMode->addItem(tr("整数"));
    mComboBoxTextMode->addItem(tr("小数"));
    mComboBoxTextMode->setCurrentIndex(ValueText->fGetMode());
    mLabelIntLenShow = TBase::fLabel(tr("整数长度"), tr("mLabelIntLenShow"));
    mSpinBoxTextIntLen = TBase::fSpinBox(tr("mSpinBoxTextIntLen"), 0, 32);
    mSpinBoxTextIntLen->setValue(ValueText->fGetIntLen());
    mLabelPointLenShow = TBase::fLabel(tr("小数长度"), tr("mLabelPointLenShow"));
    mSpinBoxTextPointLen = TBase::fSpinBox(tr("mSpinBoxTextPointLen"), 0, 32);
    mSpinBoxTextPointLen->setValue(ValueText->fGetPointLen());

    mLabelUpAlarm=TBase::fLabel(tr("报警上限"), tr("mLabelUpAlarm"));
    mSpinBoxUpAlarm=TBase::fDoubleSpinBox(tr("mSpinBoxUpAlarm"), -65555, 65555);
    mSpinBoxUpAlarm->setValue(ValueText->mUpAlarm);

    mLabelDownAlarm=TBase::fLabel(tr("报警下限"), tr("mLabelUpAlarm"));
    mSpinBoxDownAlarm=TBase::fDoubleSpinBox(tr("mSpinBoxUpAlarm"), -65555, 65555);
    mSpinBoxDownAlarm->setValue(ValueText->mDownAlarm);

    QGridLayout *mLayoutValueText = new QGridLayout();
    mLayoutValueText->addWidget(mLabelModeShow, 0, 0);
    mLayoutValueText->addWidget(mComboBoxTextMode, 0, 1, 1, 3);
    mLayoutValueText->addWidget(mLabelIntLenShow, 1, 0);
    mLayoutValueText->addWidget(mSpinBoxTextIntLen, 1, 1);
    mLayoutValueText->addWidget(mLabelPointLenShow, 1, 2);
    mLayoutValueText->addWidget(mSpinBoxTextPointLen, 1, 3);

    mLayoutValueText->addWidget(mLabelDownAlarm, 2, 0);
    mLayoutValueText->addWidget(mSpinBoxDownAlarm, 2, 1);
    mLayoutValueText->addWidget(mLabelUpAlarm, 2, 2);
    mLayoutValueText->addWidget(mSpinBoxUpAlarm, 2, 3);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxValueText);
    LayoutTotal->addLayout(mLayoutValueText);
    LayoutTotal->addStretch();

    return true;
}

bool DialogStyle::fCreatePixmap()
{
    TPixmap *Pixmap = dynamic_cast<TPixmap *>(mItem);

    if(NULL == Pixmap)
    {
        return false;
    }

    //mGroupBoxPixmap = new QGroupBox(tr("图片"));
    mGroupBoxPixmap = new QGroupBox(tr(""));
    mLabelPixmapDirShow = TBase::fLabel(tr("路径"), tr("mLabelPixmapDirShow"));
    mLineEditPixmapDir = TBase::fLineEdit(tr("mLineEditPixmapDir"));

    mButtonPixmap = TBase::fButton(tr("浏览"), tr("mButtonPixmap"));
    mLabelCondition=TBase::fLabel(tr("显示条件"), tr("mLabelCondition"));
    mComboxCondition=TBase::fComboBox(tr("mComboxCondition"));
    mComboxCondition->addItem(tr("关"));
    mComboxCondition->addItem(tr("开"));
    mComboxCondition->addItem(tr("离线"));
    mComboxCondition->addItem(tr("背景图"));
    if(Pixmap->condiation.size()>=1)
    {
        mComboxCondition->setCurrentIndex(Pixmap->condiation.at(0));
        mLineEditPixmapDir->setText(Pixmap->mFileDir.at(0));
    }

    mLabelPixmapDirShow1 = TBase::fLabel(tr("路径"), tr("mLabelPixmapDirShow1"));
    mLineEditPixmapDir1 = TBase::fLineEdit(tr("mLineEditPixmapDir1"));

    mButtonPixmap1 = TBase::fButton(tr("浏览"), tr("mButtonPixmap1"));
    mLabelCondition1=TBase::fLabel(tr("显示条件"), tr("mLabelCondition1"));
    mComboxCondition1=TBase::fComboBox(tr("mComboxCondition1"));
    mComboxCondition1->addItem(tr("关"));
    mComboxCondition1->addItem(tr("开"));
    mComboxCondition1->addItem(tr("离线"));
    mComboxCondition1->addItem(tr("背景图"));
    if(Pixmap->condiation.size()>=2)
    {
    mComboxCondition1->setCurrentIndex(Pixmap->condiation.at(1));
    mLineEditPixmapDir1->setText(Pixmap->mFileDir.at(1));
    }

    mLabelPixmapDirShow2 = TBase::fLabel(tr("路径"), tr("mLabelPixmapDirShow2"));
    mLineEditPixmapDir2 = TBase::fLineEdit(tr("mLineEditPixmapDir2"));

    mButtonPixmap2 = TBase::fButton(tr("浏览"), tr("mButtonPixmap2"));
    mLabelCondition2=TBase::fLabel(tr("显示条件"), tr("mLabelCondition2"));
    mComboxCondition2=TBase::fComboBox(tr("mComboxCondition2"));
    mComboxCondition2->addItem(tr("关"));
    mComboxCondition2->addItem(tr("开"));
    mComboxCondition2->addItem(tr("离线"));
    mComboxCondition2->addItem(tr("背景图"));
    if(Pixmap->condiation.size()>=3)
    {
    mComboxCondition2->setCurrentIndex(Pixmap->condiation.at(2));
    mLineEditPixmapDir2->setText(Pixmap->mFileDir.at(2));
    }

    connect(mButtonPixmap, SIGNAL(clicked()), this, SLOT(fPixmapPathChange()));
    connect(mButtonPixmap1, SIGNAL(clicked()), this, SLOT(fPixmapPathChange()));
    connect(mButtonPixmap2, SIGNAL(clicked()), this, SLOT(fPixmapPathChange()));

    QHBoxLayout *mLayoutPixmap = new QHBoxLayout();
    mLayoutPixmap->addWidget(mLabelPixmapDirShow);
    mLayoutPixmap->addWidget(mLineEditPixmapDir);
    mLayoutPixmap->addWidget(mButtonPixmap);
    mLayoutPixmap->addWidget(mLabelCondition);
    mLayoutPixmap->addWidget(mComboxCondition);

    QHBoxLayout *mLayoutPixmap1 = new QHBoxLayout();
    mLayoutPixmap1->addWidget(mLabelPixmapDirShow1);
    mLayoutPixmap1->addWidget(mLineEditPixmapDir1);
    mLayoutPixmap1->addWidget(mButtonPixmap1);
    mLayoutPixmap1->addWidget(mLabelCondition1);
    mLayoutPixmap1->addWidget(mComboxCondition1);

    QHBoxLayout *mLayoutPixmap2 = new QHBoxLayout();
    mLayoutPixmap2->addWidget(mLabelPixmapDirShow2);
    mLayoutPixmap2->addWidget(mLineEditPixmapDir2);
    mLayoutPixmap2->addWidget(mButtonPixmap2);
    mLayoutPixmap2->addWidget(mLabelCondition2);
    mLayoutPixmap2->addWidget(mComboxCondition2);

    QVBoxLayout *LayoutTotal = new QVBoxLayout(mGroupBoxPixmap);
    LayoutTotal->addLayout(mLayoutPixmap);
    LayoutTotal->addLayout(mLayoutPixmap1);
    LayoutTotal->addLayout(mLayoutPixmap2);
    LayoutTotal->addStretch();

    return true;
}
void DialogStyle::fPixmapPathChange()          //设置图片的路径
{
    TButton *ButtoPixmap = dynamic_cast<TButton *>(mItem);
    QString pixmapPath;
    if(ButtoPixmap)
        pixmapPath=QFileDialog::getOpenFileName(this,
    "Select one or more files to open", QDir::currentPath(), tr("Image (*.png *.jpg)"));
    else
    {
        pixmapPath=QFileDialog::getOpenFileName(this,
    "Select one or more files to open", QDir::currentPath());

    }
    if(pixmapPath.isEmpty())
    {
        return;
    }

    if(ButtoPixmap)
    {
        mLineEditPixmapButtonDir->setText(pixmapPath);

    }
    TPixmap *Pixmap = dynamic_cast<TPixmap *>(mItem);
    if(NULL == Pixmap)
    {
        return;
    }

    Pixmap->fSetFileDir(pixmapPath);
    if(sender()->objectName()==tr("mButtonPixmap"))
    {
           mLineEditPixmapDir->setText(pixmapPath);

    }
    if(sender()->objectName()==tr("mButtonPixmap1"))
    {
           mLineEditPixmapDir1->setText(pixmapPath);
    }
    if(sender()->objectName()==tr("mButtonPixmap2"))
    {
           mLineEditPixmapDir2->setText(pixmapPath);
    }
}

DialogStyle::DialogStyle(TItem *Item, const QList<TService *> &ServiceList, QWidget *parent) :
    QDialog(parent)
{
    // 初始化
    fInit();

    mItem = Item;

    if(NULL == mItem)
    {
        mInit = false;
        return;
    }
    else
    {
        mInit = true;
    }

    fCreateItem(ServiceList);
    fCreateText();
    fCreateValueText();
    fCreatePixmap();
    fCreateAngle();
    fCreateTable();
    fCreateButton();
    fCreateMeter();
    fCreateProcessBar();

    mButtonOK = TBase::fButton(tr("应用"), tr("mButtonOK"), 75, 36);
    connect(mButtonOK, SIGNAL(clicked()), this, SLOT(fApply()));
    mButtonCancel = TBase::fButton(tr("关闭"), tr("mButtonCancel"), 75, 36);
    connect(mButtonCancel, SIGNAL(clicked()), this, SLOT(fClose()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonOK);
    mLayoutButton->addWidget(mButtonCancel);

    TabWidget = new QTabWidget();

    if(mGroupBoxPos)
    {
        TabWidget->addTab(mGroupBoxPos, tr("坐标"));
    }

    if(mGroupBoxStyle)
    {
        TabWidget->addTab(mGroupBoxStyle, tr("样式"));
    }
    if(mGroupBoxConfig)
    {
        TabWidget->addTab(mGroupBoxConfig, tr("扩展属性"));
    }
    if(mLinkList)
    {
        TabWidget->addTab(mLinkList, tr("联动列表"));
    }
    if(mGroupBoxValueText)
    {
        TabWidget->addTab(mGroupBoxValueText, tr("值文本"));
    }

    if(mGroupBoxText)
    {
        TabWidget->addTab(mGroupBoxText, tr("文本"));
    }

    if(mGroupBoxPixmap)
    {
        TabWidget->addTab(mGroupBoxPixmap, tr("图片"));
    }

    if(mGroupBoxAngle)
    {
        TabWidget->addTab(mGroupBoxAngle, tr("弧度"));
    }

    if(mGroupBoxButton)
    {
        TabWidget->addTab(mGroupBoxButton, tr("按钮"));
    }

    if(mGroupBoxProcessBar)
    {
        TabWidget->addTab(mGroupBoxProcessBar, tr("刻度条"));
    }

    if(mGroupBoxMeter)
    {
        TabWidget->addTab(mGroupBoxMeter, tr("仪表盘"));
    }

    if(mGroupBoxTable)
    {
        TabWidget->addTab(mGroupBoxTable, tr("表格"));
    }

    QVBoxLayout *mLayoutMain = new QVBoxLayout(this);

    mLayoutMain->addWidget(TabWidget);
    mLayoutMain->addStretch();
    mLayoutMain->addLayout(mLayoutButton);

    setLayout(mLayoutMain);

    mButtonOK->setFocus();
    mButtonOK->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOK->setShortcut(Qt::Key_Enter);
    mButtonOK->setShortcut(Qt::Key_Return);

    setWindowTitle(tr("设置样式"));
    setFixedSize(sizeHint());
}

void DialogStyle::fChooseLineColor()
{
    fChooseColor(mLabelLineColor, mLineColor);
}

void DialogStyle::fChooseLineWidth(int Width)
{
    TBase::fColorLabel(mLabelLineWidth, Qt::black, mLabelLineWidth->width(), Width);
}

void DialogStyle::fChooseBackColor()
{
    fChooseColor(mLabelBackColor, mBackColor);
}

void DialogStyle::fChooseValueColor()
{
    fChooseColor(mLabelProcessBarValueColor, mProcessBarValueColor);
}

void DialogStyle::fChoosePointerColor()
{
    fChooseColor(mLabelMeterPointerColor, mMeterPointerColor);
}

void DialogStyle::fChooseColor(QLabel *Label, QColor &Color)
{
    if(TBase::fChooseColor(Color, this))
    {        
        TBase::fColorLabel(Label, Color);
    }
}

void DialogStyle::fClose()
{
    close();
}

bool DialogStyle::fSaveItem()
{
    TItem *Item = mItem;

    if(NULL == Item)
    {
        return false;
    }

    Item->fMoveTo(mSpinBoxStartX->value(), mSpinBoxStartY->value(), mSpinBoxStopX->value(), mSpinBoxStopY->value());
    Item->fSetZValue(mSpinBoxZ->value());
    // 先设定中心再旋转
    Item->fSetRotateCenter(mSpinBoxRotateX->value(), mSpinBoxRotateY->value());
    Item->fSetRotateAngle(mSpinBoxRotateAngle->value());

    // 设定线色
    mLineColor.setAlpha(mSpinBoxLineAlpha->value());

    if(mLineColor != Item->fGetLineColor())
    {
        Item->fSetLineColor(mLineColor);
    }

    // 设定线型
    int LineStyle = mComboBoxLineStyle->currentIndex();
    if(LineStyle < 0)
    {
        LineStyle = 0;
    }

    Qt::PenStyle mPenStyle = Qt::PenStyle(LineStyle);

    if(mPenStyle != Item->fGetLineStyle())
    {
        Item->fSetLineStyle(mPenStyle);
    }

    int LineWidth = mSpinBoxLineWidth->value();

    if(LineWidth != Item->fGetLineWidth())
    {
        Item->fSetLineWidth(LineWidth);
    }

    mBackColor.setAlpha(mSpinBoxBackAlpha->value());

    if(mBackColor != Item->fGetBackGroundColor())
    {
        Item->fSetBackGroundColor(mBackColor);
    }
    TItemEx *ItemEx = dynamic_cast<TItemEx *>(mItem);
    if(NULL == ItemEx)
    {
        return false;
    }

    ItemEx->fSetNameId(mLineEditNameId->text());
    ItemEx->fSetServiceId(TBase::fGetComboBoxCurrentData(mComboBoxService));
    ItemEx->fSetHint(mTextEditHint->toPlainText());
    ItemEx->showStart=msipnBoxStartRange->value();
    ItemEx->showStop=msipnBoxStopRange->value();
    TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);
    if(Graphics!=NULL)
    {
        Graphics->fSetNodeId(mLineEditSubDeviceNum->value());
        Graphics->fSetMainId(mLineEditMainDeviceNum->value());
        Graphics->fSetGroupId(mSpinBoxGroupId->value());
        Graphics->fSetGroupName(mLabelGroupName->text());
        Graphics->threeDevName=mComboBoxModbusPro->currentText();
        bool ok;
        Graphics->startAddr=mLineEditStartAddr->text().toInt(&ok, 16);
        TGraphics::link link;
        link.currntNum=mLineEditGreaterEqual->value();
        link.groupId=mGroupNum->value();
        link.writeNum=mLineEditWrite->value();
        if(mGroupNum->value()!=0)
        {
            Graphics->greaterEqual.append(link);
        }
        link.currntNum=mLineEditLess->value();
        link.groupId=mGroupLessNum->value();
        link.writeNum=mLineEditLessWrite->value();
        if(mGroupLessNum->value()!=0)
        {
            Graphics->lessThan.append(link);
        }
    }
    TButton *Button = dynamic_cast<TButton *>(mItem);
    if(Button!=NULL)
    {
        TButton::link link;
        link.groupId=mGroupNum->value();
        link.writeNum=mLineEditWrite->value();
        if(mGroupNum->value()!=0)
        {
            Button->greaterEqual.append(link);
        }
        TButton::Control control;
        control.servername=mComboBoxService->currentText();

        control.mainId=mLineEditMainDeviceNum->value();
        control.devId=mLineEditSubDeviceNum->value();
        bool ok;
        control.startAddr=mLineEditStartAddr->text().toInt(&ok, 16);
        control.writeNum=mLineEditLessWrite->value();
        if((control.mainId!=0)&&(!control.servername.isEmpty()))
        {
           Button->control.append(control);
        }

    }
    return true;
}

bool DialogStyle::fSaveText()
{
    TText* Text = dynamic_cast<TText *>(mItem);

    if(NULL == Text)
    {
        return false;
    }

    Text->fSetText(mTextEditText->toPlainText());
    Text->fSetFontFamily(mComboBoxFontFamily->currentText());
    Text->fSetFontSize(mSpinBoxFontSize->value());
    Text->fSetFontBlod(mCheckBoxFontBlob->isChecked());
    Text->fSetFontItalic(mCheckBoxFontItalic->isChecked());
    Text->fSetFontSpacing(mSpinBoxSpacing->value());
    Text->fSetFontUnderLine(mCheckBoxFontUnderLine->isChecked());
    Text->fSetFontStrikeOut(mCheckBoxFontStrikeOut->isChecked());

    return true;
}

bool DialogStyle::fSaveValueText()
{
    TValueText* ValueText = dynamic_cast<TValueText *>(mItem);

    if(NULL == ValueText)
    {
        return false;
    }

    ValueText->fSetMode(mComboBoxTextMode->currentIndex());
    ValueText->fSetIntLen(mSpinBoxTextIntLen->value());
    ValueText->fSetPointLen(mSpinBoxTextPointLen->value());
    ValueText->mDownAlarm=mSpinBoxDownAlarm->value();  //报警
    ValueText->mUpAlarm=mSpinBoxUpAlarm->value();


    return true;
}

bool DialogStyle::fSavePixmap()
{
    TPixmap *Pixmap = dynamic_cast<TPixmap *>(mItem);

    if(NULL == Pixmap)
    {
        return false;
    }
    Pixmap->mFileDir.clear();
    Pixmap->condiation.clear();
    //Pixmap->fSetFileDir(mLineEditPixmapDir->text());
    Pixmap->mFileDir.push_back(mLineEditPixmapDir->text());
    Pixmap->mFileDir.push_back(mLineEditPixmapDir1->text());
    Pixmap->mFileDir.push_back(mLineEditPixmapDir2->text());


    Pixmap->condiation.push_back(mComboxCondition->currentIndex());
    Pixmap->condiation.push_back(mComboxCondition1->currentIndex());
    Pixmap->condiation.push_back(mComboxCondition2->currentIndex());
    qDebug()<<mComboxCondition->currentIndex();

    return true;
}

bool DialogStyle::fSaveAngle()
{
    TArc *Arc = dynamic_cast<TArc *>(mItem);

    if(NULL == Arc)
    {
        return false;
    }

    Arc->fSetMode(mComboBoxArcMode->currentIndex());
    Arc->fSetStartAngle(mSpinBoxStartAngle->value());
    Arc->fSetCoverAngle(mSpinBoxCoverAngle->value());

    return true;
}

void DialogStyle::fApply()
{
    fSaveItem();
    fSaveText();
    fSaveValueText();
    fSavePixmap();
    fSaveAngle();
    fSaveTable();
    fSaveButton();
    fSaveMeter();
    fSaveProcessBar();

    mItem->fUpdate();

    //close();
}

DialogStyle::~DialogStyle()
{}
void DialogStyle::modSendOrder()
{
    QPushButton *button=qobject_cast<QPushButton *>(sender());
    if(button) {
        QString order=button->objectName();
        if(order.left(1).toInt()==1) {
            TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);
            if(Graphics) {
                //Graphics->greaterEqual.removeAt(order.right(order.length()-2).toInt());
                TGraphics::link link=Graphics->greaterEqual.at(order.right(order.length()-2).toInt());
                ModOrder modOrder(1, link.currntNum, link.groupId, link.writeNum);
                modOrder.exec();

                if(modOrder.isSave)
                {
                    Graphics->greaterEqual[order.right(order.length()-2).toInt()].currntNum=modOrder.mEditButton1_1->text().toInt();
                    Graphics->greaterEqual[order.right(order.length()-2).toInt()].groupId=modOrder.mEditButton1_2->text().toInt();
                    Graphics->greaterEqual[order.right(order.length()-2).toInt()].writeNum=modOrder.mEditButton1_3->text().toDouble();
                }
                updateLabel();
            }
        }
        if(order.left(1).toInt()==2) {
            TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);
            if(Graphics) {
                //Graphics->lessThan.removeAt(order.right(order.length()-2).toInt());

                TGraphics::link link=Graphics->lessThan.at(order.right(order.length()-2).toInt());
                ModOrder modOrder(2, link.currntNum, link.groupId, link.writeNum);
                modOrder.exec();

                if(modOrder.isSave)
                {
                    Graphics->lessThan[order.right(order.length()-2).toInt()].currntNum=modOrder.mEditButton1_1->text().toInt();
                    Graphics->lessThan[order.right(order.length()-2).toInt()].groupId=modOrder.mEditButton1_2->text().toInt();
                    Graphics->lessThan[order.right(order.length()-2).toInt()].writeNum=modOrder.mEditButton1_3->text().toDouble();
                }
                updateLabel();
            }
        }
        if(order.left(1).toInt()==3) {
            TButton *Button = dynamic_cast<TButton *>(mItem);
            if(Button!=NULL) {
                //Button->greaterEqual.removeAt(order.right(order.length()-2).toInt());
                //qDebug()<<Button->greaterEqual.size();

                TButton::link link=Button->greaterEqual.at(order.right(order.length()-2).toInt());
                ModOrder modOrder(3, link.groupId, link.writeNum);

                modOrder.exec();

                if(modOrder.isSave)
                {
                    //Button->greaterEqual[order.right(order.length()-2).toInt()].currntNum=modOrder.mEditButton1_1->text().toInt();
                    Button->greaterEqual[order.right(order.length()-2).toInt()].groupId=modOrder.mEditButton1_2->text().toInt();
                    Button->greaterEqual[order.right(order.length()-2).toInt()].writeNum=modOrder.mEditButton1_3->text().toDouble();
                }

                updateLabel();
            }
        }
        if(order.left(1).toInt()==4) {
            TButton *Button = dynamic_cast<TButton *>(mItem);
            if(Button!=NULL) {
                //Button->control.removeAt(order.right(order.length()-2).toInt());
                //qDebug()<<Button->control.size();

                TButton::Control link=Button->control.at(order.right(order.length()-2).toInt());
                ModOrder modOrder(4, link.mainId, link.devId, link.writeNum, link.startAddr, link.servername);

                modOrder.exec();

                if(modOrder.isSave)
                {
                    //Button->greaterEqual[order.right(order.length()-2).toInt()].currntNum=modOrder.mEditButton1_1->text().toInt();
                    Button->control[order.right(order.length()-2).toInt()].servername=modOrder.mComboBoxService->currentText();
                    Button->control[order.right(order.length()-2).toInt()].mainId=modOrder.mEditButton1_1->text().toInt();
                    Button->control[order.right(order.length()-2).toInt()].devId=modOrder.mEditButton1_3->text().toInt();
                    bool ok;
                    Button->control[order.right(order.length()-2).toInt()].startAddr=modOrder.mEditButton1_2->text().toInt(&ok, 16);
                    Button->control[order.right(order.length()-2).toInt()].writeNum=modOrder.mEditButton1_4->text().toDouble();
                }
                updateLabel();
            }
        }
    }
}

void DialogStyle::deleteContral()
{
    QPushButton *button=qobject_cast<QPushButton *>(sender());
    if(button) {
        QString order=button->objectName();
        if(order.left(1).toInt()==1) {
            TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);
            if(Graphics) {
                Graphics->greaterEqual.removeAt(order.right(order.length()-2).toInt());
                updateLabel();
            }
        }
        if(order.left(1).toInt()==2) {
            TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);
            if(Graphics) {
                Graphics->lessThan.removeAt(order.right(order.length()-2).toInt());
                updateLabel();
            }
        }
        if(order.left(1).toInt()==3) {
            TButton *Button = dynamic_cast<TButton *>(mItem);
            if(Button!=NULL) {
                Button->greaterEqual.removeAt(order.right(order.length()-2).toInt());
                //qDebug()<<Button->greaterEqual.size();
                updateLabel();
            }
        }
        if(order.left(1).toInt()==4) {
            TButton *Button = dynamic_cast<TButton *>(mItem);
            if(Button!=NULL) {
                Button->control.removeAt(order.right(order.length()-2).toInt());
                //qDebug()<<Button->control.size();
                updateLabel();
            }
        }
    }
}
void DialogStyle::updateLabel()
{
    int index=TabWidget->indexOf(mLinkList);
    delete mLinkList;
    mLinkList=nullptr;
    TGraphics *Graphics = dynamic_cast<TGraphics *>(mItem);

    if(Graphics)
    {
        mLinkList=new QGroupBox(tr(""));
        QGridLayout *linkLayout = new QGridLayout();
        int i=0;
        int Mynum=0;
        foreach (TGraphics::link link, Graphics->greaterEqual) {
            linkLayout->addWidget(TBase::fLabel(tr("大于"), tr("mLabelGreaterEqual")), i, 0);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.currntNum), tr("mLabelGreaterEqual")), i, 1);

            linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.groupId), tr("mLabelGreaterWrite")), i, 3);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 4);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.writeNum), tr("mLabelGreaterWrite")), i, 5);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            button->setObjectName("1:"+QString::number(Mynum));
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));

            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("1:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 6);
            linkLayout->addWidget(button , i, 7);
            Mynum++;
            i++;
        }
        Mynum=0;
        foreach (TGraphics::link link, Graphics->lessThan) {
            linkLayout->addWidget(TBase::fLabel(tr("小于"), tr("mLabelGreaterEqual")), i, 0);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.currntNum), tr("mLabelGreaterEqual")), i, 1);
            linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.groupId), tr("mLabelGreaterWrite")), i, 3);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 4);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.writeNum), tr("mLabelGreaterWrite")), i, 5);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            button->setObjectName("2:"+QString::number(Mynum));
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));

            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("2:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 6);
            linkLayout->addWidget(button , i, 7);
            Mynum++;
            i++;
        }

        QVBoxLayout *LinkTotal = new QVBoxLayout(mLinkList);
        LinkTotal->addLayout(linkLayout);
        LinkTotal->addStretch();

    }
    TButton *Button = dynamic_cast<TButton *>(mItem);
    if(Button!=NULL)
    {
        mLinkList=new QGroupBox(tr(""));
        QGridLayout *linkLayout = new QGridLayout();
        int i=0;
        int Mynum=0;
        foreach (TButton::link link, Button->greaterEqual) {
            linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), i, 0);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.groupId), tr("mLabelGreaterWrite")), i, 1);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(link.writeNum), tr("mLabelGreaterWrite")), i, 3);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            button->setObjectName("3:"+QString::number(Mynum));
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));
            QPushButton *buttonmod=new QPushButton(tr("修改"));

            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("3:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 4);


            linkLayout->addWidget(button , i, 5);
            Mynum++;
            i++;
        }
        Mynum=0;
        foreach(TButton::Control contral, Button->control)
        {
            linkLayout->addWidget(TBase::fLabel(tr("服务"), tr("mLabelGreaterWrite")), i, 0);
            linkLayout->addWidget(TBase::fLabel(contral.servername, tr("mLabelGreaterWrite")), i, 1);

            linkLayout->addWidget(TBase::fLabel(tr("主设备"), tr("mLabelGreaterWrite")), i, 2);
            linkLayout->addWidget(TBase::fLabel(QString::number(contral.mainId), tr("mLabelGreaterWrite")), i, 3);
            linkLayout->addWidget(TBase::fLabel(tr("通道号"), tr("mLabelGreaterWrite")), i, 4);
            linkLayout->addWidget(TBase::fLabel(QString::number(contral.devId), tr("mLabelGreaterWrite")), i, 5);
            linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), i, 6);
            linkLayout->addWidget(TBase::fLabel(QString::number(contral.writeNum), tr("mLabelGreaterWrite")), i, 7);
            QPushButton *button=new QPushButton();
            button->setIcon(button->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
            button->setMaximumSize(40, 40);
            connect(button, SIGNAL(clicked()), this, SLOT(deleteContral()));
            button->setObjectName("4:"+QString::number(Mynum));
            QPushButton *buttonmod=new QPushButton(tr("修改"));
            buttonmod->setMaximumSize(40, 40);
            buttonmod->setObjectName("4:"+QString::number(Mynum));
            connect(buttonmod, SIGNAL(clicked()), this, SLOT(modSendOrder()));

            linkLayout->addWidget(buttonmod , i, 8);

            linkLayout->addWidget(button , i, 9);
            Mynum++;
            i++;
        }

        QVBoxLayout *LinkTotal = new QVBoxLayout(mLinkList);
        LinkTotal->addLayout(linkLayout);
        LinkTotal->addStretch();
    }
    if(mLinkList)
    {
        TabWidget->insertTab(index, mLinkList, tr("联动列表"));
        TabWidget->setCurrentWidget(mLinkList);
    }
}
