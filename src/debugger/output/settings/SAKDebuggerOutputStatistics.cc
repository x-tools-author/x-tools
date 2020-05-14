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
#include "SAKDebugger.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerOutputStatistics.hh"

SAKDebuggerOutputStatistics::SAKDebuggerOutputStatistics(QObject *parent)
    :QObject (parent)
{
    dataContext.rxBytes  = 0;
    dataContext.txBytes  = 0;
    dataContext.rxFrames = 0;
    dataContext.txFrames = 0;
    dataContext.rxBytesPerSecond = 0;
    dataContext.txBytesPerSecond = 0;

    speedCalculationTimer.setInterval(1*1000);
    connect(&speedCalculationTimer, &QTimer::timeout, this, &SAKDebuggerOutputStatistics::updateSpeed);
    speedCalculationTimer.start();
}

void SAKDebuggerOutputStatistics::clearRxStatistics()
{
    dataContext.rxBytes = 0;
    dataContext.rxFrames = 0;

    emit rxBytesChanged();
    emit rxFramesChanged();
}

void SAKDebuggerOutputStatistics::clearTxStatistics()
{
    dataContext.txBytes = 0;
    dataContext.txFrames = 0;

    txBytesChanged();
    txFramesChanged();
}

void SAKDebuggerOutputStatistics::setDevice(SAKDebuggerDevice *device)
{
    /// @brief 关联读写数据
    connect(device, &SAKDebuggerDevice::bytesRead, this, &SAKDebuggerOutputStatistics::bytesRead);
    connect(device, &SAKDebuggerDevice::bytesWritten, this, &SAKDebuggerOutputStatistics::bytesWritten);
}

void SAKDebuggerOutputStatistics::updateSpeed()
{
    auto cal = [](QString &str, quint64 &bytes){
        if (bytes < 1024){
            str = QString("%1 B/s").arg(bytes);
        }else if(bytes < (1024*1024)){
            str = QString("%1 KB/s").arg(bytes/1024);
        }else{
           str = QString("%1 MB/s").arg(bytes/(1024*1024));
        }

        bytes = 0;
    };

    cal(_rxSpeed, dataContext.rxBytesPerSecond);
    cal(_txSpeed, dataContext.txBytesPerSecond);
    emit rxSpeedChanged();
    emit txSpeedChanged();
}

void SAKDebuggerOutputStatistics::bytesRead(QByteArray bytes)
{
    dataContext.rxFrames += 1;
    dataContext.rxBytes += static_cast<quint64>(bytes.length());
    dataContext.rxBytesPerSecond += static_cast<quint64>(bytes.length());

    emit rxFramesChanged();
    emit rxBytesChanged();
}

void SAKDebuggerOutputStatistics::bytesWritten(QByteArray bytes)
{
    dataContext.txFrames += 1;
    dataContext.txBytes += static_cast<quint64>(bytes.length());
    dataContext.txBytesPerSecond += static_cast<quint64>(bytes.length());

    emit txFramesChanged();
    emit txBytesChanged();
}

QString SAKDebuggerOutputStatistics::rxSpeed()
{
    return _rxSpeed;
}

QString SAKDebuggerOutputStatistics::txSpeed()
{
    return _txSpeed;
}

QString SAKDebuggerOutputStatistics::rxFrames()
{
    return QString::number(dataContext.rxFrames);
}

QString SAKDebuggerOutputStatistics::rxBytes()
{
    return QString::number(dataContext.rxBytes);
}

QString SAKDebuggerOutputStatistics::txFrames()
{
    return QString::number(dataContext.txFrames);
}

QString SAKDebuggerOutputStatistics::txBytes()
{
    return QString::number(dataContext.txBytes);
}
