#ifndef TTEXT_H
#define TTEXT_H

#include "../Graphics/Graphics.h"

/////////////////////////////////////////////////  文字  /////////////////////////////////////////////////////
class TText : public TGraphics
{
    Q_OBJECT
public:
    TText(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::green, const QColor &BackColor = Qt::white,
          const QString &Text = tr("Text"));

    virtual ~TText();

public slots:
    TItem *fCopy();
    void fCopy(TText *TextFrom);

    void fSetFontFamily(const QString &Family);
    QString fGetFontFamily() const;
    void fSetFontSize(int Size);
    int fGetFontSize() const;
    void fSetFontSpacing(int Spacing);
    int fGetFontSpacing() const;
    void fSetFontBlod(bool Blod);
    bool fGetFontBlod() const;
    void fSetFontItalic(bool Italic);
    bool fGetFontItalic() const;
    void fSetFontUnderLine(bool UnderLine);
    bool fGetFontUnderLine() const;
    void fSetFontStrikeOut(bool StrikeOut);
    bool fGetFontStrikeOut() const;

    void fSetText(const QString &Text);
    QString fGetText() const;

public:
    QString mText;
    QString mFontFamily;

    int mFontSize;
    bool mFontBlod;
    bool mFontItalic;
    int mFontSpacing;
    bool mFontUnderLine;
    bool mFontStrikeOut;
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);
};
#endif // TTEXT_H
