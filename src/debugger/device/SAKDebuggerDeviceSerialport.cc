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

#include "SAKDebugger.hh"
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
        if (str.contains("Unknown")){
            continue;
        }
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
        if (str.contains("Unknown")){
            continue;
        }
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
        if (str.contains("Unknown")){
            continue;
        }
        list.append(str);
    }

    return list;
}

void SAKDebuggerDeviceSerialport::open()
{
    if (serialport->isOpen()){
        openRequest = false;
        clostRequest = true;
    }else{
        openRequest = true;
        clostRequest = false;
    }

    deviceWaitCondition.wakeAll();
}

void SAKDebuggerDeviceSerialport::refresh()
{
    _serialports = avalidSerialports();
    emit serialportsChanged();
}

void SAKDebuggerDeviceSerialport::run()
{
    serialport = new QSerialPort;
    while (1) {
        /// @brief “优雅地”地退出线程
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 打开串口
        if (openRequest){
            openRequest = false;
            if (serialport->isOpen()){
                serialport->close();
            }

            if (openActually(serialport)){
                emit deviceStateChanged(true);
            }else{
                emit deviceStateChanged(false);
            }
        }

        /// @brief 关闭串口
        if (clostRequest){
            clostRequest = false;
            serialport->close();
            debugger->setMessage(tr("串口已关闭"));
            emit deviceStateChanged(false);
        }

        /// @brief 读取数据的前提是设备已打开
        if (serialport->isOpen()){
            /// @brief 日了哈士奇，没有这行居然读不到数据（qt5.12.8）
            serialport->waitForReadyRead(1);
            QByteArray bytes = serialport->readAll();
            if (bytes.length()){
                emit bytesRead(bytes);
            }
        }

        /// @brief 发送数据
        while(1) {
            if (serialport->isOpen()){
                QByteArray array = takeArrayFromDataList();
                if (array.isEmpty()){
                    break;
                }else {
                    qint64 ret = serialport->write(array);
                    serialport->waitForBytesWritten();
                    if (ret == -1){
                        debugger->setMessage(tr("发送数据失败:") + serialport->errorString());
                    }else{
                        emit bytesWritten(array);
                    }
                }
            }else{
                break;
            }
        }

        /// @brief 等待，有数据需要发送是线程会被唤醒
        deviceMutex.lock();
        deviceWaitCondition.wait(&deviceMutex, readInterval);
        deviceMutex.unlock();
    }

    if (serialport->isOpen()){
        serialport->close();
    }
    delete serialport;
}

bool SAKDebuggerDeviceSerialport::openActually(QSerialPort *port)
{
    if (port){
        QMetaEnum parityEnum = QMetaEnum::fromType<QSerialPort::Parity>();
        QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(parityEnum.keyToValue(parameterContext.parity.toLatin1().data()));

        QMetaEnum dataBitsEnum = QMetaEnum::fromType<QSerialPort::DataBits>();
        QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(dataBitsEnum.keyToValue(parameterContext.dataBits.toLatin1().data()));

        QMetaEnum stopBitsEnum = QMetaEnum::fromType<QSerialPort::StopBits>();
        QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(stopBitsEnum.keyToValue(parameterContext.stopBits.toLatin1().data()));

        port->setParity(parity);
        port->setBaudRate(parameterContext.baudRate.toInt());
        port->setDataBits(dataBits);
        port->setStopBits(stopBits);
        port->setPortName(parameterContext.portName);

        qDebug() << serialport->portName() << " "
                 << serialport->baudRate() << " "
                 << serialport->dataBits() << " "
                 << serialport->stopBits() << " "
                 << serialport->parity() << " ";
        if (port->open(QSerialPort::ReadWrite)){
            debugger->setMessage(tr("打开串口成功"));
            return true;
        }else{
            debugger->setMessage(tr("打开串口失败:") + port->errorString(), true);
            return false;
        }
    }

    Q_ASSERT_X(false, __FUNCTION__, "Oh, a null pointer");
    return false;
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

QString SAKDebuggerDeviceSerialport::paraPortName()
{
    return parameterContext.portName;
}

void SAKDebuggerDeviceSerialport::setParaPortName(QString name)
{
    parameterContext.portName = name;
    emit paraPortNameChanged();
}

QString SAKDebuggerDeviceSerialport::paraBaudRate()
{
    return parameterContext.baudRate;
}

void SAKDebuggerDeviceSerialport::setParaBaudRate(QString br)
{
    parameterContext.baudRate = br;
    emit paraBaudRateChanged();
}

QString SAKDebuggerDeviceSerialport::paraDataBits()
{
    return parameterContext.dataBits;
}

void SAKDebuggerDeviceSerialport::setParaDataBits(QString db)
{
    parameterContext.dataBits = db;
}

QString SAKDebuggerDeviceSerialport::paraStopBits()
{
    return parameterContext.stopBits;
}

void SAKDebuggerDeviceSerialport::setParaStopBits(QString sb)
{
    parameterContext.stopBits = sb;
    emit paraStopBitsChanged();
}

QString SAKDebuggerDeviceSerialport::paraParity()
{
    return parameterContext.parity;
}

void SAKDebuggerDeviceSerialport::setParaParity(QString p)
{
    parameterContext.parity = p;
    emit paraParityChanged();
}
