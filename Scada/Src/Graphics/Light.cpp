#include "Light.h"

#include "../Base/Base.h"
#include "../Base/LibBase.h"

TLight::TLight(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
        TGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(TItem::Light);
}

TLight::~TLight()
{}

TItem *TLight::fCopy()
{
    TLight *Light = new TLight();

    Light->fCopy(this);

    return Light;
}

void TLight::fCopy(TLight *LightFrom)
{
    if(NULL == LightFrom)
    {
        return;
    }

    TGraphics::fCopy(LightFrom);
}

void TLight::fDraw(QPainter *painter)
{
    double Width = mStopX - mStartX;        // 绘图区域的宽度
    double Height = mStopY - mStartY;       // 绘图区域的高度
    double CenterX = mStartX + Width / 2;   // 绘图区域的中心X
    double CenterY = mStartY + Height / 2;  // 绘图区域的中心Y

    painter->translate(CenterX, CenterY);   // 把区域中心作为零点
    painter->setRenderHint(QPainter::Antialiasing); // 去锯齿
    painter->setPen(Qt::transparent);

    int Radius = MIN(Width / 2, Height / 2);// 绘制圆形的半径

    // 边框的外圆
    QLinearGradient lg1(0, -Radius, 0, Radius); // 线性渐变
    lg1.setColorAt(0, mLineColor);
    lg1.setColorAt(1, TBase::fGetNewColor(mLineColor, -90));
    painter->setBrush(lg1);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 边框的内圆
    Radius *= 0.87;
    QLinearGradient lg2(0, -Radius, 0, Radius);
    lg2.setColorAt(0, TBase::fGetNewColor(mLineColor, -100));
    lg2.setColorAt(1, mLineColor);
    painter->setBrush(lg2);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 灯的圆
    Radius *= 0.96;
    QRadialGradient rg(0, 0, Radius); // 辐射渐变
    rg.setColorAt(0, mBackGroundColor);
    rg.setColorAt(0.6, TBase::fGetNewColor(mBackGroundColor, -30));
    rg.setColorAt(1, TBase::fGetNewColor(mBackGroundColor, -70));
    painter->setBrush(rg);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 高光
    Radius *= 0.97;
    QPainterPath path;
    path.addEllipse(-Radius, -Radius * 1.02, Radius << 1, Radius << 1);
    QPainterPath bigEllipse;

    // 减去高光下部的大圆
    Radius *= 2;
    bigEllipse.addEllipse(-Radius, -Radius * 0.3, Radius << 1, Radius << 1);
    path -= bigEllipse;

    QLinearGradient lg3(0, -Radius / 2, 0, 0);
    lg3.setColorAt(0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1, QColor(255, 255, 255, 30));
    painter->setBrush(lg3);
    painter->drawPath(path);
}

/*void TLight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    fBeforePaint(painter, option, widget);

    double Width = mStopX - mStartX;        // 绘图区域的宽度
    double Height = mStopY - mStartY;       // 绘图区域的高度
    double CenterX = mStartX + Width / 2;   // 绘图区域的中心X
    double CenterY = mStartY + Height / 2;  // 绘图区域的中心Y

    painter->translate(CenterX, CenterY);   // 把区域中心作为零点
    painter->setRenderHint(QPainter::Antialiasing); // 去锯齿
    painter->setPen(Qt::transparent);

    int Radius = MIN(Width / 2, Height / 2);// 绘制圆形的半径

    // 边框的外圆
    QLinearGradient lg1(0, -Radius, 0, Radius); // 线性渐变
    lg1.setColorAt(0, mLineColor);
    lg1.setColorAt(1, TBase::fGetNewColor(mLineColor, -90));
    painter->setBrush(lg1);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 边框的内圆
    Radius *= 0.87;
    QLinearGradient lg2(0, -Radius, 0, Radius);
    lg2.setColorAt(0, TBase::fGetNewColor(mLineColor, -100));
    lg2.setColorAt(1, mLineColor);
    painter->setBrush(lg2);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 灯的圆
    Radius *= 0.96;
    QRadialGradient rg(0, 0, Radius); // 辐射渐变
    rg.setColorAt(0, mBackGroundColor);
    rg.setColorAt(0.6, TBase::fGetNewColor(mBackGroundColor, -30));
    rg.setColorAt(1, TBase::fGetNewColor(mBackGroundColor, -70));
    painter->setBrush(rg);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 高光
    Radius *= 0.97;
    QPainterPath path;
    path.addEllipse(-Radius, -Radius * 1.02, Radius << 1, Radius << 1);
    QPainterPath bigEllipse;

    // 减去高光下部的大圆
    Radius *= 2;
    bigEllipse.addEllipse(-Radius, -Radius * 0.3, Radius << 1, Radius << 1);
    path -= bigEllipse;

    QLinearGradient lg3(0, -Radius / 2, 0, 0);
    lg3.setColorAt(0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1, QColor(255, 255, 255, 30));
    painter->setBrush(lg3);
    painter->drawPath(path);

}*/
