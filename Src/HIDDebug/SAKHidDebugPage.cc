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

#include "SAKHidDevice.hh"
#include "SAKHidDebugPage.hh"
#include "SAKHidDeviceController.hh"

SAKHidDebugPage::SAKHidDebugPage(QWidget *parent)
    :SAKDebugPage (parent)
    ,udpDevice (nullptr)
    ,udpDeviceController (new SAKHidDeviceController)
{
    setUpController();
    setWindowTitle(tr("UDP调试"));
}

SAKHidDebugPage::~SAKHidDebugPage()
{
    udpDeviceController->deleteLater();
    if (udpDevice){
        udpDevice->terminate();
        delete udpDevice;
    }
}

void SAKHidDebugPage::setUiEnable(bool enable)
{
    udpDeviceController->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}

void SAKHidDebugPage::changeDeviceStatus(bool opened)
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

void SAKHidDebugPage::openOrColoseDevice()
{
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

        udpDevice = new SAKHidDevice(localHost, localPort, customSetting, targetHost, targetPort, this);

        connect(this, &SAKHidDebugPage::writeDataRequest,udpDevice, &SAKHidDevice::writeBytes);

        connect(udpDevice, &SAKHidDevice::bytesWriten,          this, &SAKHidDebugPage::bytesWritten);
        connect(udpDevice, &SAKHidDevice::bytesRead,            this, &SAKHidDebugPage::bytesRead);
        connect(udpDevice, &SAKHidDevice::messageChanged,       this, &SAKHidDebugPage::outputMessage);
        connect(udpDevice, &SAKHidDevice::deviceStatuChanged,   this, &SAKHidDebugPage::changeDeviceStatus);

        udpDevice->start();
    }    
}


void SAKHidDebugPage::refreshDevice()
{
    udpDeviceController->refresh();
}

QWidget *SAKHidDebugPage::controllerWidget()
{
    return udpDeviceController;
}
