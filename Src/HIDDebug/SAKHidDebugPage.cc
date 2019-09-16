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

<<<<<<< HEAD
#include "SAKHidDevice.hh"
#include "SAKHidDebugPage.hh"
#include "SAKHidDeviceController.hh"

SAKHidDebugPage::SAKHidDebugPage(QWidget *parent)
    :SAKDebugPage (parent)
    ,udpDevice (nullptr)
    ,udpDeviceController (new SAKHidDeviceController)
=======
#include "SAKUdpDevice.hh"
#include "SAKUdpDebugPage.hh"
#include "SAKUdpDeviceController.hh"

SAKUdpDebugPage::SAKUdpDebugPage(QWidget *parent)
    :SAKDebugPage (parent)
    ,udpDevice (nullptr)
    ,udpDeviceController (new SAKUdpDeviceController)
>>>>>>> developer
{
    setUpController();
    setWindowTitle(tr("UDP调试"));
}

<<<<<<< HEAD
SAKHidDebugPage::~SAKHidDebugPage()
=======
SAKUdpDebugPage::~SAKUdpDebugPage()
>>>>>>> developer
{
    udpDeviceController->deleteLater();
    if (udpDevice){
        udpDevice->terminate();
        delete udpDevice;
    }
}

<<<<<<< HEAD
void SAKHidDebugPage::setUiEnable(bool enable)
=======
void SAKUdpDebugPage::setUiEnable(bool enable)
>>>>>>> developer
{
    udpDeviceController->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}

<<<<<<< HEAD
void SAKHidDebugPage::changeDeviceStatus(bool opened)
=======
void SAKUdpDebugPage::changeDeviceStatus(bool opened)
>>>>>>> developer
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

<<<<<<< HEAD
void SAKHidDebugPage::openOrColoseDevice()
=======
void SAKUdpDebugPage::openOrColoseDevice()
>>>>>>> developer
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

<<<<<<< HEAD
        udpDevice = new SAKHidDevice(localHost, localPort, customSetting, targetHost, targetPort, this);

        connect(this, &SAKHidDebugPage::writeDataRequest,udpDevice, &SAKHidDevice::writeBytes);

        connect(udpDevice, &SAKHidDevice::bytesWriten,          this, &SAKHidDebugPage::bytesWritten);
        connect(udpDevice, &SAKHidDevice::bytesRead,            this, &SAKHidDebugPage::bytesRead);
        connect(udpDevice, &SAKHidDevice::messageChanged,       this, &SAKHidDebugPage::outputMessage);
        connect(udpDevice, &SAKHidDevice::deviceStatuChanged,   this, &SAKHidDebugPage::changeDeviceStatus);
=======
        udpDevice = new SAKUdpDevice(localHost, localPort, customSetting, targetHost, targetPort, this);

        connect(this, &SAKUdpDebugPage::writeDataRequest,udpDevice, &SAKUdpDevice::writeBytes);

        connect(udpDevice, &SAKUdpDevice::bytesWriten,          this, &SAKUdpDebugPage::bytesWritten);
        connect(udpDevice, &SAKUdpDevice::bytesRead,            this, &SAKUdpDebugPage::bytesRead);
        connect(udpDevice, &SAKUdpDevice::messageChanged,       this, &SAKUdpDebugPage::outputMessage);
        connect(udpDevice, &SAKUdpDevice::deviceStatuChanged,   this, &SAKUdpDebugPage::changeDeviceStatus);
>>>>>>> developer

        udpDevice->start();
    }    
}


<<<<<<< HEAD
void SAKHidDebugPage::refreshDevice()
=======
void SAKUdpDebugPage::refreshDevice()
>>>>>>> developer
{
    udpDeviceController->refresh();
}

<<<<<<< HEAD
QWidget *SAKHidDebugPage::controllerWidget()
=======
QWidget *SAKUdpDebugPage::controllerWidget()
>>>>>>> developer
{
    return udpDeviceController;
}
