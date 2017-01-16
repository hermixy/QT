#include "battery.h"
#include <QDebug>

Battery::Battery(QWidget *parent) : QMainWindow(parent)
{
    QWidget *mWidget = new QWidget(this);
    mWidget->setObjectName(QStringLiteral("mWidget"));
    setCentralWidget(mWidget);

    QGridLayout *mLayoutImage = new QGridLayout(mWidget);

    for(int i = 0; i < 16; ++i)
    {
        mLabelArray[i] = TBase::fLabel(tr(""), tr("mLabelArray") + QString::number(i), 42, 72);
        mLabelArray[i]->setScaledContents(true);
        //mImageArray[i] = new QImage(":/root/pic/g100.png");
        mLabelArray[i]->setPixmap(QPixmap(tr(":/root/pic/g100.png")));

        mLabelData[i] = TBase::fLabel(tr(""), tr("mLabelData") + QString::number(i));
        mLabelData[i]->setText(tr("100%"));

        mLayoutImage->addWidget(mLabelArray[i], (i / 8) * 2, i % 8);
        mLayoutImage->addWidget(mLabelData[i], (i / 8) * 2 + 1, i % 8, 1, 1, Qt::AlignCenter);
    }

    mSpinData = TBase::fDoubleSpinBox(tr("mSpinData"), 0, 100, 100, 24);
    mSpinData->setValue(100);
    mSpinData->setSingleStep(10);

    mLayoutImage->addWidget(mSpinData, 0, 10);

    connect(mSpinData, SIGNAL(valueChanged(double)), this, SLOT(fDataChange(double)));

    setWindowTitle(tr("电池服务程序"));
}

void Battery::fDataChange(double Data)
{
    QString ImageDir;

    if(Data <= 10)
    {
        ImageDir = tr(":/root/pic/r10.png");
    }
    else if(Data <= 20)
    {
        ImageDir = tr(":/root/pic/r20.png");
    }
    else if(Data <= 30)
    {
        ImageDir = tr(":/root/pic/r30.png");
    }
    else if(Data <= 40)
    {
        ImageDir = tr(":/root/pic/g40.png");
    }
    else if(Data <= 50)
    {
        ImageDir = tr(":/root/pic/g50.png");
    }
    else if(Data <= 60)
    {
        ImageDir = tr(":/root/pic/g60.png");
    }
    else if(Data <= 70)
    {
        ImageDir = tr(":/root/pic/g70.png");
    }
    else if(Data <= 80)
    {
        ImageDir = tr(":/root/pic/g80.png");
    }
    else if(Data <= 90)
    {
        ImageDir = tr(":/root/pic/g90.png");
    }
    else
    {
        ImageDir = tr(":/root/pic/g100.png");
    }

    mLabelArray[0]->setPixmap(QPixmap(ImageDir));
    mLabelData[0]->setText(QString::number(Data) + tr("%"));
}
