#ifndef DIALOG_NEW_SCENE_H
#define DIALOG_NEW_SCENE_H

#include "../Scada/Src/Base/Base.h"

class DialogNewScene : public QDialog
{
    Q_OBJECT

public:
    DialogNewScene(QWidget *parent = 0);
    ~DialogNewScene();

    //bool mInit;
    bool mSave;
    void fSetTitle(const QString &Title);
    bool fSetName(const QString &Name);
    QString fGetName() const;
    bool fSetBackColor(const QColor &Color);
    QColor fGetBackColor() const;

private slots:
    void fChooseBackColor();
    void fApply();

private:
    QLabel *mLabelNameShow;
    QLineEdit *mLineEdit;
    QLabel *mLabelBackColorShow;
    QLabel *mLabelBackColor;
    QPushButton *mButtonColor;
    QPushButton *mButtonOk;
    QPushButton *mButtonClose;

    QColor mBackColor;
};

#endif // DIALOG_NEW_SCENE_H
