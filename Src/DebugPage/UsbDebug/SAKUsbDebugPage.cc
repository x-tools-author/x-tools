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

#include "SAKUsbDevice.hh"
#include "SAKUsbDebugPage.hh"
#include "SAKUsbDeviceController.hh"

SAKUsbDebugPage::SAKUsbDebugPage(QWidget *parent)
    :SAKDebugPage (parent)
    ,udpDevice (nullptr)
    ,udpDeviceController (new SAKUsbDeviceController)
{
    setUpController();
    setWindowTitle(tr("UDP调试"));
}

SAKUsbDebugPage::~SAKUsbDebugPage()
{
    udpDeviceController->deleteLater();
    if (udpDevice){
        udpDevice->terminate();
        delete udpDevice;
    }
}

void SAKUsbDebugPage::setUiEnable(bool enable)
{
    udpDeviceController->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}

void SAKUsbDebugPage::changeDeviceStatus(bool opened)
{
    /*
     * 设备打开失败，使能ui, 打开成功，禁止ui
     */
    setUiEnable(!opened);
    switchPushButton->setText(opened ? tr("关闭") : tr("打开"));
    if (!opened){
        if (udpDevice){
            udpDevice->terminate();
            delete udpDevice;
            udpDevice = nullptr;
        }
    }
    emit deviceStatusChanged(opened);
}

void SAKUsbDebugPage::openOrColoseDevice()
{/*
    if (udpDevice){
        switchPushButton->setText(tr("打开"));
        udpDevice->terminate();
        delete udpDevice;
        udpDevice = nullptr;

        setUiEnable(true);
        emit deviceStatusChanged(false);
    }else{
        switchPushButton->setText(tr("关闭"));
        QString localHost = udpDeviceController->localHost();
        quint16 localPort = udpDeviceController->localPort();
        bool customSetting = udpDeviceController->enableCustomLocalSetting();
        QString targetHost = udpDeviceController->targetHost();
        quint16 targetPort = udpDeviceController->targetPort();

        udpDevice = new SAKUdpDevice(localHost, localPort, customSetting, targetHost, targetPort, this);

        connect(this, &SAKUdpDebugPage::writeDataRequest,udpDevice, &SAKUdpDevice::writeBytes);

        connect(udpDevice, &SAKUdpDevice::bytesWriten,          this, &SAKUdpDebugPage::bytesWritten);
        connect(udpDevice, &SAKUdpDevice::bytesRead,            this, &SAKUdpDebugPage::bytesRead);
        connect(udpDevice, &SAKUdpDevice::messageChanged,       this, &SAKUdpDebugPage::outputMessage);
        connect(udpDevice, &SAKUdpDevice::deviceStatuChanged,   this, &SAKUdpDebugPage::changeDeviceStatus);

        udpDevice->start();
    }    */
}


void SAKUsbDebugPage::refreshDevice()
{
    udpDeviceController->refresh();
}

QWidget *SAKUsbDebugPage::controllerWidget()
{
    return udpDeviceController;
}
