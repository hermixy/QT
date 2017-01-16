#include "scenselistdialog.h"
#include "Maker.h"
ScenseListDialog::ScenseListDialog(QList<TScene*> mSceneList, QWidget *parent):QDialog(parent)
{
    mLayoutMain=new QGridLayout(this);
    int i=0, j=0;
    foreach(TScene *Scene, mSceneList)
    {
        QPushButton *button=TBase::fButton(Scene->fGetName(), tr("mButtonLogin"));
        connect(button, SIGNAL(clicked()), this, SLOT(clickScense()));
        mLayoutMain->addWidget(button, i, j);
        if(j++==10)
        {
            i++;
            j=0;
        }
    }
}

ScenseListDialog::~ScenseListDialog()
{

}

void ScenseListDialog::clickScense()
{
    QString SceneName=qobject_cast<QPushButton *>(sender())->text();
    foreach(TScene *scene, Maker::maker->mSceneList)
    {
        if(scene->fGetName()==SceneName)
        {
            Maker::maker->fSetCurrentScene(scene);
            close();
            return;
        }
    }
}
