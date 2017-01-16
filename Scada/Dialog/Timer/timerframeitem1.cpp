#include "timerframeitem1.h"
#include "ui_timerframeitem1.h"
#include <QDebug>

TimerFrameItem1::TimerFrameItem1(QString name, QList<TimerOnOff> timerOn, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimerFrameItem1)
{
    ui->setupUi(this);
    ui->labelGroupName->setText(name);
    if(timerOn.size()==0)
    {
        return;
    }
    /****************************************/
    ui->timeEdit1_1->setTime(timerOn.at(0).time);
    ui->timeEdit2_1->setTime(timerOn.at(1).time);
    ui->timeEdit3_1->setTime(timerOn.at(2).time);
    ui->timeEdit4_1->setTime(timerOn.at(3).time);

    ui->timeEdit5_1->setTime(timerOn.at(4).time);
    ui->timeEdit6_1->setTime(timerOn.at(5).time);
    ui->timeEdit7_1->setTime(timerOn.at(6).time);

    ui->timeEdit8_1->setTime(timerOn.at(7).time);
    ui->timeEdit9_1->setTime(timerOn.at(8).time);
    ui->timeEdit10_1->setTime(timerOn.at(9).time);
    ui->timeEdit11_1->setTime(timerOn.at(10).time);
    ui->timeEdit12_1->setTime(timerOn.at(11).time);
    ui->timeEdit13_1->setTime(timerOn.at(12).time);
    ui->timeEdit14_1->setTime(timerOn.at(13).time);
    /***************************************************/
    ui->timeEdit1_2->setTime(timerOn.at(14).time);
    ui->timeEdit2_2->setTime(timerOn.at(15).time);
    ui->timeEdit3_2->setTime(timerOn.at(16).time);
    ui->timeEdit4_2->setTime(timerOn.at(17).time);
    ui->timeEdit5_2->setTime(timerOn.at(18).time);
    ui->timeEdit6_2->setTime(timerOn.at(19).time);

    ui->timeEdit7_2->setTime(timerOn.at(20).time);
    ui->timeEdit8_2->setTime(timerOn.at(21).time);
    ui->timeEdit9_2->setTime(timerOn.at(22).time);
    ui->timeEdit10_2->setTime(timerOn.at(23).time);
    ui->timeEdit11_2->setTime(timerOn.at(24).time);
    ui->timeEdit12_2->setTime(timerOn.at(25).time);
    ui->timeEdit13_2->setTime(timerOn.at(26).time);
    ui->timeEdit14_2->setTime(timerOn.at(27).time);

}

