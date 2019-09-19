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

#include <QApplication>
#include "SAKHidDevice.hh"
#include "SAKDebugPage.hh"

SAKHidDevice::SAKHidDevice(SAKDebugPage *debugPage, QString path, QObject *parent)
    :QThread (parent)    
    ,debugPage (debugPage)
    ,path (path)
    ,hidDevice (nullptr)
    ,readTimer (nullptr)
{
    moveToThread(this);
}

SAKHidDevice::~SAKHidDevice()
{
    if (hidDevice){
        hid_close(hidDevice);
    }

    if (readTimer){
        delete readTimer;
    }
}

void SAKHidDevice::run()
{
    hidDevice = hid_open_path(path.toLatin1().constData());
    if (!hidDevice){
        emit messageChanged(tr("无法打开设备")+QString::fromWCharArray(hid_error(hidDevice)), false);
        return;
    }

    readTimer = new QTimer;
    connect(readTimer, &QTimer::timeout, this, &SAKHidDevice::readBytes, Qt::QueuedConnection);
    readBytes();

    emit deviceStatuChanged(true);
    hid_set_nonblocking(hidDevice, 0);
    exec();
}

void SAKHidDevice::readBytes()
{        
    readTimer->stop();

    QByteArray data;
    data.resize(1024);
    int ret = hid_read(hidDevice, reinterpret_cast<unsigned char*>(data.data()), static_cast<size_t>(data.length()));
    if (ret == -1){
        emit messageChanged(tr("读取数据失败：")+QString::fromWCharArray(hid_error(hidDevice)), false);
    }else{
        data.resize(ret);
        emit bytesRead(data);
    }

    readTimer->start(debugPage->readWriteParameters().waitForReadyReadTime);
}

void SAKHidDevice::writeBytes(QByteArray data)
{    
    int ret = hid_write(hidDevice, reinterpret_cast<const unsigned char*>(data.constData()), static_cast<size_t>(data.length()));
    if (ret == -1){
        emit messageChanged(tr("发送数据失败：")+QString::fromWCharArray(hid_error(hidDevice)), false);
    }else{
        emit bytesWriten(data);
    }
}
