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
    :SAKTabPage (parent)
    ,serialPortAssistant (nullptr)
    ,controller (new SAKSerialportAssistantController)
{
    setUpController();       
}

SAKTabPageSerialportAssistant::~SAKTabPageSerialportAssistant()
{
    controller->deleteLater();
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
            initSignalAndSlot(false);
        }
    }
    emit deviceStatusChanged(opened);
}
void SAKTabPageSerialportAssistant::openOrColoseDevice()
{
    if (serialPortAssistant){
        switchPushButton->setText(tr("打开"));
        initSignalAndSlot(false);
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
        serialPortAssistant = new SAKSerialportAssistant(name,
                                                         baudRate,
                                                         dataBits,
                                                         stopBits,
                                                         parity,
                                                         this);
        initSignalAndSlot(true);

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

void SAKTabPageSerialportAssistant::initSignalAndSlot(bool needToConnect)
{
    if (needToConnect){
        connect(serialPortAssistant, &SAKSerialportAssistant::deviceStatuChanged,
                this, &SAKTabPageSerialportAssistant::changeDeviceStatus);
        connect(this, &SAKTabPageSerialportAssistant::need2writeBytes,
                serialPortAssistant, &SAKSerialportAssistant::writeBytes);
        connect(serialPortAssistant, &SAKSerialportAssistant::bytesRead,
                this, &SAKTabPageSerialportAssistant::bytesRead);
        connect(serialPortAssistant, &SAKSerialportAssistant::bytesWrite,
                this, &SAKTabPageSerialportAssistant::bytesWritten);
    }else{
        disconnect(serialPortAssistant, &SAKSerialportAssistant::deviceStatuChanged,
                this, &SAKTabPageSerialportAssistant::changeDeviceStatus);
        disconnect(this, &SAKTabPageSerialportAssistant::need2writeBytes,
                serialPortAssistant, &SAKSerialportAssistant::writeBytes);
        disconnect(serialPortAssistant, &SAKSerialportAssistant::bytesRead,
                this, &SAKTabPageSerialportAssistant::bytesRead);
        disconnect(serialPortAssistant, &SAKSerialportAssistant::bytesWrite,
                this, &SAKTabPageSerialportAssistant::bytesWritten);
    }
}
