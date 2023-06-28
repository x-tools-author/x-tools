/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUS_HH
#define SAKMODBUS_HH

#include <QThread>
#include <QModbusReply>
#include <QModbusServer>
#include <QModbusDevice>
#include <QLoggingCategory>

class SAKModbus : public QThread
{
    Q_OBJECT
public:
    SAKModbus(QObject *parent = nullptr);
    ~SAKModbus();

    QModbusDevice *createRtuSerialDevice(int deviceType,
                                         const QString &portName,
                                         int parity,
                                         int baudRate,
                                         int dataBits,
                                         int stopBits);
    QModbusDevice *createTcpDevice(int deviceType, QString address, int port);
    bool isClient(QModbusDevice *device);
    bool isServer(QModbusDevice *device);
    bool resetServerMap(QModbusDevice *server);
    bool setClientParameters(QModbusDevice *device,
                             int timeout,
                             int numberOfRetries);
    bool setServerParameters(QModbusDevice *device,
                             int option,
                             QVariant value);
    bool openDevice(QModbusDevice *device);
    QString modbuseDeviceErrorString(QModbusDevice *device);
    bool setServerData(QModbusDevice *server,
                       QModbusDataUnit::RegisterType table,
                       quint16 address,
                       quint16 data);
    QModbusReply *sendReadRequest(QModbusDevice *device,
                                  int registerType,
                                  int startAddress,
                                  int size,
                                  int serverAddress);
    QJsonArray serverValues(QModbusServer *server,
                            int registerType,
                            int address, int size);
    QModbusReply *sendWriteRequest(QModbusDevice *device,
                                   int registerType,
                                   int startAddress,
                                   QJsonArray values,
                                   int serverAddress);
    bool isValidModbusReply(QVariant reply);
    QJsonArray modbusReplyResult(QModbusReply *reply);
    QModbusReply *sendRawRequest(QModbusDevice *device,
                                 int serverAddress,
                                 int functionCode,
                                 const QByteArray &data);

private:

};

#endif // SAKMODBUS_HH
