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
    ,hidDevice (nullptr)
    ,hidDeviceController (new SAKHidDeviceController)
{
    setUpController();
    setWindowTitle(tr("UDP调试"));
}

SAKHidDebugPage::~SAKHidDebugPage()
{
    hidDeviceController->deleteLater();
    if (hidDevice){
        hidDevice->terminate();
        delete hidDevice;
    }
}

void SAKHidDebugPage::setUiEnable(bool enable)
{
    hidDeviceController->setEnabled(enable);
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
        if (hidDevice){
            hidDevice->terminate();
            delete hidDevice;
            hidDevice = nullptr;
        }
    }
    emit deviceStatusChanged(opened);
}

void SAKHidDebugPage::openOrColoseDevice()
{
    if (hidDevice){
        switchPushButton->setText(tr("打开"));
        hidDevice->terminate();
        delete hidDevice;
        hidDevice = nullptr;

        setUiEnable(true);
        emit deviceStatusChanged(false);
    }else{
        switchPushButton->setText(tr("关闭"));
        hidDevice = new SAKHidDevice(this, hidDeviceController->devicePath());

        connect(this, &SAKHidDebugPage::writeDataRequest,hidDevice, &SAKHidDevice::writeBytes);

        connect(hidDevice, &SAKHidDevice::bytesWriten,          this, &SAKHidDebugPage::bytesWritten);
        connect(hidDevice, &SAKHidDevice::bytesRead,            this, &SAKHidDebugPage::bytesRead);
        connect(hidDevice, &SAKHidDevice::messageChanged,       this, &SAKHidDebugPage::outputMessage);
        connect(hidDevice, &SAKHidDevice::deviceStatuChanged,   this, &SAKHidDebugPage::changeDeviceStatus);

        hidDevice->start();
    }    
}


void SAKHidDebugPage::refreshDevice()
{
    hidDeviceController->refresh();
}

QWidget *SAKHidDebugPage::controllerWidget()
{
    return hidDeviceController;
}
