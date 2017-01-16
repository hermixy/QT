#ifndef SCENSELISTDIALOG_H
#define SCENSELISTDIALOG_H
#include <QGridLayout>
#include "../../Src/Base/Base.h"
#include "../Scada/Src/Scene/Scene.h"
class ScenseListDialog : public QDialog
{
    Q_OBJECT
public:
    ScenseListDialog(QList<TScene*> mSceneList, QWidget *parent);
    ~ScenseListDialog();

private:
    QGridLayout* mLayoutMain;
public slots:
    void clickScense();
};

#endif // SCENSELISTDIALOG_H
