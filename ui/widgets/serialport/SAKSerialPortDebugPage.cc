/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKGlobal.hh"
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKSerialPortDeviceController.hh"

SAKSerialPortDebugPage::SAKSerialPortDebugPage(QWidget *parent)
    :SAKDebugPage (SAKGlobal::SAKEnumDebugPageTypeCOM, parent)
    ,serialPortAssistant (nullptr)
    ,controller (new SAKSerialPortDeviceController)
{
    setUpController();
    setWindowTitle(tr("串口调试"));
}

SAKSerialPortDebugPage::~SAKSerialPortDebugPage()
{
    delete controller;
    if (serialPortAssistant){
        serialPortAssistant->terminate();
        delete serialPortAssistant;
    }
}

void SAKSerialPortDebugPage::setUiEnable(bool enable)
{
    controller->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}

void SAKSerialPortDebugPage::changeDeviceStatus(bool opened)
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

void SAKSerialPortDebugPage::openOrColoseDevice()
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
        serialPortAssistant = new SAKSerialPortDevice(name, baudRate, dataBits, stopBits, parity, this);

        connect(this, &SAKSerialPortDebugPage::writeDataRequest,serialPortAssistant, &SAKSerialPortDevice::writeBytes);

        connect(serialPortAssistant, &SAKSerialPortDevice::bytesWriten,         this, &SAKSerialPortDebugPage::bytesWritten);
        connect(serialPortAssistant, &SAKSerialPortDevice::bytesRead,          this, &SAKSerialPortDebugPage::bytesRead);
        connect(serialPortAssistant, &SAKSerialPortDevice::messageChanged,     this, &SAKSerialPortDebugPage::outputMessage);
        connect(serialPortAssistant, &SAKSerialPortDevice::deviceStatuChanged, this, &SAKSerialPortDebugPage::changeDeviceStatus);

        serialPortAssistant->start();
    }    
}


void SAKSerialPortDebugPage::refreshDevice()
{
    controller->refresh();
}

QWidget *SAKSerialPortDebugPage::controllerWidget()
{
    return controller;
}
