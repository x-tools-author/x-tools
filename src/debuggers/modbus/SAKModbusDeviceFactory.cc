/******************************************************************************
 * Copyright 2022 Qter(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDebug>
#include <QtEndian>
#include <QMetaEnum>
#include <QJsonArray>
#include <QModbusDevice>
#include <QCoreApplication>
#include <QModbusTcpClient>
#include <QModbusTcpServer>
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
#include <QModbusRtuSerialSlave>
#include <QModbusRtuSerialMaster>
#else
#include <QModbusRtuSerialClient>
#include <QModbusRtuSerialServer>
#endif

#include "SAKModbusDeviceFactory.hh"

SAKModbusDeviceFactory *SAKModbusDeviceFactory::m_instance = Q_NULLPTR;
QMutex SAKModbusDeviceFactory::m_instanceMutext;
SAKModbusDeviceFactory::SAKModbusDeviceFactory(QObject *parent)
    : QObject{parent}
    , mLoggingCategory("SAK.Modbus.ModbusDeviceFactory")
{

}

SAKModbusDeviceFactory::~SAKModbusDeviceFactory()
{
    m_instanceMutext.lock();
    m_instance = Q_NULLPTR;
    m_instanceMutext.unlock();
}

SAKModbusDeviceFactory *SAKModbusDeviceFactory::instance()
{
    m_instanceMutext.lock();
    if (!m_instance) {
        m_instance = new SAKModbusDeviceFactory(qApp);
    }
    m_instanceMutext.unlock();

    Q_ASSERT_X(m_instance, __FUNCTION__,
               "Can not allocate memory for QMSModbusFactory!");
    return m_instance;
}

QModbusDevice *SAKModbusDeviceFactory::createRtuSerialDevice(
    int deviceType, const QString &portName, int parity, int baudRate,
    int dataBits, int stopBits)
{
    QModbusDevice *device = Q_NULLPTR;
    if (deviceType == ModbusDeviceRtuSerialClient) {
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        device = new QModbusRtuSerialMaster(this);
#else
        device = new QModbusRtuSerialClient(this);
#endif
    } else if (deviceType == ModbusDeviceRtuSerialServer) {
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        device = new QModbusRtuSerialSlave(this);
#else
        device = new QModbusRtuSerialServer(this);
#endif
    }

    if (device) {
        device->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                                       portName);
        device->setConnectionParameter(QModbusDevice::SerialParityParameter,
                                       parity);
        device->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                                       baudRate);
        device->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                                       dataBits);
        device->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                                       stopBits);
        return device;
    }

    Q_ASSERT_X(device, __FUNCTION__, "Unknow device type.");
    return Q_NULLPTR;
}

QModbusDevice *SAKModbusDeviceFactory::createTcpDevice(int deviceType,
                                                       QString address,
                                                       int port)
{
    QModbusDevice *device = Q_NULLPTR;
    if (deviceType == ModbusDeviceTcpClient) {
        device = new QModbusTcpClient(this);
    } else if (deviceType == ModbusDeviceTcpServer) {
        device = new QModbusTcpServer(this);
    }

    if (device) {
        device->setConnectionParameter(QModbusDevice::NetworkAddressParameter,
                                       address);
        device->setConnectionParameter(QModbusDevice::NetworkPortParameter,
                                       port);
        return device;
    }

    return Q_NULLPTR;
}

bool SAKModbusDeviceFactory::openDevice(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusDevice")) {
            return device->connectDevice();
        }
    }
    return false;
}


bool SAKModbusDeviceFactory::closeDevice(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusDevice")) {
            device->disconnectDevice();
            device->deleteLater();
            return true;
        }
    }
    return false;
}

bool SAKModbusDeviceFactory::setClientParameters(QModbusDevice *device,
                                                 int timeout,
                                                 int numberOfRetries)
{
    if (device) {
        if (device->inherits("QModbusClient")) {
            QModbusClient *modbusClient = qobject_cast<QModbusClient*>(device);
            modbusClient->setTimeout(timeout);
            modbusClient->setNumberOfRetries(numberOfRetries);
            return true;
        }
    }

    return false;
}

bool SAKModbusDeviceFactory::setServerParameters(QModbusDevice *device,
                                                 int option,
                                                 QVariant value)
{
    if (device) {
        if (device->inherits("QModbusServer")) {
            QModbusServer *modbusServer = qobject_cast<QModbusServer*>(device);
            modbusServer->setValue(option, value);
            return true;
        }
    }

    return false;
}

bool SAKModbusDeviceFactory::isFinishedReply(QVariant reply)
{
    if (reply.canConvert<QObject*>()) {
        QObject *obj = reply.value<QObject*>();
        if (obj->inherits("QModbusReply")) {
            QModbusReply *modbusReply = qobject_cast<QModbusReply*>(obj);
            return modbusReply->isFinished();
        }
    }

    return false;
}

QString SAKModbusDeviceFactory::modbuseDeviceErrorString(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusDevice")) {
            return device->errorString();
        } else {
            return QString("Invalid device object!");
        }
    }

    return QString("Invalid modbus device!");
}

QString SAKModbusDeviceFactory::modbuseReplyErrorString(QVariant reply)
{
    if (reply.canConvert<QObject*>()) {
        QObject *obj = reply.value<QObject*>();
        if (obj->inherits("QModbusReply")) {
            QModbusReply *modbusReply = qobject_cast<QModbusReply*>(obj);
            return modbusReply->errorString();
        } else {
            return QString("Invalid reply object!");
        }
    }

    return QString("Invalid modbus reply!");
}

QJsonArray SAKModbusDeviceFactory::modbusReplyResult(QModbusReply *reply)
{
    if (!reply) {
        return QJsonArray();
    }

    if (!reply->inherits("QModbusReply")) {
        return QJsonArray();
    }

    QJsonArray resultJsonArray;
    if (reply->type() == QModbusReply::Common) {
        QModbusDataUnit dataUnit = reply->result();
        for (uint i = 0; i < dataUnit.valueCount(); i++) {
            quint16 rawData = dataUnit.value(i);
            int registerType = dataUnit.registerType();
            int coils = QModbusDataUnit::Coils;
            int discreteInputs = QModbusDataUnit::DiscreteInputs;
            if ((registerType == coils) || (registerType == discreteInputs)) {
                if (rawData != 0) {
                    rawData = 1;
                }
            }
            resultJsonArray.append(int(rawData));
        }

        return resultJsonArray;
    }

    QModbusResponse rawResult = reply->rawResult();
    QByteArray data = rawResult.data();
    for (int i = 0; i < data.length(); i++) {
        resultJsonArray.append(data.at(i));
    }

    return resultJsonArray;
}

QByteArray SAKModbusDeviceFactory::modbusReplyRawData(QModbusReply *reply)
{
    if (reply) {
        return reply->rawResult().data();
    }

    return QByteArray();
}

QModbusReply *SAKModbusDeviceFactory::sendReadRequest(
    QModbusDevice *device, int registerType, int startAddress,
    int size, int serverAddress)
{
    if (device && isClient(device)) {
        qCInfo(mLoggingCategory) << "register-type:" << registerType
                                 << " start-address:" << startAddress
                                 << " register-number:" << size
                                 << " serverAddress:" << serverAddress;
        QModbusClient *client = qobject_cast<QModbusClient*>(device);
        QModbusDataUnit::RegisterType cookedRegisterType
            = static_cast<QModbusDataUnit::RegisterType>(registerType);
        QModbusDataUnit dataUnit(cookedRegisterType, startAddress, size);
        QModbusReply *reply = client->sendReadRequest(dataUnit, serverAddress);
        return reply;
    }

    return Q_NULLPTR;
}

QModbusReply *SAKModbusDeviceFactory::sendWriteRequest(
    QModbusDevice *device, int registerType, int startAddress,
    QJsonArray values, int serverAddress)
{
    if (device && isClient(device)) {
        QModbusDataUnit::RegisterType cookedRegisterType
            = static_cast<QModbusDataUnit::RegisterType>(registerType);
        QVector<quint16> registerValues;
        for (int i = 0; i < values.count(); i++) {
            registerValues.append(values.at(i).toInt());
        }

        QModbusDataUnit dataUnit(cookedRegisterType,
                                 startAddress, registerValues);
        if (dataUnit.isValid()) {
            qCInfo(mLoggingCategory) << "register-type:" << registerType
                                     << " start-address:" << startAddress
                                     << " serverAddress:" << serverAddress
                                     << " values:" << values;

            QModbusClient *client = qobject_cast<QModbusClient*>(device);
            QModbusReply *reply = client->sendWriteRequest(dataUnit,
                                                           serverAddress);
            return reply;
        } else {
            qCWarning(mLoggingCategory) << "Unvalid data unit!";
        }
    }

    return Q_NULLPTR;
}

QModbusReply *SAKModbusDeviceFactory::sendRawRequest(QModbusDevice *device,
                                                     int serverAddress,
                                                     int functionCode,
                                                     const QByteArray &data)
{

    auto fc = static_cast<QModbusPdu::FunctionCode>(functionCode);
    QModbusRequest request(fc, data);
    if (device && isClient(device)) {
        QModbusClient *client = qobject_cast<QModbusClient*>(device);
        return client->sendRawRequest(request, serverAddress);
    }

    return Q_NULLPTR;
}

void SAKModbusDeviceFactory::deleteModbusReply(QVariant modbusReply)
{
    if (modbusReply.canConvert<QModbusReply*>()) {
        modbusReply.value<QModbusReply*>()->deleteLater();
    }
}

bool SAKModbusDeviceFactory::isClient(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusClient")) {
            return true;
        }
    }

    return false;
}

bool SAKModbusDeviceFactory::isRawResult(QVariant reply)
{
    if (reply.canConvert<QObject*>()) {
        QObject *obj = reply.value<QObject*>();
        if (obj->inherits("QModbusReply")) {
            QModbusReply *re = reply.value<QModbusReply*>();
            return re->type() == QModbusReply::Raw;
        }
    }

    return false;
}

bool SAKModbusDeviceFactory::isServer(QModbusDevice *device)
{
    if (device) {
        if (device->inherits("QModbusServer")) {
            return true;
        }
    }

    return false;
}

bool SAKModbusDeviceFactory::isValidModbusReply(QVariant reply)
{
    if (reply.canConvert<QModbusReply*>()) {
        QModbusReply *modbusReply = reply.value<QModbusReply*>();
        if (modbusReply) {
            qCWarning(mLoggingCategory) << "QModbusReply is null!";
            return false;
        } else if (modbusReply->isFinished()) {
            qCWarning(mLoggingCategory) << "QModbusReply is finished!";
            return false;
        } else {
            return true;
        }
    }

    return false;
}

bool SAKModbusDeviceFactory::resetServerMap(QVariant server)
{
    if (server.canConvert<QModbusServer*>()) {
        QVector<quint16> values(UINT16_MAX + 1, 0);
        QModbusDataUnit dataUnit(QModbusDataUnit::Coils, 0, values);

        QModbusDataUnitMap map;
        map.insert(QModbusDataUnit::Coils, dataUnit);
        map.insert(QModbusDataUnit::DiscreteInputs, dataUnit);
        map.insert(QModbusDataUnit::HoldingRegisters, dataUnit);
        map.insert(QModbusDataUnit::InputRegisters, dataUnit);

        QModbusServer *modbusServer = server.value<QModbusServer*>();
        return modbusServer->setMap(map);
    }

    return false;
}

bool SAKModbusDeviceFactory::setServerData(QModbusDevice *server,
                                           QModbusDataUnit::RegisterType table,
                                           quint16 address, quint16 data)
{
    if (server) {
        if (server->inherits("QModbusServer")) {
            QModbusServer *modbusServer = qobject_cast<QModbusServer*>(server);
            return modbusServer->setData(table, address, data);
        }
    }

    return false;
}

QJsonArray SAKModbusDeviceFactory::serverValues(QModbusServer *server,
                                                int registerType,
                                                int address, int size)
{
    QJsonArray values;
    if (server) {
        QModbusDataUnit::RegisterType table =
                static_cast<QModbusDataUnit::RegisterType>(registerType);
        for (int i = address; i < size; i++) {
            quint16 value;
            if (server->data(table, i, &value)) {
                values.append(value);
            } else {
                qCWarning(mLoggingCategory) << "Parameters error!";
                break;
            }
        }
    } else {
        qCWarning(mLoggingCategory) << "Can not get values from null object!";
    }

    return values;
}
