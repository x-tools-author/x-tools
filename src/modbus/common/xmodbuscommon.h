/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QJsonObject>
#include <QSerialPort>

namespace xModbus {

enum class XModbusType { RtuClient, RtuServer, TcpClient, TcpServer };
void setupModebusDeviceType(QComboBox *cb);
void setupRtuNames(QComboBox *cb);
void setupRtuDataBits(QComboBox *cb);
void setupRtuParity(QComboBox *cb);
void setupRtuStopBits(QComboBox *cb);
void setupRtuBaudRate(QComboBox *cb);

struct DeviceConnectionParameterKeys
{
    const QString deviceName{"deviceName"};
    const QString deviceType{"deviceType"};
    const QString portName{"portName"};
    const QString dataBits{"dataBits"};
    const QString parity{"parity"};
    const QString stopBits{"stopBits"};
    const QString baudRate{"baudRate"};
    const QString tcpAddress{"tcpAddress"};
    const QString tcpPort{"tcpPort"};
    const QString numberOfRetries{"numberOfRetries"};
    const QString timeout{"timeout"};
    const QString serverAddress{"serverAddress"};
    const QString listenOnlyMode{"listenOnlyMode"};
};

struct DeviceConnectionParameters
{
    QString deviceName;
    int deviceType;
    QString portName;
    int dataBits;
    int parity;
    int stopBits;
    int baudRate;
    QString tcpAddress;
    int tcpPort;
    int numberOfRetries;
    int timeout;
    int serverAddress;
    bool listenOnlyMode;
};

QJsonObject deviceConnectionParameters2Json(const DeviceConnectionParameters &params);
DeviceConnectionParameters json2DeviceConnectionParameters(const QJsonObject &params);

} // namespace xModbus