#ifndef SCALE_H
#define SCALE_H

#include <QWidget>
#include <QProgressBar>

class TScale: public QWidget
{
    Q_OBJECT

public:
    TScale(int Width, int Height, QWidget *parent = 0);
    TScale(int Width, int Height, int Min, int Max, int Step, QWidget *parent = 0);
    bool fSetValue(double Value);
    void fSetRange(double Min, double Max, double Step);
    void fSetTextHead(const QString &Head);
    void fSetTextEnd(const QString &End);
    void fSetText(const QString &Head, const QString &End);
    void fSetColor(const QColor &Color);

protected:
    void paintEvent(QPaintEvent *event);//重绘用的事件处理

private:
    QProgressBar *mBar;

    int mWidth;
    int mHeight;

    double mMin;
    double mMax;
    double mStep;

    int mValue;
    QString mHead;
    QString mEnd;

    void fInit();
};

#endif // SCALE_H
