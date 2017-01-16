#include "qwidgetpie.h"
#include "chartpie.h"
#include <QWebFrame>
#include <QtWidgets>
#include <QWebElementCollection>
#include <QNetworkDiskCache>
QWidgetPie::QWidgetPie(QWidget *parent) : QWebView(parent)
{
    //! The object we will expose to JavaScript engine:
    m_pie = new ChartPie(this);
    // Signal is emitted before frame loads any web content:
    QObject::connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
                     this, SLOT(addJSObject()));
    load(QUrl::fromLocalFile(qApp->applicationDirPath()+"/html/DynamicPie.html"));
}

QWidgetPie::~QWidgetPie()
{

}
void QWidgetPie::addJSObject()
{
    page()->mainFrame()->addToJavaScriptWindowObject(QString("echartpie"), m_pie);
    qDebug()<<"addJSobjectpie";
}
ChartPie *QWidgetPie::charpie() const
{
    return m_pie;
}

