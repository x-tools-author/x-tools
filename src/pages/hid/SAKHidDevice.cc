/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QEventLoop>

#include <QApplication>
#include "SAKHidDevice.hh"
#include "SAKHidDebugPage.hh"
#include "SAKHidDeviceController.hh"

SAKHidDevice::SAKHidDevice(SAKHidDebugPage *debugPage, QObject *parent)
    :SAKDevice(parent)
    ,debugPage (debugPage)
    ,hidDevice (nullptr)
{

}

SAKHidDevice::~SAKHidDevice()
{

}

void SAKHidDevice::run()
{
    QEventLoop eventLoop;
    SAKHidDeviceController *deviceController = debugPage->controllerInstance();
    path = deviceController->devicePath();
    hid_init();
    hidDevice = hid_open_path(path.toLatin1().constData());
    if (!hidDevice){
        emit deviceStateChanged(false);
        emit messageChanged(tr("无法打开设备")+QString::fromWCharArray(hid_error(hidDevice)), false);
        return;
    }

    /// @brief 读取数据不阻塞
    int ret = hid_set_nonblocking(hidDevice, 1);
    if (ret < 0){
        emit deviceStateChanged(false);
        emit messageChanged(tr("读取方式设置失败：")+QString::fromWCharArray(hid_error(hidDevice)), false);
    }

    emit deviceStateChanged(true);
    while (true) {
        /// @brief 响应中中断
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 读取数据
        QByteArray data;
        data.resize(50);
        data.fill('\0');
#if 1
        int ret = hid_read(hidDevice, reinterpret_cast<unsigned char*>(data.data()), static_cast<size_t>(data.length()));
#else
        int ret = hid_get_feature_report(hidDevice, reinterpret_cast<unsigned char*>(data.data()), static_cast<size_t>(data.length()));
#endif
        if(ret == 0){
            QString temp(data);
            qDebug() << __FUNCTION__ << temp.length();
            emit bytesRead(data);
        }

        /// @brief 发送数据
        while (true) {
            QByteArray bytes = takeWaitingForWrittingBytes();
            if (bytes.length()){
                int ret = hid_write(hidDevice, reinterpret_cast<const unsigned char*>(bytes.constData()), static_cast<size_t>(bytes.length()));
                if (ret == -1){
                    emit messageChanged(tr("发送数据失败：")+QString::fromWCharArray(hid_error(hidDevice)), false);
                }else{
                    emit bytesWritten(bytes);
                }
            }else{
                break;
            }
        }

        /// @brief 处理事件
        eventLoop.processEvents();

        /// @brief 线程睡眠
        threadMutex.lock();
        threadWaitCondition.wait(&threadMutex, debugPage->readWriteParameters().runIntervalTime);
        threadMutex.unlock();
    }

    hid_exit();
    emit deviceStateChanged(false);
}
