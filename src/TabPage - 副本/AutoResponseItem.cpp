/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "AutoResponseItem.h"
#include "ui_AutoResponseItem.h"

#include <QDateTime>

AutoResponseItem::AutoResponseItem(QWidget *parent)
    :QWidget(parent)
    ,clearOutputInfoTimer(new QTimer)
    ,ui(new Ui::AutoResponseItem)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);

    Connect();
}

AutoResponseItem::~AutoResponseItem()
{
    delete ui;
    delete clearOutputInfoTimer;
}

void AutoResponseItem::Connect()
{
    connect(ui->pushButtonCancle, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pushButtonApply, SIGNAL(clicked(bool)), this, SLOT(addAotoResponseItem()));
    connect(clearOutputInfoTimer, SIGNAL(timeout()), this, SLOT(clearOutputInfo()));
}

void AutoResponseItem::addAotoResponseItem()
{
    QString receiveData = ui->lineEditReceiveData->text().trimmed();
    QString sendData = ui->lineEditSendData->text().trimmed();
    QString Description = ui->lineEditDescription->text().trimmed();

    if (!receiveData.isEmpty() && !sendData.isEmpty()){
        if (isModify){
            emit need2modifyResponseItem(receiveData, sendData, Description);
            isModify = false;
        }else {
            emit need2addAotoResponseItem(receiveData, sendData, Description);
        }
        this->close();
    }else {
        if (receiveData.isEmpty()){
            outputInfo(tr("接收数据选项不能为空！"), QString("red"));
            ui->lineEditReceiveData->setFocus();
            QApplication::beep();
        }else if(sendData.isEmpty()){
            outputInfo(tr("发送数据选项不能为空！"), QString("red"));
            ui->lineEditSendData->setFocus();
            QApplication::beep();
        }else {
            /// nothing to do.
        }
    }
}

void AutoResponseItem::outputInfo(QString info, QString color)
{
    QString dateTimeInfo = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss ");
    dateTimeInfo = QString("<font color=silver>%1<font>").arg(dateTimeInfo);

    ui->labelInfo->setText(QString("%1<font color=%2>%3</font>").arg(dateTimeInfo).arg(color).arg(info));

    /// 3秒后清空信息输出
    const int inventor = 3;
    if (clearOutputInfoTimer->isActive()){
        clearOutputInfoTimer->stop();
        clearOutputInfoTimer->start(inventor*1000);
    }else {
        clearOutputInfoTimer->start(inventor*1000);
    }
}

void AutoResponseItem::clearOutputInfo()
{
    ui->labelInfo->clear();
}

void AutoResponseItem::setText(QString receiveDataString, QString sendDataString, QString description)
{
    ui->lineEditReceiveData->setText(receiveDataString);
    ui->lineEditSendData->setText(sendDataString);
    ui->lineEditDescription->setText(description);
}

void AutoResponseItem::showModify()
{
    isModify = true;
    show();
}

void AutoResponseItem::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    isModify = false;
}

void AutoResponseItem::cancle()
{
    isModify = false;
}
