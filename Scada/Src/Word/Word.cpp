#include "Word.h"

#include <QApplication>
#include <QClipboard>

#include <QDebug>

TWord::TWord(const QString &Dir)
{
    mInit = false;
    mWordFile = NULL;
    mDocument = NULL;

    // 新建一个word应用程序,并设置为不可见
    mWordFile = new QAxWidget("Word.Application", 0, Qt::MSWindowsOwnDC);

    if(NULL == mWordFile)
    {
        return;
    }

    QVariant Value(false);

    mWordFile->setProperty("Visible", Value);

    // 获取所有的工作文档
    QAxObject *Documents = mWordFile->querySubObject("Documents");

    if(NULL == Documents)
    {
        return;
    }

    // 以文件template.dot为模版新建一个文档
    Documents->dynamicCall("Add(QString)", Dir);

    // 获取当前激活的文档
    mDocument = mWordFile->querySubObject("ActiveDocument");

    if(NULL == mDocument)
    {
        return;
    }

    mInit = true;
}

TWord::~TWord()
{
    fClose();
}

bool TWord::fInsertPixmap(const QString &Bookmark, const QPixmap &Pixmap)
{
    if(!mInit)
    {
        return false;
    }

    QByteArray MarkValue;
    MarkValue.append(("Bookmarks("));
    MarkValue.append(Bookmark.toLocal8Bit().data());
    MarkValue.append(")");

    // 获取文档中名字为label2的标签
    QAxObject *BookmarkPixmap = mDocument->querySubObject(MarkValue);

    bool Ret = false;

    // 选中标签，将图片插入到标签位置
    if(BookmarkPixmap && !BookmarkPixmap->isNull())
    {
        BookmarkPixmap->dynamicCall("Select(void)");

#if false
        // 保存文件方式
        QString DirPath = QApplication::applicationDirPath() + "//temp.png";
        Pixmap.save(DirPath);
        BookmarkPixmap->querySubObject("Range")->querySubObject("InlineShapes")->dynamicCall("AddPicture(const QString&)", DirPath);
        //BookmarkPixmap->querySubObject("Range")->setProperty("InlineShapes", DirPath);

        Ret = true;
#else
        // 利用系统剪切板方式
        QClipboard *Clipboard = QApplication::clipboard();

        if(Clipboard)
        {
            // 保存当前剪切板的图片
            QPixmap SystemPixmap(Clipboard->pixmap());
            Clipboard->setPixmap(Pixmap);

            BookmarkPixmap->querySubObject("Range")->dynamicCall("Paste()");

            // 恢复剪切板的图片
            Clipboard->setPixmap(SystemPixmap);

            Ret = true;
        }
#endif
    }

    return Ret;
}

bool TWord::fInsertText(const QString &Bookmark, const QString &Text)
{
    if(!mInit)
    {
        return false;
    }

    QByteArray MarkValue;
    MarkValue.append(("Bookmarks("));
    MarkValue.append(Bookmark.toLocal8Bit().data());
    MarkValue.append(")");

    // 获取文档中名字为label1的标签
    QAxObject* BookmarkText = mDocument->querySubObject(MarkValue);

    if(NULL == BookmarkText)
    {
        return false;
    }

    bool Ret = false;

    // 选中标签，将字符InsertText插入到标签位置
    if(BookmarkText && !BookmarkText->isNull())
    {
        BookmarkText->dynamicCall("Select(void)");

        QVariant Value(Text);
        BookmarkText->querySubObject("Range")->setProperty("Text", Value);

        Ret = true;
    }

    delete BookmarkText;

    return Ret;
}

bool TWord::fSaveAs(const QString &Dir)
{
    if(!mInit)
    {
        return false;
    }

    mDocument->dynamicCall("SaveAs(const QString&)", Dir);

    return true;
}

bool TWord::fClose()
{
    mInit = false;

    mDocument->dynamicCall("Close(boolean)", false);
    mWordFile->dynamicCall("Quit()");

    return true;
}
