#include "qwidgetline.h"
#include "ChartLine.h"
#include <QWebFrame>
#include <QWebElementCollection>
#include <QNetworkDiskCache>

QWidgetLine::QWidgetLine(QWidget *parent) : QWebView(parent)
{
    //! The object we will expose to JavaScript engine:
    m_line = new EChartTest(this);
    // Signal is emitted before frame loads any web content:
    QObject::connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
                     this, SLOT(addJSObject()));
    //load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/html/dynamicline.html"));
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/html/dynamicline.html"));


}

QWidgetLine::~QWidgetLine()
{

}
void QWidgetLine::addJSObject()
{
    page()->mainFrame()->addToJavaScriptWindowObject(QString("echarttest"), m_line);
    qDebug()<<"addJSobject";
}
EChartTest *QWidgetLine::charlin() const
{
    return m_line;
}
