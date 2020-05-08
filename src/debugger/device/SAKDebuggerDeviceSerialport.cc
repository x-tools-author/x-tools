/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
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
#include <QMetaEnum>
#include <QQmlEngine>
#include <QSerialPortInfo>

#include "SAKDebuggerDeviceSerialport.hh"

SAKDebuggerDeviceSerialport::SAKDebuggerDeviceSerialport(SAKDebugger *debugger, QObject *parent)
    :SAKDebuggerDevice (debugger, parent)
    ,serialport (Q_NULLPTR)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SAKDebuggerDeviceSerialport::~SAKDebuggerDeviceSerialport()
{

}

QStringList SAKDebuggerDeviceSerialport::avalidSerialports()
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    QStringList list;
    for (auto var:infos){
        list.append(var.portName());
    }

    return list;
}

QStringList SAKDebuggerDeviceSerialport::avalidBaudRates()
{
    QList<qint32> rates = QSerialPortInfo::standardBaudRates();
    QStringList list;
    for (auto var:rates){
        list.append(QString::number(var));
    }
    return list;
}

QStringList SAKDebuggerDeviceSerialport::avalidStopBits()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}

QStringList SAKDebuggerDeviceSerialport::avalidParitys()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::Parity>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}

QStringList SAKDebuggerDeviceSerialport::avalidDataBits()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}

void SAKDebuggerDeviceSerialport::run()
{
    serialport = new QSerialPort;
    while (1) {
        /// @brief “优雅地”地退出线程
        if (isInterruptionRequested()){
            break;
        }
    }

    delete serialport;
}

QStringList SAKDebuggerDeviceSerialport::serialports()
{
    return avalidSerialports();
}

QStringList SAKDebuggerDeviceSerialport::baudRates()
{
    return avalidBaudRates();
}

QStringList SAKDebuggerDeviceSerialport::stopBits()
{
    return avalidStopBits();
}

QStringList SAKDebuggerDeviceSerialport::paritys()
{
    return avalidParitys();
}

QStringList SAKDebuggerDeviceSerialport::dataBits()
{
    return avalidDataBits();
}
