#ifndef TABLE_SERVICE_H
#define TABLE_SERVICE_H

#include "../File/File.h"
#include "../TableView/TableView.h"

#include <QApplication>

class TTableService
{
    Q_DECLARE_TR_FUNCTIONS(TTableService)

public:
    static bool fOutToCsv(TTableView *Table, QWidget *parent = 0);

private:
    TTableService(){}
    ~TTableService(){}
};

#endif // TABLE_SERVICE_H
