#include "modorder.h"
#include "Maker.h"

ModOrder::ModOrder(int order, double i1,double i2, double i3, int i4, QString  i5, QWidget *parent):QDialog(parent)
{
    if(order==1)
    {
        int i=0;
        QGridLayout *linkLayout = new QGridLayout(this);
        linkLayout->addWidget(TBase::fLabel(tr("大于"), tr("mLabelGreaterEqual")), 0, i++);

        mEditButton1_1=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_1->setText(QString::number(i1));
        linkLayout->addWidget(mEditButton1_1, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_2=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_2->setText(QString::number(i2));
        linkLayout->addWidget(mEditButton1_2, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_3=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_3->setText(QString::number(i3));
        linkLayout->addWidget(mEditButton1_3, 0, i++);


        mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
        connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
        mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
        connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

        linkLayout->addWidget(mButtonOk, 0, i++);
        linkLayout->addWidget(mButtonClose, 0, i++);
    }
    if(order==2)
    {
        int i=0;
        QGridLayout *linkLayout = new QGridLayout(this);
        linkLayout->addWidget(TBase::fLabel(tr("小于"), tr("mLabelGreaterEqual")), 0, i++);

        mEditButton1_1=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_1->setText(QString::number(i1));
        linkLayout->addWidget(mEditButton1_1, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_2=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_2->setText(QString::number(i2));
        linkLayout->addWidget(mEditButton1_2, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_3=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_3->setText(QString::number(i3));
        linkLayout->addWidget(mEditButton1_3, 0, i++);


        mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
        connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
        mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
        connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

        linkLayout->addWidget(mButtonOk, 0, i++);
        linkLayout->addWidget(mButtonClose, 0, i++);
    }

    if(order==3)
    {
        int i=0;
        QGridLayout *linkLayout = new QGridLayout(this);


        linkLayout->addWidget(TBase::fLabel(tr("组号"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_2=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_2->setText(QString::number(i1));
        linkLayout->addWidget(mEditButton1_2, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_3=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_3->setText(QString::number(i2));
        linkLayout->addWidget(mEditButton1_3, 0, i++);


        mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
        connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
        mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
        connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

        linkLayout->addWidget(mButtonOk, 0, i++);
        linkLayout->addWidget(mButtonClose, 0, i++);
    }

    if(order==4)
    {
        int i=0;
        QGridLayout *linkLayout = new QGridLayout(this);

        linkLayout->addWidget(TBase::fLabel(tr("服务"), tr("mLabelGreaterWrite")), 0, i++);

        mComboBoxService = TBase::fComboBox(tr("mComboBoxService"));

        if(Maker::maker)
        {
            foreach(TService *Service, Maker::maker->mServiceList)
            {
                mComboBoxService->addItem(Service->fGetName(), QVariant(Service->fGetId()));
            }
        }
        mComboBoxService->setCurrentText(i5);
        linkLayout->addWidget(mComboBoxService, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("主设备"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_1=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_1->setText(QString::number(i1));
        linkLayout->addWidget(mEditButton1_1, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("起始地址"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_2=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_2->setText(QString::number(i4, 16));
        linkLayout->addWidget(mEditButton1_2, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("通道号"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_3=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_3->setText(QString::number(i2));
        linkLayout->addWidget(mEditButton1_3, 0, i++);

        linkLayout->addWidget(TBase::fLabel(tr("写入"), tr("mLabelGreaterWrite")), 0, i++);

        mEditButton1_4=TBase::fLineEdit("mEditButton1_1");
        mEditButton1_4->setText(QString::number(i3));
        linkLayout->addWidget(mEditButton1_4, 0, i++);


        mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"));
        connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
        mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"));
        connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

        linkLayout->addWidget(mButtonOk, 0, i++);
        linkLayout->addWidget(mButtonClose, 0, i++);
    }

}

ModOrder::~ModOrder()
{

}
void ModOrder::fApply()
{
    isSave=true;
    close();
}
