/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPortTransmitter.h"

#include <QDebug>

#include "../Communication/Communication.h"
#include "../Communication/SerialPort.h"
#include "xToolsCompatibility.h"

namespace xTools {

SerialPortTransmitter::SerialPortTransmitter(QObject *parent)
    : AbstractTransmitter{parent}
{}

void SerialPortTransmitter::inputBytes(const QByteArray &bytes)
{
    QByteArray ba = xToolsByteArrayToHex(bytes, ' ');
    QString hex = QString::fromLatin1(ba);

    m_toolsMutex.lock();
    for (auto tool : m_tools) {
        tool->inputBytes(bytes);
    }
    m_toolsMutex.unlock();
}

int SerialPortTransmitter::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant SerialPortTransmitter::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto ret = qobject_cast<SerialPort *>(m_tools.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKey ctx;
#if 0
    if (key == ctx.enable) {
        return ret->isEnable();
    } else if (key == ctx.baudRate) {
        return ret->baudRate();
    } else if (key == ctx.dataBits) {
        return ret->dataBits();
    } else if (key == ctx.flowControl) {
        return ret->flowControl();
    } else if (key == ctx.parity) {
        return ret->parity();
    } else if (key == ctx.portName) {
        return ret->portName();
    } else if (key == ctx.stopBits) {
        return ret->stopBits();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow index");
        qWarning() << "unknown index:" << index;
        return false;
    }
#endif

    return true;
}

bool SerialPortTransmitter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    auto ret = qobject_cast<SerialPort *>(m_tools.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKey ctx;
#if 0
    if (key == ctx.enable) {
        ret->setIsEnable(value.toBool());
    } else if (key == ctx.baudRate) {
        ret->setBaudRate(value.toInt());
    } else if (key == ctx.dataBits) {
        ret->setDataBits(value.toInt());
    } else if (key == ctx.flowControl) {
        ret->setFlowControl(value.toInt());
    } else if (key == ctx.parity) {
        ret->setParity(value.toInt());
    } else if (key == ctx.portName) {
        ret->setPortName(value.toString());
    } else if (key == ctx.stopBits) {
        ret->setStopBits(value.toInt());
    } else {
        qDebug() << "unknow key:" << key;
        return false;
    }
#endif

    return true;
}

QVariant SerialPortTransmitter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    ItemContextKey ctx;
    if (section == 0) {
        return ctx.enable;
    } else if (section == 1) {
        return ctx.portName;
    } else if (section == 2) {
        return ctx.baudRate;
    } else if (section == 3) {
        return ctx.dataBits;
    } else if (section == 4) {
        return ctx.stopBits;
    } else if (section == 5) {
        return ctx.parity;
    } else if (section == 6) {
        return ctx.flowControl;
    } else {
        return QVariant("Unknown");
    }
}

Communication *SerialPortTransmitter::createTool()
{
    SerialPort *tool = new SerialPort();
    return tool;
}

} // namespace xTools
