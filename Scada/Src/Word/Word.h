#ifndef TWORD_H
#define TWORD_H

#include <QAxWidget>
#include <QAxObject>

class TWord
{
public:
    TWord(const QString &Dir);
    ~TWord();

    bool fInsertText(const QString &Bookmark, const QString &Text);
    bool fInsertPixmap(const QString &Bookmark, const QPixmap &Pixmap);
    bool fSaveAs(const QString &Dir);
    bool fClose();

private:
    bool mInit;
    QAxWidget *mWordFile;
    QAxObject *mDocument;
};

#endif

