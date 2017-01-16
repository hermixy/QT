#include "TableView.h"
#include <QHeaderView>
#include <QDebug>

TTableView::TTableView(const QString ObjectName, QWidget *parent) : QTableView(parent)
{
    setObjectName(ObjectName);

    mModel = new QStandardItemModel;

    setModel(mModel);

    setShowGrid(true);// 显示网格

    fSetSelectRows(true);
    fSetEditAble(false);

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(fClick(QModelIndex)));
}

TTableView::~TTableView()
{}

void TTableView::fClick(QModelIndex Index)
{
    emit mClick(Index.row(), Index.column());
}

void TTableView::fSetSize(int RowSize, int ColSize)
{
    int AddColNum = ColSize - fGetColSum();

    if(AddColNum > 0)
    {
        for(int i = 0; i < AddColNum; i++)
        {
            fAddCol(QString::number(fGetColSum() + 1));
        }
    }
    else if(AddColNum < 0)
    {
        for(int i = 0; i < -AddColNum; i++)
        {
            fDelCol(fGetColSum() - 1);
        }
    }

    int AddRowNum = RowSize - fGetRowSum();

    if(AddRowNum > 0)
    {
        for(int i = 0; i < AddRowNum; i++)
        {
            fAddRow();
        }
    }
    else if(AddRowNum < 0)
    {
        for(int i = 0; i < -AddRowNum; i++)
        {
            fDelRow(fGetRowSum() - 1);
        }
    }
}

int TTableView::fGetColSum()
{
    return mModel->columnCount();
}

int TTableView::fGetRowSum()
{
    return mModel->rowCount();
}

void TTableView::fSetSelectRows(bool Enable)
{    
    if(Enable)
    {
        setSelectionBehavior(QTableView::SelectRows);// 选中整行
    }
    else
    {
        setSelectionBehavior(QTableView::SelectItems);
    }
}

void TTableView::fSetEditAble(bool Enable)
{
    if(Enable)
    {
        setEditTriggers(QTableView::AllEditTriggers);
    }
    else
    {
        setEditTriggers(QTableView::NoEditTriggers);// 不可编辑
    }
}

