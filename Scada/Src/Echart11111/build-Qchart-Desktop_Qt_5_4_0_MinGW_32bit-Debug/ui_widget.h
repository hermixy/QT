/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWebView *webView;
    QWebView *webView_pie;
    QWebView *webView_bar;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(884, 498);
        Widget->setContextMenuPolicy(Qt::NoContextMenu);
        webView = new QWebView(Widget);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setGeometry(QRect(20, 10, 811, 181));
        webView->setUrl(QUrl(QStringLiteral("about:blank")));
        webView_pie = new QWebView(Widget);
        webView_pie->setObjectName(QStringLiteral("webView_pie"));
        webView_pie->setGeometry(QRect(20, 200, 401, 261));
        webView_pie->setUrl(QUrl(QStringLiteral("about:blank")));
        webView_bar = new QWebView(Widget);
        webView_bar->setObjectName(QStringLiteral("webView_bar"));
        webView_bar->setGeometry(QRect(440, 200, 391, 261));
        webView_bar->setUrl(QUrl(QStringLiteral("about:blank")));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
