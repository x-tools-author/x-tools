/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKSerialportAssistant.hh"
#include "SAKTabPageSerialportAssistant.hh"
#include "SAKSerialportAssistantController.hh"

SAKTabPageSerialportAssistant::SAKTabPageSerialportAssistant(QWidget *parent)
    :SAKDebugPage (parent)
    ,serialPortAssistant (nullptr)
    ,controller (new SAKSerialportAssistantController)
{
    setUpController();
    setWindowTitle(tr("串口调试"));
}

SAKTabPageSerialportAssistant::~SAKTabPageSerialportAssistant()
{
    delete controller;
    if (serialPortAssistant){
        serialPortAssistant->terminate();
        delete serialPortAssistant;
    }
}

void SAKTabPageSerialportAssistant::setUiEnable(bool enable)
{
    controller->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}

void SAKTabPageSerialportAssistant::changeDeviceStatus(bool opened)
{
    /*
     * 设备打开失败，使能ui, 打开成功，禁止ui
     */
    setUiEnable(!opened);
    switchPushButton->setText(opened ? tr("关闭") : tr("打开"));
    if (!opened){
        if (serialPortAssistant){
            serialPortAssistant->terminate();
            delete serialPortAssistant;
            serialPortAssistant = nullptr;
        }
    }
    emit deviceStatusChanged(opened);
}

void SAKTabPageSerialportAssistant::openOrColoseDevice()
{
    if (serialPortAssistant){
        switchPushButton->setText(tr("打开"));
        serialPortAssistant->terminate();
        delete serialPortAssistant;
        serialPortAssistant = nullptr;

        setUiEnable(true);
        emit deviceStatusChanged(false);
    }else{
        switchPushButton->setText(tr("关闭"));
        const QString name = controller->name();
        const qint32 baudRate = controller->baudRate();
        const QSerialPort::DataBits dataBits = controller->dataBits();
        const QSerialPort::StopBits stopBits = controller->stopBits();
        const QSerialPort::Parity parity = controller->parity();
        serialPortAssistant = new SAKSerialportAssistant(name, baudRate, dataBits, stopBits, parity, this);

        connect(this, &SAKTabPageSerialportAssistant::writeDataRequest,serialPortAssistant, &SAKSerialportAssistant::writeBytes);

        connect(serialPortAssistant, &SAKSerialportAssistant::bytesWriten,         this, &SAKTabPageSerialportAssistant::bytesWritten);
        connect(serialPortAssistant, &SAKSerialportAssistant::bytesRead,          this, &SAKTabPageSerialportAssistant::bytesRead);
        connect(serialPortAssistant, &SAKSerialportAssistant::messageChanged,     this, &SAKTabPageSerialportAssistant::outputMessage);
        connect(serialPortAssistant, &SAKSerialportAssistant::deviceStatuChanged, this, &SAKTabPageSerialportAssistant::changeDeviceStatus);

        serialPortAssistant->start();
    }    
}


void SAKTabPageSerialportAssistant::refreshDevice()
{
    controller->refresh();
}

QWidget *SAKTabPageSerialportAssistant::controllerWidget()
{
    return controller;
}