void TTableView::fSetHEdit(bool Enable)
{
#ifndef DEF_RUN_IN_EMBEDDED
    if(Enable)
    {
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else
    {
        horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }
#else
    if(Enable)
    {
        horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    }
    else
    {
        horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    }
#endif
}

void TTableView::fSetVEdit(bool Enable)
{
#ifndef DEF_RUN_IN_EMBEDDED
    if(Enable)
    {
        verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else
    {
        verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    }
#else
    if(Enable)
    {
        verticalHeader()->setResizeMode(QHeaderView::Stretch);
    }
    else
    {
        verticalHeader()->setResizeMode(QHeaderView::Fixed);
    }
#endif
}

void TTableView::fSetHeadName(int Index, const QString &Name)
{
    if(Index >= 0 && Index < fGetColSum())
    {
        mModel->setHeaderData(Index, Qt::Horizontal, Name);
    }
}

void TTableView::fSetHeadLength(int Index, int Len)
{
    if(Index >= 0 && Index < fGetColSum())
    {
        setColumnWidth(Index, Len);
    }
}

void TTableView::fSetRowLength(int Len)
{
    if(Len > 0)
    {
        for(int i = 0; i < fGetRowSum(); i++)
        {
            setRowHeight(i, Len);
        }
    }
}

void TTableView::fUpdateFixWidth()
{
    int Width = 0;

    for(int i = 0; i < fGetColSum(); i++)
    {
        Width += fGetColLength(i);
    }

    setFixedWidth(Width + 40);
}

int TTableView::fGetColLength(int Index)
{
    if(Index >= 0 && Index < fGetColSum())
    {
        return columnWidth(Index);
    }

    return 0;
}

QString TTableView::fGetCurrentData(int ColNum)
{
    return fGetCell(fGetCurrentIndex(), ColNum);
}

int TTableView::fGetCurrentIndex() const
{
    return currentIndex().row();
}

void TTableView::fSetCurrentIndex(int Row)
{
    selectRow(Row);

    emit mClick(Row, 0);
}

void TTableView::fClear()
{
    while(fGetRowSum() > 0)
    {
        fDelRow(0);
    }
}

QString TTableView::fGetCell(int RowNum, int ColNum)
{
    if(RowNum >= 0 && RowNum < fGetRowSum()
            && ColNum >= 0 && ColNum < fGetColSum())
    {
        QStandardItem *Item = mModel->item(RowNum, ColNum);

        if(Item)
        {
            return Item->text();
        }

    }

    return tr("");
}

void TTableView::fSetCell(int RowNum, int ColNum, const QString &Value)
{
    if(RowNum >= 0 && RowNum < fGetRowSum()
            && ColNum >= 0 && ColNum < fGetColSum())
    {
        QStandardItem *Item = mModel->item(RowNum, ColNum);

        if(Item)
        {
            Item->setText(Value);
        }
    }
}

int TTableView::fAddRow()
{
    //qDebug() << "TTableView::fAddRow()0" << fGetRowSum();
    int Col = fGetColSum();
    int Index = fGetRowSum();

    mModel->setRowCount(Index + 1);

    for(int i = 0; i < Col; i++)
    {
        QStandardItem *NewItem = new QStandardItem();
        NewItem->setTextAlignment(Qt::AlignLeft);

        if(0 == Index % 2)
        {
            NewItem->setBackground(QBrush(QColor(255, 255, 180)));
        }

        mModel->setItem(Index, i, NewItem);
    }

    //qDebug() << "TTableView::fAddRow()1" << fGetRowSum();

    return Index;
}

int TTableView::fAddRow(QList<QString> &RowData)
{
    int Index = fAddRow();

    int Count = RowData.length() > fGetColSum() ? fGetColSum() : RowData.length();

    for(int i = 0; i < Count; i++)
    {
        fSetCell(Index, i, RowData.at(i));
    }

    return Index;
}

void TTableView::fUpdateRow(int Row, QList<QString> &RowData)
{
    if(Row < 0 || Row >= fGetRowSum())
    {
        return;
    }

    int Count = RowData.length() > fGetColSum() ? fGetColSum() : RowData.length();

    for(int i = 0; i < Count; i++)
    {
        fSetCell(Row, i, RowData.at(i));
    }
}

void TTableView::fDelRow(int Index)
{
    //qDebug() << "TTableView::fDelRow()0" << fGetRowSum();
    if(Index >= 0 && Index < fGetRowSum())
    {
        int Count = fGetColSum();

        for(int i = 0; i < Count; i++)
        {
            QStandardItem *DelItem = mModel->item(Index, i);

            if(DelItem)
            {
                delete DelItem;
                DelItem = NULL;
            }
        }

        mModel->removeRow(Index);
    }

    //qDebug() << "TTableView::fDelRow()1" << fGetRowSum();
}

int TTableView::fAddCol(const QString &Name)
{
    //qDebug() << "TTableView::fAddCol()0" << fGetColSum();

    int Index = fGetColSum();

    mModel->setColumnCount(Index + 1);
    mModel->setHeaderData(Index, Qt::Horizontal, Name);

    int Count = fGetRowSum();

    for(int i = 0; i < Count; i++)
    {
        QStandardItem *NewItem = new QStandardItem();
        NewItem->setTextAlignment(Qt::AlignLeft);

        if(0 == i % 2)
        {
            NewItem->setBackground(QBrush(QColor(255, 255, 180)));
        }

        mModel->setItem(i, Index, NewItem);
    }

    //qDebug() << "TTableView::fAddCol()1" << fGetColSum();

    return Index;
}

void TTableView::fDelCol(int Index)
{
    //qDebug() << "TTableView::fDelCol()0" << fGetColSum();

    if(Index >= 0 && Index < fGetColSum())
    {
        int Count = fGetRowSum();

        for(int i = 0; i < Count; i++)
        {
            QStandardItem *Item = mModel->item(i, Index);

            if(Item)
            {
                delete Item;
                Item = NULL;
            }
        }

        mModel->removeColumn(Index);
    }

    //qDebug() << "TTableView::fDelCol()1" << fGetColSum();
}