TimerFrameItem1::~TimerFrameItem1()
{
    delete ui;
}
QList<TimerOnOff> TimerFrameItem1::getTimer()
{
    QList<TimerOnOff> timelist;

    TimerOnOff onOFF1_1;
    onOFF1_1.day=1;
    onOFF1_1.time=ui->timeEdit1_1->time();
    onOFF1_1.onOff=1;

    TimerOnOff onOFF2_1;
    onOFF2_1.day=1;
    onOFF2_1.time=ui->timeEdit2_1->time();
    onOFF2_1.onOff=0;

    TimerOnOff onOFF3_1;
    onOFF3_1.day=2;
    onOFF3_1.time=ui->timeEdit3_1->time();
    onOFF3_1.onOff=1;

    TimerOnOff onOFF4_1;
    onOFF4_1.day=2;
    onOFF4_1.time=ui->timeEdit4_1->time();
    onOFF4_1.onOff=0;

    TimerOnOff onOFF5_1;
    onOFF5_1.day=3;
    onOFF5_1.time=ui->timeEdit5_1->time();
    onOFF5_1.onOff=1;

    TimerOnOff onOFF6_1;
    onOFF6_1.day=3;
    onOFF6_1.time=ui->timeEdit6_1->time();
    onOFF6_1.onOff=0;

    TimerOnOff onOFF7_1;
    onOFF7_1.day=4;
    onOFF7_1.time=ui->timeEdit7_1->time();
    onOFF7_1.onOff=1;

    TimerOnOff onOFF8_1;
    onOFF8_1.day=4;
    onOFF8_1.time=ui->timeEdit8_1->time();
    onOFF8_1.onOff=0;

    TimerOnOff onOFF9_1;
    onOFF9_1.day=5;
    onOFF9_1.time=ui->timeEdit9_1->time();
    onOFF9_1.onOff=1;

    TimerOnOff onOFF10_1;
    onOFF10_1.day=5;
    onOFF10_1.time=ui->timeEdit10_1->time();
    onOFF10_1.onOff=0;

    TimerOnOff onOFF11_1;
    onOFF11_1.day=6;
    onOFF11_1.time=ui->timeEdit11_1->time();
    onOFF11_1.onOff=1;

    TimerOnOff onOFF12_1;
    onOFF12_1.day=6;
    onOFF12_1.time=ui->timeEdit12_1->time();
    onOFF12_1.onOff=0;

    TimerOnOff onOFF13_1;
    onOFF13_1.day=7;
    onOFF13_1.time=ui->timeEdit13_1->time();
    onOFF13_1.onOff=1;

    TimerOnOff onOFF14_1;
    onOFF14_1.day=7;
    onOFF14_1.time=ui->timeEdit14_1->time();
    onOFF14_1.onOff=0;
    /********************************/
    TimerOnOff onOFF1_2;
    onOFF1_2.day=1;
    onOFF1_2.time=ui->timeEdit1_2->time();
    onOFF1_2.onOff=1;

    TimerOnOff onOFF2_2;
    onOFF2_2.day=1;
    onOFF2_2.time=ui->timeEdit2_2->time();
    onOFF2_2.onOff=0;

    TimerOnOff onOFF3_2;
    onOFF3_2.day=2;
    onOFF3_2.time=ui->timeEdit3_2->time();
    onOFF3_2.onOff=1;

    TimerOnOff onOFF4_2;
    onOFF4_2.day=2;
    onOFF4_2.time=ui->timeEdit4_2->time();
    onOFF4_2.onOff=0;

    TimerOnOff onOFF5_2;
    onOFF5_2.day=3;
    onOFF5_2.time=ui->timeEdit5_2->time();
    onOFF5_2.onOff=1;

    TimerOnOff onOFF6_2;
    onOFF6_2.day=3;
    onOFF6_2.time=ui->timeEdit6_2->time();
    onOFF6_2.onOff=0;

    TimerOnOff onOFF7_2;
    onOFF7_2.day=4;
    onOFF7_2.time=ui->timeEdit7_2->time();
    onOFF7_2.onOff=1;

    TimerOnOff onOFF8_2;
    onOFF8_2.day=4;
    onOFF8_2.time=ui->timeEdit8_2->time();
    onOFF8_2.onOff=0;

    TimerOnOff onOFF9_2;
    onOFF9_2.day=5;
    onOFF9_2.time=ui->timeEdit9_2->time();
    onOFF9_2.onOff=1;

    TimerOnOff onOFF10_2;
    onOFF10_2.day=5;
    onOFF10_2.time=ui->timeEdit10_2->time();
    onOFF10_2.onOff=0;

    TimerOnOff onOFF11_2;
    onOFF11_2.day=6;
    onOFF11_2.time=ui->timeEdit11_2->time();
    onOFF11_2.onOff=1;

    TimerOnOff onOFF12_2;
    onOFF12_2.day=6;
    onOFF12_2.time=ui->timeEdit12_2->time();
    onOFF12_2.onOff=0;

    TimerOnOff onOFF13_2;
    onOFF13_2.day=7;
    onOFF13_2.time=ui->timeEdit13_2->time();
    onOFF13_2.onOff=1;

    TimerOnOff onOFF14_2;
    onOFF14_2.day=7;
    onOFF14_2.time=ui->timeEdit14_2->time();
    onOFF14_2.onOff=0;
    /*********************************/

    if(onOFF1_1.time!=onOFF2_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF1_1);
        timelist.append(onOFF2_1);
    }
    else
    {
        onOFF1_1.onOff=-1;
        onOFF2_1.onOff=-1;
        timelist.append(onOFF1_1);
        timelist.append(onOFF2_1);
        onOFF1_1.time=QTime();
        onOFF2_1.time=QTime();
    }
    if(onOFF3_1.time!=onOFF4_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF3_1);
        timelist.append(onOFF4_1);
    }
    else
    {
        onOFF3_1.onOff=-1;
        onOFF4_1.onOff=-1;
        timelist.append(onOFF3_1);
        timelist.append(onOFF4_1);
        onOFF3_1.time=QTime();
        onOFF4_1.time=QTime();
    }
    if(onOFF5_1.time!=onOFF6_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF5_1);
        timelist.append(onOFF6_1);
    }
    else
    {
        onOFF5_1.onOff=-1;
        onOFF6_1.onOff=-1;
        timelist.append(onOFF5_1);
        timelist.append(onOFF6_1);
        onOFF5_1.time=QTime();
        onOFF6_1.time=QTime();
    }
    if(onOFF7_1.time!=onOFF8_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF7_1);
        timelist.append(onOFF8_1);

    }
    else
    {
        onOFF7_1.onOff=-1;
        onOFF8_1.onOff=-1;
        timelist.append(onOFF7_1);
        timelist.append(onOFF8_1);
        onOFF7_1.time=QTime();
        onOFF8_1.time=QTime();
    }
    if(onOFF9_1.time!=onOFF10_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF9_1);
        timelist.append(onOFF10_1);
    }
    else
    {
        onOFF9_1.onOff=-1;
        onOFF10_1.onOff=-1;
        timelist.append(onOFF9_1);
        timelist.append(onOFF10_1);
        onOFF9_1.time=QTime();
        onOFF10_1.time=QTime();
    }
    if(onOFF11_1.time!=onOFF12_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF11_1);
        timelist.append(onOFF12_1);
    }
    else
    {
        onOFF11_1.onOff=-1;
        onOFF12_1.onOff=-1;
        timelist.append(onOFF11_1);
        timelist.append(onOFF12_1);
        onOFF11_1.time=QTime();
        onOFF12_1.time=QTime();
    }
    if(onOFF13_1.time!=onOFF14_1.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF13_1);
        timelist.append(onOFF14_1);
    }
    else
    {
        onOFF13_1.onOff=-1;
        onOFF14_1.onOff=-1;
        timelist.append(onOFF13_1);
        timelist.append(onOFF14_1);
        onOFF13_1.time=QTime();
        onOFF14_1.time=QTime();
    }
   /**********************************/
    if(onOFF1_2.time!=onOFF2_2.time) {
        timelist.append(onOFF1_2);
        timelist.append(onOFF2_2);
    }
    else
    {
        onOFF1_2.onOff=-1;
        onOFF2_2.onOff=-1;
        timelist.append(onOFF1_2);
        timelist.append(onOFF2_2);
        onOFF1_2.time=QTime();
        onOFF2_2.time=QTime();
    }
    if(onOFF3_2.time!=onOFF4_2.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF3_2);
        timelist.append(onOFF4_2);
    }
    else
    {
        onOFF3_2.onOff=-1;
        onOFF4_2.onOff=-1;
        timelist.append(onOFF3_2);
        timelist.append(onOFF4_2);
        onOFF3_2.time=QTime();
        onOFF4_2.time=QTime();
    }
    if(onOFF5_2.time!=onOFF6_2.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF5_2);
        timelist.append(onOFF6_2);
    }
    else
    {
        onOFF5_2.onOff=-1;
        onOFF6_2.onOff=-1;
        timelist.append(onOFF5_2);
        timelist.append(onOFF6_2);
        onOFF5_2.time=QTime();
        onOFF6_2.time=QTime();
    }
    if(onOFF7_2.time!=onOFF8_2.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF7_2);
        timelist.append(onOFF8_2);
    }
    else
    {
        onOFF7_2.onOff=-1;
        onOFF8_2.onOff=-1;
        timelist.append(onOFF7_2);
        timelist.append(onOFF8_2);
        onOFF7_2.time=QTime();
        onOFF8_2.time=QTime();
    }
    if(onOFF9_2.time!=onOFF10_2.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF9_2);
        timelist.append(onOFF10_2);
    }
    else
    {
        onOFF9_2.onOff=-1;
        onOFF10_2.onOff=-1;
        timelist.append(onOFF9_2);
        timelist.append(onOFF10_2);
        onOFF9_2.time=QTime();
        onOFF10_2.time=QTime();
    }
    if(onOFF11_2.time!=onOFF12_2.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF11_2);
        timelist.append(onOFF12_2);
    }
    else
    {
        onOFF11_2.onOff=-1;
        onOFF12_2.onOff=-1;
        timelist.append(onOFF11_2);
        timelist.append(onOFF12_2);
        onOFF11_2.time=QTime();
        onOFF12_2.time=QTime();
    }
    if(onOFF13_2.time!=onOFF14_2.time) {
        qDebug()<<"sssssss";
        timelist.append(onOFF13_2);
        timelist.append(onOFF14_2);
    }
    else
    {
        onOFF13_2.onOff=-1;
        onOFF14_2.onOff=-1;
        timelist.append(onOFF13_2);
        timelist.append(onOFF14_2);
        onOFF13_2.time=QTime();
        onOFF14_2.time=QTime();
    }
    return timelist;

}
