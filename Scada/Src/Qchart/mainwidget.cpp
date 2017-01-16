#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "qwidgetline.h"
#include "qwidgetpie.h"
#include "ChartLine.h"
#include "chartpie.h"
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

/*
    LineTab = new QTabWidget(this);
    LineTab->setObjectName(QStringLiteral("ChartLineTab"));

    PieTab = new QTabWidget(this);
    PieTab->setObjectName(QStringLiteral("ChartPieTab"));

    ui->ChartTab->clear();
    ui->ChartTab->addTab(LineTab,"曲线图");
    ui->ChartTab->addTab(PieTab,"饼图");

*/
    ui->linetabWidget->clear();



}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::addLineInterface(QString lineInterface)
{
    if(LineInterface.contains(lineInterface)) {
        return;
    } else {
        QWidgetLine *m_linechart = new QWidgetLine();
        LineInterface.insert(lineInterface, m_linechart);
        QWidget *widget=new QWidget();
        QVBoxLayout *layout=new QVBoxLayout();
        layout->addWidget(m_linechart);
        widget->setLayout(layout);
        ui->linetabWidget->addTab(widget,lineInterface);
        m_linechart->setMinimumSize(widget->size());
        qDebug()<<widget->size();
    }
}

void MainWidget::setLineValue(QString lineInter,QString title,QString subTitle,QString xdata,QString ydata )
{
   QWidgetLine *m_linechart = LineInterface.value(lineInter);
   if(!m_linechart)
       return;
   m_linechart->charlin()->setTitle(title);
   m_linechart->charlin()->setSubTitle(subTitle);
   m_linechart->charlin()->setXdata(xdata);
   m_linechart->charlin()->setYdata(ydata);
}

void MainWidget::addPieInterface(QString pieInterface)
{
    m_piechart = new  QWidgetPie();
    //PieTab->addTab(m_piechart,pieInterface);
    ui->horizontalLayout_3->addWidget(m_piechart);
}


void MainWidget::setPieValue(QString title,QString subTitle,QStringList name,QStringList value )
{

    m_piechart->charpie()->setTitle(title);
    m_piechart->charpie()->setSubTitle(subTitle);
    m_piechart->charpie()->setdata(name,value);
}
