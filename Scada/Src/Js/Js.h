#ifndef JS_H
#define JS_H

#include <QApplication>

#include <QString>
#include <QtScript/QScriptEngine>

class TJs
{
    Q_DECLARE_TR_FUNCTIONS(TJs)

public:
    TJs();
    TJs(const TJs *CopyJs);
    TJs(QObject *Parent, const QString &Dir, const QString &Name);
    ~TJs();

    void fCopy(const TJs *CopyJs);

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

#ifdef DEF_JS_SAVE_DATA
    QScriptEngine mJsIinterpreter;
    QScriptValue mScriptValue;
    QScriptValue mJsRunValue;
    QScriptValueList mJsRunArgs;
#endif

    QString mDir;
    QString mName;

#ifdef DEF_JS_SAVE_FILE
    QString mJsFileString;
#endif
};

#endif // JS_H
