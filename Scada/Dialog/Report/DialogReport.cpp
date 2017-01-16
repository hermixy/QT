#include "DialogReport.h"
#include <QDebug>
#include "../Scada/Src/Word/Word.h"

DialogReport::DialogReport(QWidget *parent) : QDialog(parent)
{
    mLabelShow = TBase::fLabel(tr("214sdg4er65g4er5g46er4"), tr("mLabelShow"), 200, 200);

    mButtonCreate = TBase::fButton(tr("生成"), tr("mButtonCreate"));
    connect(mButtonCreate, SIGNAL(clicked()), this, SLOT(fCreate()));
    mButtonClose = TBase::fButton(tr("关闭"), tr("mButtonClose"), 50, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mLayoutButton = new QHBoxLayout();
    mLayoutButton->addWidget(mButtonCreate);
    mLayoutButton->addWidget(mButtonClose);

    QVBoxLayout *mLayoutMain = new QVBoxLayout(this);
    mLayoutMain->addWidget(mLabelShow);
    mLayoutMain->addLayout(mLayoutButton);

    // 设置默认
    mButtonClose->setFocus();
    mButtonClose->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonClose->setShortcut(Qt::Key_Enter);
    mButtonClose->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("报表"));
    setFixedSize(sizeHint());
}

void DialogReport::fCreate()
{
    TWord Word(tr("E://Test2.dot"));
    Word.fInsertText(tr("test1"), tr("我123654dfghgherhgreegerger78900fdg好的"));

    QPixmap Pixmap(this->size());
    this->render(&Pixmap);

    Word.fInsertPixmap(tr("test2"), Pixmap);

    Word.fSaveAs(tr("E://test12.doc"));
    Word.fClose();
}

DialogReport::~DialogReport()
{}

