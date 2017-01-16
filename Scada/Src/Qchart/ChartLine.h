#ifndef EChartTest_H
#define EChartTest_H

#include <QFutureWatcher>
#include <QtWidgets>
#include <QMutex>

class EChartTest : public QObject
{
    Q_OBJECT
public:
    EChartTest(QObject * parent=0);
	~EChartTest();

public slots:

    QStringList getXdata();
    void setXdata(QString datatime);
    QStringList getYdata();
    void setYdata(QString value);
    QString getTitle();
    void setTitle(QString title);
    //void setTitle();
    QString getSubTitle();
    void setSubTitle(QString Subtitle);
    //void setSubTitle();


private:
    QStringList Xdata;
    QStringList Ydata;
    QString Title;
    QString SubTitle;
    QMutex mutexY;
    QMutex mutexX;
    QMutex mutexTitle;
    QMutex mutexSubTitle;
};

#endif
