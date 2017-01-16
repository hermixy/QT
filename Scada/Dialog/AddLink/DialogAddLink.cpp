#include "DialogAddLink.h"


DialogAddLink::DialogAddLink(TWidgets *Graphics, QList<TScene *> SceneList, QWidget *parent) : QDialog(parent)
{    
    mGraphics = dynamic_cast<TButton *>(Graphics);
    if(NULL == mGraphics)
    {
        mInit = false;
        return;
    }

    mInit = true;

    mSceneList = SceneList;

    mComboBoxScene = TBase::fComboBox(tr("mComboBoxScene"));
    mComboBoxScene->addItem(tr("无"));

    foreach(TScene *Scene, mSceneList)
    {
        mComboBoxScene->addItem(Scene->fGetName());
    }

    int index = 0; // 下拉框显示项
    int mLinkId = mGraphics->fGetLinkScene(); // 链接ID

    if(mLinkId > 0)
    {
        for(int i = 0; i < mSceneList.count(); ++i)
        {
            if(mLinkId == mSceneList.at(i)->fGetId())
            {
                index = i + 1;
                break;
            }
        }
    }

    mComboBoxScene->setCurrentIndex(index);

    mButtonOk = TBase::fButton(tr("确定"), tr("mButtonOk"), 75, 36);
    connect(mButtonOk, SIGNAL(clicked()), this, SLOT(fApply()));
    mButtonClose = TBase::fButton(tr("取消"), tr("mButtonClose"), 75, 36);
    connect(mButtonClose, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mLayoutMain = new QGridLayout(this);

    mLayoutMain->addWidget(mComboBoxScene, 0, 0, 1, 2);
    mLayoutMain->addWidget(mButtonOk, 1, 0);
    mLayoutMain->addWidget(mButtonClose, 1, 1);

    // 设置默认
    mButtonOk->setFocus();
    mButtonOk->setShortcut(QKeySequence::InsertParagraphSeparator);
    mButtonOk->setShortcut(Qt::Key_Enter);
    mButtonOk->setShortcut(Qt::Key_Return);

    setLayout(mLayoutMain);
    setWindowTitle(tr("添加链接"));
    setFixedSize(sizeHint());
}

void DialogAddLink::fApply()
{
    int index = mComboBoxScene->currentIndex();

    if(index > 0)
    {
        TScene *mScene = mSceneList.at(index - 1); // 减去第一个 "无"

        if(mGraphics && mScene)
        {
            mGraphics->fSetLinkScene(mScene->fGetId());
        }
    }
    else
    {
        mGraphics->fSetLinkScene(0);
    }

    close();
}

DialogAddLink::~DialogAddLink()
{
}
