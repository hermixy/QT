#ifndef DIALOG_PLAY_H
#define DIALOG_PLAY_H

#include "../../Src/Widgets/Widgets.h"
#include "../../Src/Graphics/Graphics.h"
#include "../../Src/Service/Service.h"
#include "../../Src/Base/Base.h"

class DialogPlay : public QDialog
{
    Q_OBJECT

public:
    DialogPlay(TItem *Item, const QList<TService *> &ServiceList, QWidget *parent = 0);
    ~DialogPlay();

    bool mInit;

private slots:
    void fOkClicked();

private:
    TItem *mItem;

    // 扩展
    QGroupBox *mGroupBoxConfig;
    QLabel *mLabelNameIdShow;
    QLineEdit *mLineEditNameId;
    QLabel *mLabelServiceShow;
    QComboBox *mComboBoxService;
    QLabel *mLabelHintShow;
    QTextEdit *mTextEditHint;
    QLabel *mLabelUserDataName1Show;
    QLineEdit *mLineEditUserDataName1;
    QDoubleSpinBox *mSpinBoxUserData1;
    QLabel *mLabelUserDataName2Show;
    QLineEdit *mLineEditUserDataName2;
    QDoubleSpinBox *mSpinBoxUserData2;

    // 控件
    QGroupBox *mGroupBoxWidgets;
    QLabel *mLabelWidgetsJsShow;
    QComboBox *mComboBoxWidgetsJs;

    // 图形
    QGroupBox *mGroupBoxGraphics;
    QLabel *mLabelValueShow;
    QDoubleSpinBox *mSpinBoxValue;
    QLabel *mLabelCoeShow;
    QDoubleSpinBox *mSpinBoxCoe;

    // 自身脚本
    QLabel *mLabelNodeIdShow;
    QSpinBox *mSpinBoxNodeId;
    QLabel *mLabelNodeIdText;
    QLabel *mLabelPlayJsShow;
    QComboBox *mComboBoxPlayJs;

    // 自动
    QLabel *mLabelAutoShow;
    QLabel *mLabelAutoTimeShow;
    QSpinBox *mSpinBoxAutoTime;
    QComboBox *mComboBoxAutoJs;

    // 窗口
    QPushButton *mButtonOk;
    QPushButton *mButtonClose;

    void fInit();
    bool fCreateItemEx(const QList<TService *> &ServiceList);
    bool fCreateWidgets();
    bool fCreateGraphics();

    bool fSaveItemEx();
    bool fSaveWidgets();
    bool fSaveGraphics();
};

#endif // DIALOG_PLAY_H
