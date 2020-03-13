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
#include "SAKTcpServerDevice.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKTcpServerDeviceController.hh"

SAKTcpServerDebugPage::SAKTcpServerDebugPage(QWidget *parent)
    :SAKDebugPage (SAKGlobal::SAKEnumDebugPageTypeTCPServer, parent)
    ,tcpServerDevice (nullptr)
    ,tcpServerDeviceController (new SAKTcpServerDeviceController)
{
    setUpController();
    setWindowTitle(tr("TCP服务器"));
}

SAKTcpServerDebugPage::~SAKTcpServerDebugPage()
{
    tcpServerDeviceController->deleteLater();

    if (tcpServerDevice){
        tcpServerDevice->terminate();
        delete tcpServerDevice;
    }
}

void SAKTcpServerDebugPage::setUiEnable(bool enable)
{
    tcpServerDeviceController->setUiEnable(enable);
    refreshPushButton->setEnabled(enable);
}

void SAKTcpServerDebugPage::changeDeviceStatus(bool opened)
{
    /*
     * 设备打开失败，使能ui, 打开成功，禁止ui
     */
    setUiEnable(!opened);
    switchPushButton->setText(opened ? tr("关闭") : tr("打开"));
    if (!opened){
        if (tcpServerDevice){
            tcpServerDevice->terminate();
            delete tcpServerDevice;
            tcpServerDevice = nullptr;
        }
    }
    emit deviceStatusChanged(opened);
}

void SAKTcpServerDebugPage::tryWrite(QByteArray data)
{
    emit writeBytesRequest(data, tcpServerDeviceController->currentClientHost(), tcpServerDeviceController->currentClientPort());
}

void SAKTcpServerDebugPage::afterBytesRead(QByteArray data, QString host, quint16 port)
{
    if (    (tcpServerDeviceController->currentClientHost().compare(host) == 0)
         && (tcpServerDeviceController->currentClientPort() == port)){
        emit bytesRead(data);
    }    
}

void SAKTcpServerDebugPage::afterBytesWritten(QByteArray data, QString host, quint16 port)
{
    if ((tcpServerDeviceController->currentClientHost().compare(host) == 0) && (tcpServerDeviceController->currentClientPort() == port)){
        emit bytesWritten(data);
    }
}

void SAKTcpServerDebugPage::openOrColoseDevice()
{
    if (tcpServerDevice){
        switchPushButton->setText(tr("打开"));
        tcpServerDevice->terminate();
        delete tcpServerDevice;
        tcpServerDevice = nullptr;

        setUiEnable(true);
        emit deviceStatusChanged(false);
    }else{
        switchPushButton->setText(tr("关闭"));
        QString serverHost = tcpServerDeviceController->serverHost();
        quint16 serverPort = tcpServerDeviceController->serverPort();

        tcpServerDevice = new SAKTcpServerDevice(serverHost, serverPort, this);

        connect(this, &SAKTcpServerDebugPage::writeDataRequest,  this, &SAKTcpServerDebugPage::tryWrite);
        connect(this, &SAKTcpServerDebugPage::writeBytesRequest, tcpServerDevice, &SAKTcpServerDevice::writeBytes);

        connect(tcpServerDevice, &SAKTcpServerDevice::newClientConnected, tcpServerDeviceController, &SAKTcpServerDeviceController::addClient);

        connect(tcpServerDevice, &SAKTcpServerDevice::bytesWritten,         this, &SAKTcpServerDebugPage::afterBytesWritten);
        connect(tcpServerDevice, &SAKTcpServerDevice::bytesRead,            this, &SAKTcpServerDebugPage::afterBytesRead);
        connect(tcpServerDevice, &SAKTcpServerDevice::messageChanged,       this, &SAKTcpServerDebugPage::outputMessage);
        connect(tcpServerDevice, &SAKTcpServerDevice::deviceStatuChanged,   this, &SAKTcpServerDebugPage::changeDeviceStatus);

        tcpServerDevice->start();
    }    
}


void SAKTcpServerDebugPage::refreshDevice()
{
    tcpServerDeviceController->refresh();
}

QWidget *SAKTcpServerDebugPage::controllerWidget()
{
    return tcpServerDeviceController;
}
