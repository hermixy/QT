#include "DialogJsHelper.h"

#include "../Scada/Src/File/File.h"

#include <QColorDialog>
#include <QFileDialog>

DialogJsHelper::DialogJsHelper(QWidget *parent) : QDialog(parent)
{
    //setAttribute(Qt::WA_DeleteOnClose);

    mGroupBoxType = new QGroupBox(tr("脚本用途"));
    mComboBoxType = TBase::fComboBox(tr("mComboBoxType"), 200, 22);
    mComboBoxType->addItem(tr("值变化"));
    mComboBoxType->addItem(tr("定时任务"));
    mComboBoxType->addItem(tr("联动脚本"));
    mLabelTypeHint = TBase::fLabel(tr("不同脚本不能混用"), tr("mLabelTypeHint"));

    QHBoxLayout *mLayoutType = new QHBoxLayout(mGroupBoxType);
    mLayoutType->addStretch();
    mLayoutType->addWidget(mComboBoxType);
    mLayoutType->addWidget(mLabelTypeHint);
    mLayoutType->addStretch();

    mGroupBoxCondition = new QGroupBox(tr("条件"));
    mComboBoxCondition = TBase::fComboBox(tr("mComboBoxCondition"), 120, 22);
    mComboBoxCondition->addItem(tr("大于"));
    mComboBoxCondition->addItem(tr("大于或等于"));
    mComboBoxCondition->addItem(tr("等于"));
    mComboBoxCondition->addItem(tr("小于"));
    mComboBoxCondition->addItem(tr("小于或等于"));
    mComboBoxCondition->addItem(tr("介于两值之间"));
    mComboBoxCondition->addItem(tr("未介于两值"));
    mComboBoxCondition->addItem(tr("具备所有联动条件"));
    mComboBoxCondition->addItem(tr("否则"));
    connect(mComboBoxCondition, SIGNAL(currentIndexChanged(int)), this, SLOT(fComboBoxConditionChanged(int)));

    mSpinBoxCondition = TBase::fDoubleSpinBox(tr("mSpinBoxCondition"));
    mSpinBoxCondition->setFixedSize(75, 22);
    //mSpinBoxCondition->setValue(0);
    mSpinBoxConditionEnd = TBase::fDoubleSpinBox(tr("mSpinBoxConditionEnd"));
    mSpinBoxConditionEnd->setFixedSize(75, 22);
    //mSpinBoxConditionEnd->setValue(0);
    mSpinBoxConditionEnd->setVisible(false);

    QHBoxLayout *mLayoutCondition = new QHBoxLayout(mGroupBoxCondition);
    mLayoutCondition->addWidget(mComboBoxCondition);
    mLayoutCondition->addWidget(mSpinBoxCondition);
    mLayoutCondition->addWidget(mSpinBoxConditionEnd);

    mGroupBoxAction = new QGroupBox(tr("动作"));
    mComboBoxAction = TBase::fComboBox(tr("mComboBoxAction"));
    mComboBoxAction->addItem(tr("改变线条样式"));
    mComboBoxAction->addItem(tr("改变线条宽度"));
    mComboBoxAction->addItem(tr("改变线条颜色"));
    mComboBoxAction->addItem(tr("改变填充颜色"));
    mComboBoxAction->addItem(tr("移至绝对位置"));
    mComboBoxAction->addItem(tr("移动相对位置"));
    mComboBoxAction->addItem(tr("改至绝对大小"));
    mComboBoxAction->addItem(tr("改变相对大小"));
    mComboBoxAction->addItem(tr("启动报警"));
    mComboBoxAction->addItem(tr("发送联动信号"));
    mComboBoxAction->addItem(tr("发送取消联动信号"));
    mComboBoxAction->addItem(tr("启动复归"));
    connect(mComboBoxAction, SIGNAL(currentIndexChanged(int)), this, SLOT(fComboBoxActionChanged(int)));

    mComboBoxLineStyle = TBase::fLineStyleComboBox();
    mSpinBoxLineWidth = TBase::fSpinBox(tr("mSpinBoxLineWidth"), 1, 100, 50, 22);
    mLabelColor = TBase::fLabel(tr(""), tr("mLabelColor"), 50, 22);
    mButtonColor= TBase::fButton(tr("浏览"), tr("mButtonColor"), 36, 22);
    connect(mButtonColor, SIGNAL(clicked()), this, SLOT(fButtonColorClick()));
    mSpinBoxStart = TBase::fDoubleSpinBox(tr("mSpinBoxStart"), 75, 22);
    mSpinBoxStop = TBase::fDoubleSpinBox(tr("mSpinBoxStop"), 75, 22);

    mComboBoxLineStyle->setVisible(true);
    mSpinBoxLineWidth->setVisible(false);
    mLabelColor->setVisible(false);
    mButtonColor->setVisible(false);
    mSpinBoxStart->setVisible(false);
    mSpinBoxStop->setVisible(false);

    QGridLayout *mLayoutAction = new QGridLayout();
    mLayoutAction->addWidget(mComboBoxLineStyle,0, 0, 1, 2);
    mLayoutAction->addWidget(mSpinBoxLineWidth, 1, 0, 1, 2);
    mLayoutAction->addWidget(mLabelColor, 2, 0);
    mLayoutAction->addWidget(mButtonColor, 2, 1);
    mLayoutAction->addWidget(mSpinBoxStart, 3, 0);
    mLayoutAction->addWidget(mSpinBoxStop, 3, 1);

    QHBoxLayout *mLayoutActionGroup = new QHBoxLayout(mGroupBoxAction);
    mLayoutActionGroup->addWidget(mComboBoxAction);
    mLayoutActionGroup->addLayout(mLayoutAction);

    //mTextEdit = new QTextEdit();
    mTextEdit = TBase::fTextEdit(tr("mTextEdit"));
    //mTextEdit->setReadOnly(true);

    mPushButtonAddCondition = TBase::fButton(tr("添加条件"), tr("mPushButtonAddCondition"), 75, 36);
    connect(mPushButtonAddCondition, SIGNAL(clicked()), this, SLOT(fAddCondition()));
    mPushButtonAddAction = TBase::fButton(tr("添加动作"), tr("mPushButtonAddAction"), 75, 36);
    connect(mPushButtonAddAction, SIGNAL(clicked()), this, SLOT(fAddAction()));

    mPushButtonClear = TBase::fButton(tr("清空"), tr("mPushButtonClear"), 75, 36);
    connect(mPushButtonClear, SIGNAL(clicked()), mTextEdit, SLOT(clear()));

    mPushButtonOK = TBase::fButton(tr("保存模板"), tr("mPushButtonOK"), 75, 36);
    connect(mPushButtonOK, SIGNAL(clicked()), this, SLOT(fAddTemplate()));
    mPushButtonClose = TBase::fButton(tr("关闭"), tr("mPushButtonClose"), 75, 36);
    connect(mPushButtonClose, SIGNAL(clicked()), this, SLOT(close()));    

    QGridLayout *mLayoutUp = new QGridLayout();
    mLayoutUp->addWidget(mGroupBoxType, 0, 0, 1, 3);
    mLayoutUp->addWidget(mGroupBoxCondition, 1, 0, 1, 2);
    mLayoutUp->addWidget(mPushButtonAddCondition, 1, 2);
    mLayoutUp->addWidget(mGroupBoxAction, 2, 0, 1, 2);
    mLayoutUp->addWidget(mPushButtonAddAction, 2, 2);
    mLayoutUp->addWidget(mTextEdit, 3, 0, 1, 3);

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mPushButtonClear);
    mLayoutButton->addWidget(mPushButtonOK);
    mLayoutButton->addWidget(mPushButtonClose);

    QVBoxLayout *mLayoutRight = new QVBoxLayout();
    mLayoutRight->addLayout(mLayoutUp);
    mLayoutRight->addLayout(mLayoutButton);

    // 示例
    mLabelSample = TBase::fLabel(tr("脚本示例"), tr("mLabelSample"));
    mTextEditSample = TBase::fTextEdit(tr("mTextEditSample"));

    fInitSampleText();

    QVBoxLayout *mLayoutLeft = new QVBoxLayout();
    mLayoutLeft->addWidget(mLabelSample);
    mLayoutLeft->addWidget(mTextEditSample);

    QHBoxLayout *mLayoutMain = new QHBoxLayout(this);
    mLayoutMain->addLayout(mLayoutRight);
    mLayoutMain->addLayout(mLayoutLeft);

    mPushButtonOK->setFocus();
    mPushButtonOK->setShortcut(QKeySequence::InsertParagraphSeparator);
    mPushButtonOK->setShortcut(Qt::Key_Enter);
    mPushButtonOK->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("脚本助手"));
    //setFixedSize(sizeHint().width(), sizeHint().height());
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    mColor = Qt::black;
}

