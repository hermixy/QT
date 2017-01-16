#include "Js.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

#include <QDebug>

TJs::TJs()
{
    mInit = false;

    mDir = tr("");
    mName = tr("");
    mParent = 0;
}

TJs::TJs(const TJs *CopyJs)
{
    mInit = false;

    mParent = CopyJs->mParent;
    mDir = CopyJs->mDir;
    mName = CopyJs->mName;

    if(CopyJs->mInit)
    {
        fInit();
    }
}

TJs::TJs(QObject *Parent, const QString &Dir, const QString &Name)
{
    mInit = false;

    mParent = Parent;
    mDir = Dir;
    mName = Name;
}

TJs::~TJs()
{}

void TJs::fSetParent(QObject *Parent)
{
    mParent = Parent;
}

QObject* TJs::fGetParent() const
{
    return mParent;
}

void TJs::fSetDir(const QString &Dir)
{
    mDir = Dir;
}

QString TJs::fGetDir() const
{
    return mDir;
}

void TJs::fSetName(const QString &Name)
{
    mName = Name;
}

QString TJs::fGetName() const
{
    return mName;
}

bool TJs::fGetInit()
{
    return mInit;
}

bool TJs::fInit()
{
    if(mName.length() <= 0)
    {
        return false;
    }

    qDebug() << "js init " << mDir + mName;

    QFile jsFile(mDir + mName);

    if (!jsFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot Open js File";
        return false;
    }

    QTextStream jsStream(&jsFile);
    jsStream.setCodec("UTF-8");
    mJsFileString = jsStream.readAll();
    jsFile.close();

    mInit = true;

    return true;
}

bool TJs::fRun(QObject *Object, const QString &FunName)
{
    if(!fGetInit())
    {
        fSetParent(Object);
        fInit();
    }

    return fCall(FunName);
}

bool TJs::fCall(const QString &FunName)
{
    if(!mInit || NULL == mParent)
    {
        return false;
    }

    qDebug() << "js call " << mName + " --> " + FunName;

    QScriptEngine JsIinterpreter;

    //qDebug() << mJsFileString;

    JsIinterpreter.evaluate(mJsFileString); // 执行

    if(JsIinterpreter.hasUncaughtException())
    {
        qDebug() << "Cannot Run js File";
        return false;
    }

    QScriptValue ScriptValue = JsIinterpreter.newQObject(mParent);
    QScriptValueList Args;
    Args.append(ScriptValue);

    QScriptValue JsRunValue = JsIinterpreter.globalObject().property(FunName);
    JsRunValue.call(JsRunValue, Args);

    return true;
}
