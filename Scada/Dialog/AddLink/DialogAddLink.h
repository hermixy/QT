#ifndef DIALOG_ADDLINK_H
#define DIALOG_ADDLINK_H

#include "../Scada/Src/Graphics/GraphicsPub.h"
#include "../Scada/Src/Widgets/WidgetsPub.h"
#include "../Scada/Src/Scene/Scene.h"

#include "../Scada/Src/Base/Base.h"

class DialogAddLink : public QDialog
{
    Q_OBJECT

public:
    DialogAddLink(TWidgets *Graphics, QList<TScene *> SceneList, QWidget *parent = 0);
    ~DialogAddLink();
    bool mInit;

private slots:
    void fApply();

private:
    QComboBox *mComboBoxScene;
    QPushButton *mButtonOk;
    QPushButton *mButtonClose;

    TButton *mGraphics;
    QList<TScene *> mSceneList;
};

#endif // DIALOG_ADDLINK_H
