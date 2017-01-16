#ifndef BASE_H
#define BASE_H

#include <QDialog>
#include <QApplication>

#if 1
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QCheckBox>
#else
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QCheckBox>
#endif

#include <QProgressBar>

#include <QLayout>
#include <QLineEdit>
#include <QDateTimeEdit>

class TBase
{
    Q_DECLARE_TR_FUNCTIONS(TBase)

private:
    TBase();
    ~TBase();

public:
    static QLabel* fLabel(const QString &Text, const QString &ObjectName);
    static QLabel* fLabel(const QString &Text, const QString &ObjectName, const int Width, const int Height);
    static QPushButton* fButton(const QString &Text, const QString &ObjecName);
    static QPushButton* fButton(const QString &Text, const QString &ObjecName, const int Width, const int Height);

    static void fColorLabel(QLabel *Label, const QColor &Color);
    static void fColorLabel(QLabel *Label, const QColor &Color, int Width, int Height);
    static void fPictureLabel(QLabel *Label, const QString &Dir);
    static QSpinBox* fSpinBox(const QString &ObjectName, int Min, int Max);
    static QSpinBox* fSpinBox(const QString &ObjectName, int Min, int Max, int Width, int Height);

    static QDoubleSpinBox* fDoubleSpinBox(const QString &ObjectName);
    // 调用该接口，必须传入浮点数，假如为常数，也要包含小数点，如5.0
    static QDoubleSpinBox* fDoubleSpinBox(const QString &ObjectName, const double Min, const double Max);
    static QDoubleSpinBox* fDoubleSpinBox(const QString &ObjectName, const double Min, const double Max,
                                          const int Width, const int Height);
    static QComboBox* fComboBox(const QString &ObjectName);
    static QComboBox* fComboBox(const QString &ObjectName, const int Width, const int Height);
    static QComboBox* fLineStyleComboBox();
    static QTextEdit* fTextEdit(const QString &ObjectName);
    static QTextEdit* fTextEdit(const QString &ObjectName, const int Width, const int Height);
    static QLineEdit* fLineEdit(const QString &ObjectName);
    static QLineEdit* fLineEdit(const QString &ObjectName, const int Width, const int Height);
    static QCheckBox* fCheckBox(const QString &Text, const QString &ObjectName);

    static bool fChooseColor(QColor &Color, QWidget *parent = 0);
    static int fGetComboBoxCurrentData(QComboBox *ComboBox);

    // month 距离当前时间的月
    static QDateTimeEdit* fDateTimeEdit(const QString &ObjectName, int Month = 0);

    static QProgressBar* fProgressBar(const QString &ObjectName, int Min, int Max, QWidget *parent = 0);

    static QColor fGetNewColor(const QColor &Source, int Value);
};

#endif // BASE_H
