#include "SendTo.h"

TSendTo::TSendTo()
{
    mSender = NULL;
    mType = TYPE_UNKNOWEN;
    mValueFloat = 0.0;
    mValueInt = 0;
}

TSendTo::TSendTo(QObject *Sender, int Type, double ValueFloat,
                 int ValueInt, QString Text)
{
    mSender = Sender;
    mType = Type;
    mValueFloat = ValueFloat;
    mValueInt = ValueInt;
    mText = Text;
}

TSendTo::~TSendTo()
{}

void TSendTo::fCopy(TSendTo *Src)
{
    this->fSetSender(Src->fGetSender());
    this->fSetType(Src->fGetType());
    this->fSetValueFloat(Src->fGetValueFloat());
    this->fSetValueInt(Src->fGetValueInt());
    this->fSetText(Src->fGetText());
}

void TSendTo::fSetSender(QObject *Sender)
{
    mSender = Sender;
}

QObject* TSendTo::fGetSender()
{
    return mSender;
}

void TSendTo::fSetType(int Type)
{
    mType = Type;
}

int TSendTo::fGetType()
{
    return mType;
}

void TSendTo::fSetValueFloat(double Value)
{
    mValueFloat = Value;
}

double TSendTo::fGetValueFloat()
{
    return mValueFloat;
}

void TSendTo::fSetValueInt(int ValueInt)
{
    mValueInt = ValueInt;
}

int TSendTo::fGetValueInt()
{
    return mValueInt;
}

void TSendTo::fSetText(const QString &Text)
{
    mText = Text;
}

QString TSendTo::fGetText()
{
    return mText;
}
