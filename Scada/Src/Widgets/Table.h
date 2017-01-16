#ifndef TTABLE_H
#define TTABLE_H

#include "Widgets.h"

///////////////////////  控件基类 /////////////////////////////////////
class TTable : public TWidgets
{
    Q_OBJECT
public:
    TTable(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);
    virtual ~TTable();

public slots:
    TItem *fCopy();
    void fCopy(TTable *TableFrom);

    bool fSetSize(int RowSum, int ColSum);
    int fGetRowSum();
    int fGetColSum();

    void fSetColLength(int Index, int Len);
    int fGetColLength(int Index);
    void fSetRowLength(int Len);
    int fGetRowLength();

    int fGetCurrentCol();               // 获取当前列
    int fGetCurrentIndex();             // 获取当前行
    QString fGetCurrentData(int Col);   // 获取当前行 某列的数据
    void fSetCell(int Row, int Col, QString Text);
    QString fGetCell(int Row, int Col);

private:
    int mCurrentCol;
    int mCurrentIndex;
    QString ***mData;
    int mRowSum;
    int mColSum;

    int mRowLength;
    int *mColLengthArray;

    void fDestroy();
    void fUpdatePos();
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void fDraw(QPainter *painter);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // TTABLE_H
