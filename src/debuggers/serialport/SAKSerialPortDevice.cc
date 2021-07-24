/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QElapsedTimer>

#include "SAKSerialPortDevice.hh"
#include "SAKSerialPortDebugger.hh"
#include "SAKSerialPortController.hh"

SAKSerialPortDevice::SAKSerialPortDevice(QSettings *settings, const QString &settingsGroup, QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, parent)
    ,mSerialPort(Q_NULLPTR)
{

}

void SAKSerialPortDevice::setParametersCtx(
        SAKCommonDataStructure::SAKStructSerialPortParametersContext ctx
        )
{
    mParametersCtxMutex.lock();
    mParametersCtx = ctx;
    mParametersCtxMutex.unlock();
}

bool SAKSerialPortDevice::initialize()
{
    auto parasCtx = parametersCtx();
    mSerialPort = new QSerialPort;
    mSerialPort->setBaudRate(parasCtx.baudRate);
    mSerialPort->setDataBits(parasCtx.dataBits);
    mSerialPort->setFlowControl(parasCtx.flowControl);
    mSerialPort->setParity(parasCtx.parity);
    mSerialPort->setPortName(parasCtx.portName);
    mSerialPort->setStopBits(parasCtx.stopBits);
#ifdef QT_DEBUG
    qDebug() << "baudRate" << parasCtx.baudRate
             << "dataBits" << parasCtx.dataBits
             << "flowControl" << parasCtx.flowControl
             << "parity" << parasCtx.parity
             << "portName" << parasCtx.portName
             << "stopBits" << parasCtx.stopBits
             << "intervalNs" << parasCtx.intervalNs;
#endif

    if (mSerialPort->open(QSerialPort::ReadWrite)) {
         connect(mSerialPort, &QSerialPort::readyRead,
                 this, [=](){
             emit readyRead(SAKDeviceProtectedSignal());
         });
         return true;
    } else {
         emit errorOccurred(mSerialPort->errorString());
         qWarning() << "Can not open device: " << mSerialPort->errorString();
         mSerialPort->deleteLater();
         return false;
    }
}

QByteArray SAKSerialPortDevice::read()
{
    auto parasCtx = this->parametersCtx();
    QElapsedTimer elapsedTimer;
    QByteArray bytes;
    while (true) {
        auto ret = mSerialPort->readAll();
        if (ret.length()) {
            bytes.append(ret);
            elapsedTimer.restart();
        } else {
            auto ret = elapsedTimer.nsecsElapsed();
            if (parasCtx.intervalNs <= ret) {
                break;
            }
        }
    }

    if (bytes.length()) {
        return bytes;
    } else {
        return QByteArray();
    }
}

QByteArray SAKSerialPortDevice::write(const QByteArray &bytes)
{
    if (mSerialPort->write(bytes) > 0) {
        return bytes;
    } else {
        emit errorOccurred(mSerialPort->errorString());
        qWarning() << "Write bytes failed:" << mSerialPort->errorString();
        return QByteArray();
    }
}

void SAKSerialPortDevice::uninitialize()
{
    mSerialPort->close();
    delete mSerialPort;
    mSerialPort = Q_NULLPTR;
}

SAKCommonDataStructure::SAKStructSerialPortParametersContext
        SAKSerialPortDevice::parametersCtx()
{
    mParametersCtxMutex.lock();
    auto parasCtx = mParametersCtx;
    mParametersCtxMutex.unlock();
    return parasCtx;
}
