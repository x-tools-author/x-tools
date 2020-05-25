/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
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
#include <QApplication>
#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugPage.hh"

SAKSerialPortDevice::SAKSerialPortDevice(const QString name,
                                         const qint32 baudRate,
                                         const QSerialPort::DataBits dataBits,
                                         const QSerialPort::StopBits stopBits,
                                         const QSerialPort::Parity parity,
                                         const QSerialPort::FlowControl flowControl,
                                         SAKSerialPortDebugPage *debugPage,
                                         QObject *parent)
    :QThread(parent)
    ,_name(name)
    ,_baudRate(baudRate)
    ,_dataBits(dataBits)
    ,_stopBits(stopBits)
    ,_parity(parity)
    ,_flowControl(flowControl)
    ,debugPage(debugPage)
{

}

SAKSerialPortDevice::~SAKSerialPortDevice()
{

}

void SAKSerialPortDevice::writeBytes(QByteArray data)
{
    waitingForWrittenBytesList.append(data);
}

void SAKSerialPortDevice::wakeMe()
{
    threadWaitCondition.wakeAll();
}

void SAKSerialPortDevice::run()
{
    serialPort = new QSerialPort;
    serialPort->setPortName(_name);
    serialPort->setBaudRate(_baudRate);
    serialPort->setDataBits(_dataBits);
    serialPort->setStopBits(_stopBits);
    serialPort->setParity(_parity);
    serialPort->setFlowControl(_flowControl);

    if (serialPort->open(QSerialPort::ReadWrite)){
        emit deviceStatuChanged(true);
        while (1){
            /// @brief 优雅地退出线程
            if (isInterruptionRequested()){
                break;
            }

            /// @brief 读取数据
            serialPort->waitForReadyRead(debugPage->readWriteParameters().waitForReadyReadTime);
            QByteArray bytes = serialPort->readAll();
            if (bytes.length()){
                emit bytesRead(bytes);
            }

            /// @brief 写入数据
            while (waitingForWrittenBytesList.length()){
                QByteArray var = waitingForWrittenBytesList.takeFirst();
                qint64 ret = serialPort->write(var);
                serialPort->waitForBytesWritten(debugPage->readWriteParameters().waitForBytesWrittenTime);
                if (ret == -1){
                    emit messageChanged(tr("串口发送数据失败：") + serialPort->errorString(), false);
                }else{
                    emit bytesWriten(var);
                }
            }

            threadMutex.lock();
            threadWaitCondition.wait(&threadMutex, 25);
            threadMutex.unlock();
        }

        /// @brief 关闭清理串口
        serialPort->clear();
        serialPort->close();
        delete serialPort;
        serialPort = Q_NULLPTR;
    }else{        
        emit deviceStatuChanged(false);
        emit messageChanged(tr("串口打开失败：") + serialPort->errorString(), false);
        return;
    }
}
