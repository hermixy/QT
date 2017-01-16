#ifndef DIALOG_JS_HELPER_H
#define DIALOG_JS_HELPER_H

#include "../Scada/Src/Base/Base.h"

class DialogJsHelper : public QDialog
{
    Q_OBJECT
public:
    DialogJsHelper(QWidget *parent = 0);
    ~DialogJsHelper();

private slots:
    void fComboBoxConditionChanged(int Index);
    void fComboBoxActionChanged(int Index);
    void fButtonColorClick();
    void fAddAction();
    void fAddCondition();
    void fAddTemplate();

private:
    // 种类
    QGroupBox* mGroupBoxType;
    QComboBox* mComboBoxType;
    QLabel* mLabelTypeHint;

    // 条件
    //QLabel* mLabelCondition;
    QGroupBox* mGroupBoxCondition;
    QComboBox* mComboBoxCondition;
    QDoubleSpinBox* mSpinBoxCondition;
    QDoubleSpinBox* mSpinBoxConditionEnd;

    // 动作
    //QLabel* mLabelAction;
    QGroupBox* mGroupBoxAction;
    QComboBox* mComboBoxAction;

    QPushButton* mPushButtonAddCondition;
    QPushButton* mPushButtonAddAction;
    QPushButton* mPushButtonClear;
    QPushButton* mPushButtonOK;
    QPushButton* mPushButtonClose;

    QComboBox* mComboBoxLineStyle;
    QSpinBox* mSpinBoxLineWidth;
    QLabel* mLabelColor;
    QPushButton* mButtonColor;
    QDoubleSpinBox* mSpinBoxStart;
    QDoubleSpinBox* mSpinBoxStop;

    QTextEdit* mTextEdit;

    QLabel* mLabelSample;
    QTextEdit* mTextEditSample;

    QColor mColor;    

    void fInitSampleText();
};

#endif // DIALOG_JS_HELPER_H
