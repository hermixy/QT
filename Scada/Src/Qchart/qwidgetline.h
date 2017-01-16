#ifndef QWIDGETLINE_H
#define QWIDGETLINE_H

#include <QWebView>

class EChartTest;
class QWidgetLine : public QWebView
{
    Q_OBJECT
public:
    explicit QWidgetLine(QWidget *parent = 0);
    ~QWidgetLine();
    EChartTest *charlin() const;

public:
    EChartTest * m_line;


private slots:
    void addJSObject();
};

#endif // QWIDGETLINE_H
