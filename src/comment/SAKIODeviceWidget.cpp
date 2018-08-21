/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include "SAKIODeviceWidget.h"

#include "ui_SAKIODeviceWidget.h"

SAKIODeviceWidget::SAKIODeviceWidget(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent)
    :QWidget(parent)
    ,device(_device)
    ,controler(_controler)
    ,ui(new Ui::SAKIODeviceWidget)
    ,cycleTimer(new QTimer)
    ,customControlerLayout(new QHBoxLayout)
{
    ui->setupUi(this);
    Connect();
    setCustomControler(controler);
}

SAKIODeviceWidget::~SAKIODeviceWidget()
{
    delete ui;
}

void SAKIODeviceWidget::openOrClose()
{
    if (device->isOpen()){
        close();
    }else{
        open();
    }
}

void SAKIODeviceWidget::setCustomControler(SAKIODeviceControler *controler)
{
    customControlerLayout->addWidget(controler);
    customControlerLayout->setMargin(0);
    ui->groupBoxCustom->setLayout(customControlerLayout);
}

void SAKIODeviceWidget::Connect()
{
    /// 打开/关闭设备
    connect(ui->pushButtonOpen, SIGNAL(clicked(bool)), this, SLOT(openOrClose()));
    connect(this, SIGNAL(need2open()), controler, SLOT(open()));
    connect(this, SIGNAL(need2close()), device, SLOT(close()));
    connect(controler, SIGNAL(need2open(QString,QString,QString,QString)), device, SLOT(open(QString,QString,QString,QString,QString)));
    connect(controler, SIGNAL(need2open(QString,QString,QString,QString,QString)), device, SLOT(open(QString,QString,QString,QString,QString)));
    connect(device, SIGNAL(afterDeviceOpen()), this, SLOT(afterDeviceOpen()));
    connect(device, SIGNAL(afterDeviceClose()), this, SLOT(afterDeviceClose()));

    /// 刷新设备
    connect(ui->pushButtonRefresh, SIGNAL(clicked(bool)), controler, SLOT(refresh()));

    connect(cycleTimer, SIGNAL(timeout()), this, SLOT(cycleTimerTimeout()));
}

void SAKIODeviceWidget::afterDeviceOpen()
{
    controler->afterDeviceOpen();
    ui->pushButtonRefresh->setEnabled(false);
    ui->pushButtonOpen->setText(tr("关闭"));
}

void SAKIODeviceWidget::afterDeviceClose()
{
    controler->afterDeviceClose();
    ui->pushButtonRefresh->setEnabled(true);
    ui->pushButtonOpen->setText(tr("打开"));
}

void SAKIODeviceWidget::outputInfo(QString info, QString color, bool prefix)
{
    QString strTemp;
    if (prefix){
        strTemp += QDateTime::currentDateTime().toString("<font color=silver>yyyy/MM/dd hh:mm:ss </font>");
    }

    strTemp += QString("<font color=%1>%2</font>").arg(color).arg(info);
    ui->labelOutput->setText(strTemp);
}

void SAKIODeviceWidget::writeBytes()
{
    if (ui->checkBoxCycle->isChecked()){
        bool isOK;
        int cycleTime = ui->lineEditCycleTime->text().toInt(&isOK);
        if (isOK){
            if (cycleTimer->isActive()){
                cycleTimer->stop();
                cycleTimer->start(cycleTime);
                ui->pushButtonSendData->setEnabled(false);
            }
        }else{
            outputInfo(tr("循环周期装换为整数出错！"), "red");
        }
    }else{
        if (state()){
            emit need2writeBytes(dataBytes());
        }else {
            outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
        }
    }
}

void SAKIODeviceWidget::CheckedBoxCycleClicked(bool checked)
{
    if (checked){

    }else{
        if (cycleTimer->isActive()){
            cycleTimer->stop();
        }
        ui->pushButtonSendData->setEnabled(true);
    }
}

void SAKIODeviceWidget::cycleTimerTimeout()
{
    if (state()){
        emit need2writeBytes(dataBytes());
    }else {
        outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
    }
}

QByteArray SAKIODeviceWidget::dataBytes()
{
    QByteArray data = "hello world!";

    return data;
}

void SAKIODeviceWidget::bytesRead(QByteArray data)
{
    Q_UNUSED(data);
}
