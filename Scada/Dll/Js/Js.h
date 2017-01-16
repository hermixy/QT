#ifndef JS_H
#define JS_H

#include "js_global.h"

#include <QtWidgets/QApplication>
#include <QString>
#include <QtScript/QScriptEngine>

class JSSHARED_EXPORT TJs
{
    Q_DECLARE_TR_FUNCTIONS(TJs)

public:
    TJs();
    TJs(const TJs *CopyJs);
    TJs(QObject *Parent, const QString &Dir, const QString &Name);
    ~TJs();

    void fSetParent(QObject *Parent);
    QObject *fGetParent() const;
    void fSetDir(const QString &Dir);
    QString fGetDir() const;
    void fSetName(const QString &Name);
    QString fGetName() const;

    bool fInit();
    bool fGetInit();
    bool fCall(const QString &FunName);
    bool fRun(QObject *Object, const QString &FunName);

protected:
    bool mInit;
    QObject *mParent;

    QString mDir;
    QString mName;

    QString mJsFileString;
};

#endif // JS_H
