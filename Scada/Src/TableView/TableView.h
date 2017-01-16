#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#if 1
#include <QTableView>
#else
#include <Qtwidgets/QTableView>
#endif

#include <QStandardItemModel>

class TTableView : public QTableView
{
    Q_OBJECT

public:
    TTableView(const QString ObjectName, QWidget *parent = 0);
    ~TTableView();

    void fSetSize(int RowSize, int ColSize);        // 设定表格有几行几列
    int fGetColSum();                               // 列数
    int fGetRowSum();                               // 行数

    void fSetSelectRows(bool Enable);   // 选中整行
    void fSetEditAble(bool Enable);     // 可编辑
    void fSetHEdit(bool Enable);        // 列宽是否可以通过鼠标拖动改变
    void fSetVEdit(bool Enable);        // 行高是否可以通过鼠标拖动改变

    void fSetHeadName(int Index, const QString &Name); // 设定各列名称
    void fSetHeadLength(int Index, int Len);           // 设定各列名称
    void fSetRowLength(int Len);                       // 设定行高
    void fUpdateFixWidth();
    int fGetColLength(int Index);

    QString fGetCurrentData(int ColNum);         // 获得当前选中行的某列值
    int fGetCurrentIndex() const;                      // 当前选中行
    void fSetCurrentIndex(int Row);

    void fClear();                                  // 清空表格

    void fSetCell(int RowNum, int ColNum, const QString &Value); // 设定单元格内容
    QString fGetCell(int RowNum, int ColNum);

    int fAddRow();
    int fAddRow(QList<QString> &RowData);
    void fUpdateRow(int Row, QList<QString> &RowData);
    void fDelRow(int Index);                        // 删除一行
    int fAddCol(const QString &Name);               // 增加一列
    void fDelCol(int Index);                        // 删除一列

Q_SIGNALS:
    void mClick(int Row, int Col);

private slots:
    void fClick(QModelIndex Index);

private:
    QStandardItemModel *mModel;
};

#endif // TABLE_VIEW_H
