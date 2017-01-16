#include "TableService.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

bool TTableService::fOutToCsv(TTableView *Table, QWidget *parent)
{
    QString mFileDir = QFileDialog::getSaveFileName(parent, tr("另存为"));

    if(mFileDir.length() <= 0)
    {
        return false;
    }

    if(!mFileDir.endsWith(tr(".csv")))
    {
        mFileDir += tr(".csv");
    }

    //mTable->fLoadToFile(mFileDir);
    TFile File;

    if(!File.fOpen(mFileDir, QIODevice::WriteOnly))
    {
        QMessageBox::warning(parent, tr("提示"), tr("导出失败"));
        return false;
    }

    int RowSum = Table->fGetRowSum();
    int ColSum = Table->fGetColSum();
    QString StrData;

    qDebug() << "TTable::fLoadToFile" << RowSum << ColSum;

    for(int i = 0; i < RowSum; ++i)
    {
        if(0 != i)
        {
            File.fWrite(tr("\n"));
        }

        for(int j = 0; j < ColSum; ++j)
        {
            if(0 != j)
            {
                File.fWrite(tr(","));
            }

            StrData = Table->fGetCell(i, j);
            File.fWrite(StrData);
        }
    }

    qDebug() << "TTable::fLoadToFile close";

    File.fClose();

    return true;
}
