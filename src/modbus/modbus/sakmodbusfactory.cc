/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QCoreApplication>
#include <QModbusTcpClient>
#include <QModbusTcpServer>
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>

#include "sakmodbusfactory.hh"

SAKModbusFactory::SAKModbusFactory(QObject *parent)
    : QObject(parent) {

}

SAKModbusFactory::~SAKModbusFactory() {

}

SAKModbusFactory *SAKModbusFactory::Instance() {
    static SAKModbusFactory f(qApp);
    return &f;
}

const QString SAKModbusFactory::TypeName(int type) {
    if (type == kModbusRtuSerialClient) {
        return tr("RTU Client");
    } else if (type == kModbusRtuSerialServer) {
        return tr("RTU Server");
    } else if (type == kModbusTcpClient) {
        return tr("TCP Client");
    } else if (type == kModbusTcpServer) {
        return tr("TCP Server");
    }

    Q_ASSERT_X(false, __FUNCTION__, "Unknown modebus device type");
    qCWarning(kLoggingCategory) << "Unknown modebus device type";

    return "Unknown";
}

QModbusDevice *SAKModbusFactory::CreateDevice(int type) {
    if (type == kModbusRtuSerialClient) {
        qCInfo(kLoggingCategory) << "create rtu serial master";
        return new QModbusRtuSerialClient();
    } else if (type == kModbusRtuSerialServer) {
        qCInfo(kLoggingCategory) << "create rtu serial slave";
        return new QModbusRtuSerialServer();
    } else if (type == kModbusTcpClient) {
        qCInfo(kLoggingCategory) << "create rtu tcp client";
        return new QModbusTcpClient();
    } else if (type == kModbusTcpServer) {
        qCInfo(kLoggingCategory) << "create rtu tcp server";
        return new QModbusTcpServer();
    }

    Q_ASSERT_X(false, __FUNCTION__, "Unknown modebus device type");
    qCWarning(kLoggingCategory) << "Unknown modebus device type";

    return Q_NULLPTR;
}

bool SAKModbusFactory::IsTcpDevice(QVariant modbus_device) {
    QModbusDevice *device = modbus_device.value<QModbusDevice*>();
    if (device) {
        if (qobject_cast<QModbusTcpClient*>(device)) {
            return true;
        } else if (qobject_cast<QModbusTcpClient*>(device)) {
            return true;
        }
    }

    return false;
}

bool SAKModbusFactory::IsRtuSerialDevice(QVariant modbus_device) {
    QModbusDevice *device = modbus_device.value<QModbusDevice*>();
    if (device) {
        if (qobject_cast<QModbusRtuSerialClient*>(device)) {
            return true;
        } else if (qobject_cast<QModbusRtuSerialServer*>(device)) {
            return true;
        }
    }

    return false;
}
