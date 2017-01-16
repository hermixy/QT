#ifndef DIALOGADDGROUP_H
#define DIALOGADDGROUP_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include "../Scada/Src/Base/Base.h"
class DialogAddGroup : public QDialog
{
    Q_OBJECT
public:
    DialogAddGroup(QWidget *parent=0);
    ~DialogAddGroup();
private:
    QLabel *mgroupName;
    QLabel *mGroupId;
    QLineEdit *mEditButtonText;
    QSpinBox * mSpinBoxGropu;

    QPushButton *mButtonOk;
    QPushButton *mButtonClose;
public slots:
    void fApply();
public:
    int groupId;
    QString groupName;


};

#endif // DIALOGADDGROUP_H
