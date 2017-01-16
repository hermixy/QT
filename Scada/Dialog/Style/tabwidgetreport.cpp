#include "tabwidgetreport.h"
#include "ui_reportform.h"
#include <QDebug>

TabWidgetReport::TabWidgetReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(100);
}

TabWidgetReport::~TabWidgetReport()
{
    delete ui;
}

void TabWidgetReport::on_pushButton_clicked()  //add report
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    reportName<<ui->lineEdit->text();
    ui->tableWidget->setVerticalHeaderLabels(reportName);
    ui->lineEdit->clear();
}

void TabWidgetReport::on_pushButton_3_clicked()   //delete report
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());

}

void TabWidgetReport::on_pushButton_2_clicked()
{
    ok=true;
    for(int i=0; i<ui->tableWidget->rowCount(); i++)
    {
        QStringList rowcontent;
        for(int j=0; j<ui->tableWidget->columnCount(); j++)
        {
            if(ui->tableWidget->item(i, j))
            {
                rowcontent<<ui->tableWidget->item(i, j)->text();
            }
        }
        qDebug()<<content.size();
        content.append(rowcontent);
        qDebug()<<content.size()<<rowcontent;
    }
    close();
}
