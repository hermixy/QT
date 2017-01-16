#include "chartpie.h"

ChartPie::ChartPie(QObject *parent) : QObject(parent)
{

}

ChartPie::~ChartPie()
{

}

QStringList ChartPie::getNamedata()
{
    return NameData;
}
void ChartPie::setdata(QStringList name,QStringList value)
{
    NameData = name;
    ValueData = value;
}

QStringList ChartPie::getValuedata()
{
    return ValueData;
}

QString ChartPie::getTitle()
{
    return Title;
}

void ChartPie::setTitle(QString title)
{
    Title = title;
}

QString ChartPie::getSubTitle()
{
    return SubTitle;
}

void ChartPie::setSubTitle(QString subtitle)
{
    SubTitle = subtitle;
}

int ChartPie::dataNo()
{
    dataAmount = NameData.size();
    return dataAmount;
}
