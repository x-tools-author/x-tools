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
#include "SAKTcpClientDevice.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpClientDeviceController.hh"

SAKTcpClientDebugPage::SAKTcpClientDebugPage(QWidget *parent)
    :SAKDebugPage (SAKGlobal::SAKEnumDebugPageTypeTCPClient, parent)
    ,tcpClientDevice (nullptr)
    ,tcpClientDeviceController (new SAKTcpClientDeviceController)
{
    setUpController();
    setWindowTitle(tr("TCP客户端"));
}

SAKTcpClientDebugPage::~SAKTcpClientDebugPage()
{
    tcpClientDeviceController->deleteLater();

    if (tcpClientDevice){
        tcpClientDevice->terminate();
        delete tcpClientDevice;
    }
}

void SAKTcpClientDebugPage::setUiEnable(bool enable)
{
    tcpClientDeviceController->setEnabled(enable);
    refreshPushButton->setEnabled(enable);
}

void SAKTcpClientDebugPage::changeDeviceStatus(bool opened)
{
    /*
     * 设备打开失败，使能ui, 打开成功，禁止ui
     */
    setUiEnable(!opened);
    switchPushButton->setText(opened ? tr("关闭") : tr("打开"));
    if (!opened){
        if (tcpClientDevice){
            tcpClientDevice->terminate();
            delete tcpClientDevice;
            tcpClientDevice = nullptr;
        }
    }
    emit deviceStatusChanged(opened);
}

void SAKTcpClientDebugPage::openOrColoseDevice()
{
    if (tcpClientDevice){
        switchPushButton->setText(tr("打开"));
        tcpClientDevice->terminate();
        delete tcpClientDevice;
        tcpClientDevice = nullptr;

        setUiEnable(true);
        emit deviceStatusChanged(false);
    }else{
        switchPushButton->setText(tr("关闭"));
        QString localHost = tcpClientDeviceController->localHost();
        quint16 localPort = tcpClientDeviceController->localPort();
        bool customSetting = tcpClientDeviceController->enableCustomLocalSetting();
        QString targetHost = tcpClientDeviceController->serverHost();
        quint16 targetPort = tcpClientDeviceController->serverPort();

        tcpClientDevice = new SAKTcpClientDevice(localHost, localPort, customSetting, targetHost, targetPort, this);

        connect(this, &SAKTcpClientDebugPage::writeDataRequest,tcpClientDevice, &SAKTcpClientDevice::writeBytes);

        connect(tcpClientDevice, &SAKTcpClientDevice::bytesWriten,          this, &SAKTcpClientDebugPage::bytesWritten);
        connect(tcpClientDevice, &SAKTcpClientDevice::bytesRead,            this, &SAKTcpClientDebugPage::bytesRead);
        connect(tcpClientDevice, &SAKTcpClientDevice::messageChanged,       this, &SAKTcpClientDebugPage::outputMessage);
        connect(tcpClientDevice, &SAKTcpClientDevice::deviceStatuChanged,   this, &SAKTcpClientDebugPage::changeDeviceStatus);

        tcpClientDevice->start();
    }    
}


void SAKTcpClientDebugPage::refreshDevice()
{
    tcpClientDeviceController->refresh();
}

QWidget *SAKTcpClientDebugPage::controllerWidget()
{
    return tcpClientDeviceController;
}