void DialogJsHelper::fInitSampleText()
{
    if(NULL == mTextEditSample)
    {
        return;
    }

    mTextEditSample->insertPlainText(tr("\\\\ 自身动画 脚本示例\n"));
    mTextEditSample->insertPlainText(tr("function fPlay(Item)\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("var Value = Item.fGetValue();\n\n"));
    mTextEditSample->insertPlainText(tr("if(Value > 0)\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetLineColor(255, 0, 0);\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetBackGroundColor(255, 0, 0);\n"));
    mTextEditSample->insertPlainText(tr("}\n"));
    mTextEditSample->insertPlainText(tr("else\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetLineColor(0, 255, 0);\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetBackGroundColor(0, 255, 0);\n"));
    mTextEditSample->insertPlainText(tr("}\n\n"));

    mTextEditSample->insertPlainText(tr("if(Value > 0)\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("Item.fStartAlarm();\n"));
    mTextEditSample->insertPlainText(tr("}\n\n"));

    mTextEditSample->insertPlainText(tr("Item.fUpdate();\n"));
    mTextEditSample->insertPlainText(tr("}\n\n"));

    mTextEditSample->insertPlainText(tr("\\\\ 联动动画 脚本示例\n"));
    mTextEditSample->insertPlainText(tr("function fDst(Item)\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("if(Item.fGetLinkageRecv() == Item.fGetLinkageActive())\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetLineColor(255, 0, 0);\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetBackGroundColor(255, 0, 0);\n"));
    mTextEditSample->insertPlainText(tr("}\n"));
    mTextEditSample->insertPlainText(tr("else\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetLineColor(0, 255, 0);\n"));
    mTextEditSample->insertPlainText(tr("Item.fSetBackGroundColor(0, 255, 0);\n"));
    mTextEditSample->insertPlainText(tr("}\n\n"));
    mTextEditSample->insertPlainText(tr("}\n\n"));

    mTextEditSample->insertPlainText(tr("\\\\ 调用其他图形 脚本示例\n"));
    mTextEditSample->insertPlainText(tr("function fDst(Item)\n"));
    mTextEditSample->insertPlainText(tr("{\n"));
    mTextEditSample->insertPlainText(tr("Item.fGetHostScene().fGetItemByNameId('qiangong').fDoRotate(-10);\n"));
    mTextEditSample->insertPlainText(tr("}\n\n"));
}

void DialogJsHelper::fComboBoxConditionChanged(int Index)
{
    switch(Index)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        mSpinBoxCondition->setVisible(true);
        mSpinBoxConditionEnd->setVisible(false);
        break;

    case 5:
    case 6:
        mSpinBoxCondition->setVisible(true);
        mSpinBoxConditionEnd->setVisible(true);
        break;

    case 7:
    case 8:
        mSpinBoxCondition->setVisible(false);
        mSpinBoxConditionEnd->setVisible(false);
        break;

    default:
        break;
    }
}

void DialogJsHelper::fButtonColorClick()
{    
    if(TBase::fChooseColor(mColor, this))
    {
        TBase::fColorLabel(mLabelColor, mColor);
    }
}

void DialogJsHelper::fComboBoxActionChanged(int Index)
{
    switch(Index)
    {
    case 0: // 改变线条样式
        mComboBoxLineStyle->setVisible(true);
        mSpinBoxLineWidth->setVisible(false);
        mLabelColor->setVisible(false);
        mButtonColor->setVisible(false);
        mSpinBoxStart->setVisible(false);
        mSpinBoxStop->setVisible(false);
        break;

    case 1: // 改变线条宽度
        mComboBoxLineStyle->setVisible(false);
        mSpinBoxLineWidth->setVisible(true);
        mLabelColor->setVisible(false);
        mButtonColor->setVisible(false);
        mSpinBoxStart->setVisible(false);
        mSpinBoxStop->setVisible(false);
        break;

    case 2: // 改变线条颜色
    case 3: // 改变填充颜色
        mComboBoxLineStyle->setVisible(false);
        mSpinBoxLineWidth->setVisible(false);
        TBase::fColorLabel(mLabelColor, mColor);
        mLabelColor->setVisible(true);
        mButtonColor->setVisible(true);
        mSpinBoxStart->setVisible(false);
        mSpinBoxStop->setVisible(false);
        break;

    case 4: // 移至绝对位置
    case 5: // 移动相对位置
    case 6: // 改至绝对大小
    case 7: // 改变相对大小
        mComboBoxLineStyle->setVisible(false);
        mSpinBoxLineWidth->setVisible(false);
        mLabelColor->setVisible(false);
        mButtonColor->setVisible(false);
        mSpinBoxStart->setVisible(true);
        mSpinBoxStop->setVisible(true);
        break;

    case 8:     // 报警
    case 9:     // 发送联动信号
    case 10:    // 发送取消联动信号
    case 11:    // 复归
        mComboBoxLineStyle->setVisible(false);
        mSpinBoxLineWidth->setVisible(false);
        mLabelColor->setVisible(false);
        mButtonColor->setVisible(false);
        mSpinBoxStart->setVisible(false);
        mSpinBoxStop->setVisible(false);
        break;

    default:
        break;
    }
}

void DialogJsHelper::fAddCondition()
{
    QString mString = tr("");

    if(0 != mTextEdit->toPlainText().length())
    {
        mString += tr("} \nelse ");
    }

    switch(mComboBoxCondition->currentIndex())
    {
    //大于
    case 0:
        mString += tr("if(Value > ") + QString::number(mSpinBoxCondition->value()) + tr(")");
        break;

    //大于或等于
    case 1:
        mString += tr("if(Value >= ") + QString::number(mSpinBoxCondition->value()) + tr(")");
        break;

    //等于
    case 2:
        mString += tr("if(") + QString::number(mSpinBoxCondition->value()) + tr(" == Value") + tr(")");
        break;

    //小于
    case 3:
        mString += tr("if(Value < ") + QString::number(mSpinBoxCondition->value()) + tr(")");
        break;

    //小于或等于
    case 4:
        mString += tr("if(Value <= ") + QString::number(mSpinBoxCondition->value()) + tr(")");
        break;

    //介于两值之间
    case 5:
        mString += tr("if(Value >= ") + QString::number(mSpinBoxCondition->value())
                                        + tr(" && Value <= ") + QString::number(mSpinBoxConditionEnd->value()) + tr(")");
        break;

    //未介于两值
    case 6:
        mString += tr("if(Value < ") + QString::number(mSpinBoxCondition->value())
                                        + tr(" && Value > ") + QString::number(mSpinBoxConditionEnd->value()) + tr(")");
        break;

    // 具备所有联动条件
    case 7:
        mString += tr("if(Item.fGetLinkageRecv() == Item.fGetLinkageActive())");
        break;

    default:
        break;
    }

    mString += tr("\n{\n");

    mTextEdit->insertPlainText(mString);
}

void DialogJsHelper::fAddAction()
{
    QString mString = tr("");

    mString += tr("\t");
    switch(mComboBoxAction->currentIndex())
    {
    case 0: // 改变线条样式
        mString += tr("Item.fSetLineStyle(") + QString::number(mComboBoxLineStyle->currentIndex() + 1) + tr(");");
        break;

    case 1: // 改变线条宽度
        mString += tr("Item.fSetLineWidth(") + QString::number(mSpinBoxLineWidth->value()) + tr(");");
        break;

    case 2: // 改变线条颜色
        mString += tr("Item.fSetLineColor(") + QString::number(mColor.red()) + tr(", ")
                + QString::number(mColor.green()) + tr(", ")
                + QString::number(mColor.blue()) + tr(");");
        break;

    case 3: // 改变填充颜色
        mString += tr("Item.fSetBackGroundColor(") + QString::number(mColor.red()) + tr(", ")
                + QString::number(mColor.green()) + tr(", ")
                + QString::number(mColor.blue()) + tr(");");
        break;

    case 4: // 移至绝对位置
    case 5: // 移动相对位置
    case 6: // 改至绝对大小
    case 7: // 改变相对大小
        break;

    case 8: // 报警
        mString += tr("Item.fStartAlarm();");
        break;

    case 9: // 发送联动信号
        mString += tr("Item.fSendLinkage(true);");
        break;

    case 10: // 发送取消联动信号
        mString += tr("Item.fSendLinkage(false);");
        break;

    case 11: // 启动复归
        mString += tr("Item.fSetValue(0);\n\tItem.fStartAuto();");
        break;

    default:
        break;
    }

    mString += tr("\n");

    mTextEdit->insertPlainText(mString);
}

void DialogJsHelper::fAddTemplate()
{
    QString FileDir = QFileDialog::getSaveFileName(this, tr("保存"));

    int TextType = mComboBoxType->currentIndex();

    QString FunName;

    switch(TextType)
    {
    case 0: // 自身动画
        FunName = tr("fPlay");
        break;

    case 1: // 报警条件
        FunName = tr("fAlarm");
        break;

    case 2: // 联动-发送条件
        FunName = tr("fSrc");
        break;

    case 3: // 联动-启动动画
        FunName = tr("fDst");
        break;

    default:
        FunName = tr("fPlay");
        break;
    }

    QString StrText = tr("function ") + FunName;

    StrText += tr("(Item)\n{\n");

    if(3 !=TextType)
    {
        StrText += tr("var Value = Item.fGetValue();\n\n");
    }

    StrText += mTextEdit->toPlainText();

    StrText += tr("}\n\n");

    if(1 != TextType && 2 != TextType)
    {
        StrText += tr("Item.fUpdate();\n");
    }

    StrText += tr("}");

    if(!FileDir.endsWith(tr(".") + FunName))
    {
        FileDir += tr(".") + FunName;
    }

    TFile::fWriteToFile(FileDir, StrText);
}

DialogJsHelper::~DialogJsHelper()
{}
