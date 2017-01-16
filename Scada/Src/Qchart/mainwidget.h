#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHash>
#include <QTableWidget>
namespace Ui {
class MainWidget;
}
class QWidgetLine;
class QWidgetPie;
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
public slots:
    void addLineInterface(QString lineInterface);
    void setLineValue(QString lineInterface, QString title, QString subTitle, QString xdata, QString ydata );

    void addPieInterface(QString pieInterface);
    void setPieValue(QString title,QString subTitle,QStringList name,QStringList value );

private:
    Ui::MainWidget *ui;
    //QTabWidget *LineTab;
    //QTabWidget *PieTab;
    QWidgetPie *m_piechart;
private:
    QHash<QString,QWidgetLine* > LineInterface;
    //QHash<QString,QWidgetPie*> PieInterface;

};

#endif // MAINWIDGET_H
