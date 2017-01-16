#include "File.h"

#include <QDebug>

TFile::TFile()
{
}

TFile::~TFile()
{
    fClose();
}

bool TFile::fWriteToFile(const QString &FileName, const QString &Data)
{
    QFile File(FileName);

    if(!File.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QTextStream Out(&File);
    Out.setCodec("UTF-8");

    Out << Data;

    File.close();

    return true;
}

bool TFile::fOpen(const QString &FileName, const QIODevice::OpenModeFlag &ModeFlag)
{
    mFile = new QFile(FileName);

    if(!mFile->open(ModeFlag))
    {
        return false;
    }

    mOut = new QTextStream(mFile);
    //mOut->setCodec("UTF-8");

    return true;
}

bool TFile::fWrite(const QString &Data)
{
    *mOut << Data;

    return true;
}

bool TFile::fReadLn(QString &Data)
{
    //qDebug() << mFile << mFile->isOpen() << !mOut->atEnd();
    if(mFile && mFile->isOpen() && !mOut->atEnd())
    {
        Data = mOut->readLine();
        return true;
    }

    return false;
}

bool TFile::fClose()
{
    if(mFile && mFile->isOpen())
    {
        mFile->close();
    }

    if(mFile)
    {
        delete mFile;
        mFile = 0;
    }

    if(mOut)
    {
        delete mOut;
        mOut = 0;
    }

    return true;
}
