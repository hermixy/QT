#include "DialogHint.h"

DialogHint::DialogHint(QWidget *parent) : QDialog(parent)
{
    mTextEditHint = TBase::fTextEdit(tr("mTextEditHint"), 250, 50);

    QGridLayout *mLayoutMain = new QGridLayout();
    mLayoutMain->addWidget(mTextEditHint, 0, 0);

    setLayout(mLayoutMain);

    setWindowTitle("信息");
    //setWindowOpacity(1);//设置通明度
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(sizeHint());
}

void DialogHint::fSetHint(QString Hint)
{
    mTextEditHint->setText(Hint);
}

DialogHint::~DialogHint()
{}
