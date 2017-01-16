#ifndef CHARTPIE_H
#define CHARTPIE_H

#include <QObject>
#include <QFutureWatcher>
#include <QtWidgets>

class ChartPie : public QObject
{
    Q_OBJECT
public:
    explicit ChartPie(QObject *parent = 0);
    ~ChartPie();

public slots:

    QStringList getNamedata();
    void setdata(QStringList name,QStringList value);
    QStringList getValuedata();

    QString getTitle();
    void setTitle(QString title);
    QString getSubTitle();
    void setSubTitle(QString subtitle);

    int dataNo();


private:
    QStringList NameData;
    QStringList ValueData;
    QString Title;
    QString SubTitle;
    int dataAmount;
};

#endif // CHARTPIE_H
