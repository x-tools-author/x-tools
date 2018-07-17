#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "senddatapanel.h"
#include "ui_senddatapanel.h"

#include <QApplication>
#include <QIntValidator>

SendDataPanel::SendDataPanel(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SendDataPanel),
    mpCycleTimer(new QTimer)
{
    ui->setupUi(this);
    InitUi();
    Connect();
}

SendDataPanel::~SendDataPanel()
{

}

void SendDataPanel::Connect()
{
    /// 发送数据
    connect(ui->pushButton_send, SIGNAL(clicked(bool)), this, SLOT(SendData()));
    /// 循环发送设置
    connect(ui->checkBox_cycle, SIGNAL(clicked(bool)),this, SLOT(CycleSetting(bool)));
    /// 循环发送周期
    connect(mpCycleTimer, SIGNAL(timeout()), this, SLOT(CycleTimerTimeout()));
}

void SendDataPanel::InitUi()
{
    /// 输入框输入内容限制
    ui->lineEdit_cycleTime->setMaxLength(9);
    ui->lineEdit_cycleTime->setValidator(new QIntValidator(10, 999999999, this));
}

void SendDataPanel::SendData()
{
    int cycleTime = 0;
    if (ui->checkBox_cycle->isChecked()){
        emit Need2WriteData();
        cycleTime = (ui->lineEdit_cycleTime->text().toInt());
        ui->pushButton_send->setText(tr("正在发送"));
        ui->pushButton_send->setEnabled(false);
        mpCycleTimer->start(cycleTime);
    }else {
        emit Need2WriteData();
    }
}

void SendDataPanel::CycleSetting(bool isChecked)
{
    if (isChecked){
        if (ui->lineEdit_cycleTime->text().isEmpty()){
            ui->label_errorString->setText(QString("<font color=red>%1</font>").arg(tr("请输入循环周期！")));
            QApplication::beep();
            ui->lineEdit_cycleTime->setFocus();
            ui->checkBox_cycle->setChecked(false);
        }else {
            ui->lineEdit_cycleTime->setEnabled(false);
        }
    }else {
        ui->lineEdit_cycleTime->setEnabled(true);
        ui->label_errorString->clear();
        ui->pushButton_send->setText(tr("发送"));
        ui->pushButton_send->setEnabled(true);
        mpCycleTimer->stop();
    }
}

void SendDataPanel::CycleTimerTimeout()
{
    emit Need2WriteData();
}
