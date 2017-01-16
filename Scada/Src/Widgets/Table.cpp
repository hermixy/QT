#include "Table.h"

#include "Button.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

TTable::TTable(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
    TWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    fSetType(Table);

#ifdef RUNNER
    fSetDragAble(false);
    fSetSelectAble(false);
    setAcceptHoverEvents(false);
#endif
    mCurrentIndex = -1;
    mCurrentCol = -1;
    mRowSum = 0;
    mColSum = 0;
    mRowLength = 0;
    mColLengthArray = NULL;
    mData = NULL;
}

TItem* TTable::fCopy()
{
    TTable *Table = new TTable();

    Table->fCopy(this);

    return Table;
}

void TTable::fCopy(TTable *TableFrom)
{
    if(NULL == TableFrom)
    {
        return;
    }

    TWidgets::fCopy(TableFrom);

    // 大小
    this->fSetSize(TableFrom->fGetRowSum(), TableFrom->fGetColSum());

    // 行高
    this->fSetRowLength(TableFrom->fGetRowLength());

    // 列宽
    for(int i = 0; i < TableFrom->fGetColSum(); i++)
    {
        this->fSetColLength(i, TableFrom->fGetColLength(i));
    }

    // 内容
    for(int i = 0; i < TableFrom->fGetRowSum(); i++)
    {
        for(int j = 0; j < TableFrom->fGetColSum(); j++)
        {
            this->fSetCell(i, j, TableFrom->fGetCell(i, j));
        }
    }
}

TTable::~TTable()
{
    fDestroy();
}

void TTable::fDestroy()
{
    if(mColLengthArray)
    {
        free(mColLengthArray);
        mColLengthArray = NULL;
    }

    if(NULL != mData)
    {
        for(int i = 0; i < mRowSum; i++)
        {
            if(NULL == mData[i])
            {
                break;
            }

            for(int j = 0; j < mColSum; j++)
            {
                if(NULL != mData[i][j])
                {
                    free(mData[i][j]);
                    mData[i][j] = NULL;
                }
            }

            free(mData[i]);
            mData[i] = NULL;
        }

        free(mData);
        mData = NULL;
    }

    mRowSum = 0;
    mColSum = 0;
}

void TTable::fUpdatePos()
{
    if(NULL == mColLengthArray)
    {
        return;
    }

    int Height = 0;

    for(int i = 0; i < mColSum; i++)
    {
        Height += mColLengthArray[i];
    }

    fMoveTo(mStartX, mStartY,
            mStartX + Height,
            mStartY + mRowLength * mRowSum);
}

bool TTable::fSetSize(int RowSum, int ColSum)
{
    fDestroy();

    mRowSum = RowSum;
    mColSum = ColSum;

    // 计算行高
    mRowLength = (mStopY - mStartY) / mRowSum;

    // 分配列宽长度
    mColLengthArray = (int *)malloc(sizeof(int) * mColSum);

    if(NULL == mColLengthArray)
    {
        goto SET_ERR;
    }

    // 计算列宽
    for(int i = 0; i < mColSum; i++)
    {
        mColLengthArray[i] = (mStopX - mStartX) / mColSum;
    }

    // 分配内容空间
    mData = (QString ***)malloc(sizeof(QString **) * mRowSum);

    if(NULL == mData)
    {
        goto SET_ERR;
    }

    for(int i = 0; i < mRowSum; i++)
    {
        mData[i] = NULL;
    }

    for(int i = 0; i < mRowSum; i++)
    {
        mData[i] = (QString**)malloc(sizeof(QString*) * mColSum);

        if(NULL == mData[i])
        {
            goto SET_ERR;
        }
    }

    for(int i = 0; i < mRowSum; i++)
    {
        for(int j = 0; j < mColSum; j++)
        {
            mData[i][j] = NULL;
        }
    }

    for(int i = 0; i < mRowSum; i++)
    {
        for(int j = 0; j < mColSum; j++)
        {
            mData[i][j] = new QString();

            if(NULL == mData[i][j])
            {
                goto SET_ERR;
            }
        }
    }

    // 更新
    fUpdatePos();

    return true;

SET_ERR:
    fDestroy();
    return false;
}

int TTable::fGetRowSum()
{
    return mRowSum;
}

int TTable::fGetColSum()
{
    return mColSum;
}

void TTable::fSetColLength(int Index, int Len)
{
    if(mColLengthArray && Index >= 0 && Index < mColSum)
    {
        mColLengthArray[Index] = Len;

        fUpdatePos();
    }
}

int TTable::fGetColLength(int Index)
{
    if(mColLengthArray && Index >= 0 && Index < mColSum)
    {
        return mColLengthArray[Index];
    }

    return 0;
}

void TTable::fSetRowLength(int Len)
{
    mRowLength = Len;

    fUpdatePos();
}

int TTable::fGetRowLength()
{
    return mRowLength;
}

int TTable::fGetCurrentCol()
{
    return mCurrentCol;
}

int TTable::fGetCurrentIndex()
{
    return mCurrentIndex;
}

QString TTable::fGetCurrentData(int Col)
{
    return fGetCell(fGetCurrentIndex(), Col);
}

void TTable::fSetCell(int Row, int Col, QString Text)
{
    if(Row >= 0 && Row < mRowSum && Col >= 0 && Col < mColSum)
    {
        *mData[Row][Col] = Text;
    }
}

QString TTable::fGetCell(int Row, int Col)
{
    //qDebug() << "TTable::fGetCell(int Row, int Col)";

    QString Text = tr("");

    if(Row >= 0 && Row < mRowSum && Col >= 0 && Col < mColSum)
    {
        Text = *mData[Row][Col];
    }

    return Text;
}

//void TTable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
void TTable::fDraw(QPainter *painter)
{
    //fBeforePaint(painter, option, widget);

    // 计算列宽
    for(int i = 0; i < mColSum; i++)
    {
        mColLengthArray[i] = (mStopX - mStartX) / mColSum;
    }
    // 计算行高
    mRowLength = (mStopY - mStartY) / mRowSum;

    for(int i = 0; i < mRowSum; i++)
    {
        double Width = 0;

        if(mCurrentIndex == i)
        {
#ifndef RUNNER
            painter->setBrush(Qt::blue);
#endif
        }
        else
        {
            painter->setBrush(mBackGroundColor);
        }

        for(int j = 0; j < mColSum; j++)
        {
            painter->drawRect(mStartX + Width,
                              mStartY + i * mRowLength,
                              mColLengthArray[j],
                              mRowLength);

            painter->drawText(mStartX + Width,
                              mStartY + i * mRowLength,
                              mColLengthArray[j],
                              mRowLength,
                              Qt::AlignCenter,
                              *mData[i][j]);

            Width += mColLengthArray[j];
        }
    }
}

void TTable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 判断在第几行
    if(mRowLength > 0 && mRowSum > 0)
    {
        mCurrentIndex = (event->pos().y() - mStartY) / mRowLength;
    }

    // 判断在第几列
    if(mColLengthArray)
    {
        int Height = event->pos().x() - mStartX;

        for(int i = 0; i < fGetColSum(); i++)
        {
            if(Height <= mColLengthArray[i])
            {
                mCurrentCol = i;
                break;
            }
            else
            {
                Height -= mColLengthArray[i];
            }
        }
    }


    //qDebug() << "TTable::mousePressEven" << mCurrentIndex << mCurrentCol;

    fUpdate();
    //fSetSize(fGetRowSum(), fGetColSum());
    TItem::mousePressEvent(event);
}



