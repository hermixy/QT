#ifndef TPROCESSBAR_H
#define TPROCESSBAR_H

#include "../Graphics/Graphics.h"

class TProcessBar : public TGraphics
{
    Q_OBJECT
public:
    TProcessBar(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);

    virtual ~TProcessBar();

    enum
    {
        PROCESSBAR_RIGHT,
        PROCESSBAR_TOP,
        PROCESSBAR_LEFT,
        PROCESSBAR_BOTTOM
    };

public slots:
    TItem *fCopy();
    void fCopy(TProcessBar *ProcessBarFrom);

    void fSetMode(int Mode);
    int fGetMode();

    void fSetScaleSum(int Sum);
    int fGetScaleSum();
    void fSetStartValue(double Value);
    double fGetStartValue();
    void fSetStepValue(double Value);
    double fGetStepValue();
    void fSetValueColor(const QColor &Color);
    void fSetValueColor(int R, int G, int B, int A = 255);
    QColor fGetValueColor();

    /*void fSetTextName(const QString &Name);
    QString fGetTextName();
    void fSetTextUnit(const QString &Unit);
    QString fGetTextUnit();*/

private:
    int mMode;
    int mScaleSum;          // 刻度总数
    double mStartValue;     // 起始值
    double mStepValue;      // 阶梯值
    QColor mValueColor;     // 内部填充颜色

    //QString mTextName;      // 名称
    //QString mTextUnit;      // 单位
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};

#endif // TPROCESSBAR_H
