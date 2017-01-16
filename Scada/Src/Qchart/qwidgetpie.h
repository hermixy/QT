#ifndef QWIDGETPIE_H
#define QWIDGETPIE_H

#include <QWebView>
class ChartPie;
class QWidgetPie : public QWebView
{
    Q_OBJECT
public:
    explicit QWidgetPie(QWidget *parent = 0);
    ~QWidgetPie();
    ChartPie *charpie() const;

private:
    ChartPie * m_pie;


private slots:
    void addJSObject();
};

#endif // QWIDGETPIE_H
