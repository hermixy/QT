#ifndef FILE_H
#define FILE_H

#include <QFile>
//#include <QApplication>

class TFile
{
    //Q_DECLARE_TR_FUNCTIONS(TFile)

public:
    TFile();
    ~TFile();

    static bool fWriteToFile(const QString &FileName, const QString &Data);
    //static bool fLoadTableToFile(TTableView *Table, QString FileName);

    bool fOpen(const QString &FileName, const QIODevice::OpenModeFlag &ModeFlag = QIODevice::ReadWrite);
    bool fWrite(const QString &Data);
    bool fReadLn(QString &Data);
    bool fClose();

private:
    QFile *mFile;
    class QTextStream *mOut;
};

#endif // FILE_H
