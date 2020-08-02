/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKSerialPortDeviceController.hh"

SAKSerialPortDevice::SAKSerialPortDevice(SAKSerialPortDebugPage *debugPage, QObject *parent)
    :SAKDebugPageDevice(parent)
    ,serialPort(Q_NULLPTR)
    ,debugPage(debugPage)
{

}

SAKSerialPortDevice::~SAKSerialPortDevice()
{

}

void SAKSerialPortDevice::run()
{
    QEventLoop eventLoop;
    SAKSerialPortDeviceController *controller = debugPage->controllerInstance();
    name = controller->name();
    baudRate = controller->baudRate();
    dataBits = controller->dataBits();
    stopBits = controller->stopBits();
    parity = controller->parity();
    flowControl = controller->flowControl();

    serialPort = new QSerialPort;
    serialPort->setPortName(name);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(parity);
    serialPort->setFlowControl(flowControl);

    if (serialPort->open(QSerialPort::ReadWrite)){
        emit deviceStateChanged(true);
        while (true){
            /// @brief 优雅地退出线程
            if (isInterruptionRequested()){
                break;
            }

            /// @brief 读取数据
            QByteArray bytes = serialPort->readAll();
            if (bytes.length()){
                emit bytesRead(bytes);
            }

            /// @brief 写入数据
            while (true){
                QByteArray var = takeWaitingForWrittingBytes();
                if (var.length()){
                    qint64 ret = serialPort->write(var);
                    if (ret == -1){
                        emit messageChanged(tr("Send data error: ") + serialPort->errorString(), false);
                    }else{
                        emit bytesWritten(var);
                    }
                }else{
                    break;
                }
            }

            /// @brief 处理线程事件
            eventLoop.processEvents();

            /// @brief 线程睡眠
            mThreadMutex.lock();
            mThreadWaitCondition.wait(&mThreadMutex, SAK_DEVICE_THREAD_SLEEP_INTERVAL);
            mThreadMutex.unlock();
        }

        /// @brief 关闭清理串口
        serialPort->clear();
        serialPort->close();
        delete serialPort;
        serialPort = Q_NULLPTR;
        emit deviceStateChanged(false);
    }else{        
        emit deviceStateChanged(false);
        emit messageChanged(tr("Open com error") + serialPort->errorString(), false);
        return;
    }
}
