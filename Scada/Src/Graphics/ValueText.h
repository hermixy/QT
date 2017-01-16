#ifndef TVALUE_TEXT_H
#define TVALUE_TEXT_H

#include "../Graphics/Text.h"

class TValueText : public TText
{
    Q_OBJECT
public:
    TValueText(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::green, const QColor &BackColor = Qt::white);

    virtual ~TValueText();

    enum
    {
        MODE_INT,
        MODE_FLOAT
    };

public slots:

    TItem *fCopy();
    void fCopy(TValueText *ValueTextFrom);

    virtual void fSetValue(const double Value);

    void fSetMode(int _Mode);
    int fGetMode();
    void fSetIntLen(int _IntLen);
    int fGetIntLen();
    void fSetPointLen(int _PointLen);
    int fGetPointLen();


private:
    int mMode;
    int mIntLen;
    int mPointLen;

    void fDraw(QPainter *painter);
};
#endif // TVALUE_TEXT_H
