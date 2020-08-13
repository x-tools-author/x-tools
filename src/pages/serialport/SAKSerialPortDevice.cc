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
    ,mSerialPort(Q_NULLPTR)
    ,mDebugPage(debugPage)
{

}

SAKSerialPortDevice::~SAKSerialPortDevice()
{

}

void SAKSerialPortDevice::run()
{
    QEventLoop eventLoop;
    SAKSerialPortDeviceController *controller = mDebugPage->controllerInstance();
    mName = controller->name();
    mBaudRate = controller->baudRate();
    mDataBits = controller->dataBits();
    mStopBits = controller->stopBits();
    mParity = controller->parity();
    mFlowControl = controller->flowControl();

    mSerialPort = new QSerialPort;
    mSerialPort->setPortName(mName);
    mSerialPort->setBaudRate(mBaudRate);
    mSerialPort->setDataBits(mDataBits);
    mSerialPort->setStopBits(mStopBits);
    mSerialPort->setParity(mParity);
    mSerialPort->setFlowControl(mFlowControl);

    if (mSerialPort->open(QSerialPort::ReadWrite)){
        emit deviceStateChanged(true);
        while (true){
            if (isInterruptionRequested()){
                break;
            }

            // The operation must be done, if not, data can not be read.
            eventLoop.processEvents();

            // Read data
            QByteArray bytes = mSerialPort->readAll();
            if (bytes.length()){
                emit bytesRead(bytes);
            }

            // Write data
            while (true){
                QByteArray var = takeWaitingForWrittingBytes();
                if (var.length()){
                    qint64 ret = mSerialPort->write(var);
                    if (ret == -1){
                        emit messageChanged(tr("Send data error: ") + mSerialPort->errorString(), false);
                    }else{
                        emit bytesWritten(var);
                    }
                }else{
                    break;
                }
            }

            // Do something make cpu happy
            if (isInterruptionRequested()){
                break;
            }else{
                mThreadMutex.lock();
                mThreadWaitCondition.wait(&mThreadMutex, SAK_DEVICE_THREAD_SLEEP_INTERVAL);
                mThreadMutex.unlock();
            }
        }

        // Free memery
        mSerialPort->clear();
        mSerialPort->close();
        delete mSerialPort;
        mSerialPort = Q_NULLPTR;
        emit deviceStateChanged(false);
    }else{        
        emit deviceStateChanged(false);
        emit messageChanged(tr("Open com error") + mSerialPort->errorString(), false);
        return;
    }
}
