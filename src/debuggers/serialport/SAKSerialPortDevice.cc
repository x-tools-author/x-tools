/****************************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
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

SAKSerialPortDevice::SAKSerialPortDevice(QSettings *settings, const QString &settingsGroup, QWidget *uiParent, QObject *parent)
    :SAKDebuggerDevice(settings, settingsGroup, uiParent, parent)
    ,mSerialPort(Q_NULLPTR)
{

}

bool SAKSerialPortDevice::initialize()
{
    auto parasCtx = parametersContext().value<SAKSerialPortParametersContext>();
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
             << "intervalNs" << parasCtx.frameIntervel;
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

SAKDebuggerDevice::ReadContextVector SAKSerialPortDevice::read()
{
    // Calculate the max frame interval.
    auto parasCtx = parametersContext().value<SAKSerialPortParametersContext>();
    const int defauleFrameInterval = 4;
    int frameIntervel = parasCtx.frameIntervel;
    qint64 frameIntervelNs = 0;
    qint64 consumeNsPerBit = (1000*1000*1000)/parasCtx.baudRate;
    if (frameIntervel < defauleFrameInterval) {
        frameIntervelNs = defauleFrameInterval*consumeNsPerBit;
    } else {
        frameIntervelNs = frameIntervel*consumeNsPerBit;
    }

    QElapsedTimer elapsedTimer;
    QByteArray bytes;
    while (true) {
        if (!mSerialPort) {
            break;
        }

        auto ret = mSerialPort->readAll();
        if (ret.length()) {
            bytes.append(ret);
            elapsedTimer.restart();
        } else {
            auto ret = elapsedTimer.nsecsElapsed();
            if (frameIntervelNs <= ret) {
                break;
            }
        }
    }
    ReadContextVector contexts;

    if (bytes.length()) {
        if (mSerialPort) {
            ReadContext ctx;
            ctx.bytes = bytes;
            ctx.flag = mSerialPort->portName();
            return contexts << ctx;
        }

    }

    return contexts;
}

SAKDebuggerDevice::WriteContext SAKSerialPortDevice::write(const QByteArray &bytes)
{
    WriteContext context;
    context.flag = mSerialPort->portName();
    if (mSerialPort->write(bytes) > 0) {
        context.bytes = bytes;
    } else {
        emit errorOccurred(mSerialPort->errorString());
        qWarning() << "Write bytes failed:" << mSerialPort->errorString();
    }
    return context;
}

void SAKSerialPortDevice::uninitialize()
{
    mSerialPort->close();
    delete mSerialPort;
    mSerialPort = Q_NULLPTR;
}
