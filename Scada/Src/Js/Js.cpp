#include "Js.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

#include <QDateTime>
#include <QThread>
#include <QDebug>

TJs::TJs()
{
    mInit = false;

    //mDir = tr("");
    //mName = tr("");
    mParent = 0;
}

TJs::TJs(const TJs *CopyJs)
{
    fCopy(CopyJs);
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

void TJs::fCopy(const TJs *CopyJs)
{
    mInit = false;

    fSetParent(CopyJs->fGetParent());
    fSetDir(CopyJs->fGetDir());
    fSetName(CopyJs->fGetName());

    if(CopyJs->mInit)
    {
        fInit();
    }
}

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

#ifdef DEF_JS_SAVE_DATA
    QString JsFileString = jsStream.readAll();
#endif

#ifdef DEF_JS_SAVE_FILE
    mJsFileString = jsStream.readAll();
#endif

    jsFile.close();

#ifdef DEF_JS_SAVE_DATA
    mJsIinterpreter.evaluate(JsFileString); // 执行

    if(mJsIinterpreter.hasUncaughtException())
    {
        qDebug() << "Cannot Run js File";
        return false;
    }

    mScriptValue = mJsIinterpreter.newQObject(mParent);
    mJsRunArgs.append(mScriptValue);

    //qDebug() << "TJs::fCall 2" << sizeof(Args) << QDateTime::currentMSecsSinceEpoch();

    mJsRunValue = mJsIinterpreter.globalObject().property(tr("fPlay"));

    //mJsRunValue.call(mJsRunValue, mJsRunArgs);
#endif

    mInit = true;

    return true;
}

bool TJs::fRun(QObject *Object, const QString &FunName)
{
    //qDebug() << "TJs::fRun";

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

#ifndef DEF_RUN_AS_RUNER
    Q_UNUSED(FunName);
#endif

    //qDebug() << "js call " << mName + " --> " + FunName + tr(" at ") << QThread::currentThread();

#ifdef DEF_JS_SAVE_FILE
    QScriptEngine JsIinterpreter;

    //qDebug() << "TJs::fCall 1" << sizeof(JsIinterpreter) << QDateTime::currentMSecsSinceEpoch();

    JsIinterpreter.evaluate(mJsFileString); // 执行

    if(JsIinterpreter.hasUncaughtException())
    {
        qDebug() << "Cannot Run js File";
        return false;
    }

    QScriptValue ScriptValue = JsIinterpreter.newQObject(mParent);
    QScriptValueList Args;
    Args.append(ScriptValue);

    //qDebug() << "TJs::fCall 2" << sizeof(Args) << QDateTime::currentMSecsSinceEpoch();

    QScriptValue JsRunValue = JsIinterpreter.globalObject().property(FunName);
    JsRunValue.call(JsRunValue, Args);

    //qDebug() << "TJs::fCall 3" << sizeof(JsRunValue) << QDateTime::currentMSecsSinceEpoch();

    //qDebug() << "TJs::fCall 4" << sizeof(JsIinterpreter) << QDateTime::currentMSecsSinceEpoch();
#endif

#ifdef DEF_JS_SAVE_DATA
    Q_UNUSED(FunName);

    qDebug() << "TJs::fCall";
    mJsRunValue.call(mJsRunValue, mJsRunArgs);
#endif

    return true;
}
