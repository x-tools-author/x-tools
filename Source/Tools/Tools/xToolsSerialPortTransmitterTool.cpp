/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSerialPortTransmitterTool.h"

#include <QDebug>

#include "xToolsCompatibility.h"
#include "xToolsSerialPortTool.h"

xToolsSerialPortTransmitterTool::xToolsSerialPortTransmitterTool(QObject *parent)
    : xToolsTransmitterTool{parent}
{}

QString xToolsSerialPortTransmitterTool::cookHeaderString(const QString &str)
{
    ItemContextKey keys;
    if (str == keys.enable) {
        return tr("enable");
    } else if (str == keys.portName) {
        return tr("portName");
    } else if (str == keys.baudRate) {
        return tr("baudRate");
    } else if (str == keys.parity) {
        return tr("parity");
    } else if (str == keys.stopBits) {
        return tr("stopBits");
    } else if (str == keys.dataBits) {
        return tr("dataBits");
    } else if (str == keys.flowControl) {
        return tr("flowControl");
    } else if (str == keys.description) {
        return tr("description");
    }

    return "--";
}

QVariant xToolsSerialPortTransmitterTool::itemContext(int index)
{
    QJsonObject obj;
    ItemContextKey ctx;
    if (index >= 0 && index < m_tools.count()) {
        xToolsSerialPortTool *tool = qobject_cast<xToolsSerialPortTool *>(m_tools.value(index));
        obj.insert(ctx.baudRate, tool->baudRate());
        obj.insert(ctx.dataBits, tool->dataBits());
        obj.insert(ctx.enable, tool->isEnable());
        obj.insert(ctx.flowControl, tool->flowControl());
        obj.insert(ctx.parity, tool->parity());
        obj.insert(ctx.portName, tool->portName());
        obj.insert(ctx.stopBits, tool->stopBits());
    } else {
        obj.insert(ctx.baudRate, 9600);
        obj.insert(ctx.dataBits, QSerialPort::Data8);
        obj.insert(ctx.enable, true);
        obj.insert(ctx.flowControl, QSerialPort::NoFlowControl);
        obj.insert(ctx.parity, QSerialPort::NoParity);
        obj.insert(ctx.portName, "");
        obj.insert(ctx.stopBits, QSerialPort::OneStop);
        qInfo() << "get default value:" << obj;
    }

    return obj;
}

void xToolsSerialPortTransmitterTool::inputBytes(const QByteArray &bytes)
{
    QByteArray ba = xToolsByteArrayToHex(bytes, ' ');
    QString hex = QString::fromLatin1(ba);

    m_toolsMutex.lock();
    for (auto tool : m_tools) {
        tool->inputBytes(bytes);
    }
    m_toolsMutex.unlock();
}

int xToolsSerialPortTransmitterTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant xToolsSerialPortTransmitterTool::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    
    auto ret = qobject_cast<xToolsSerialPortTool *>(m_tools.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKey ctx;
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

    return true;
}

bool xToolsSerialPortTransmitterTool::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    
    auto ret = qobject_cast<xToolsSerialPortTool *>(m_tools.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKey ctx;
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

    return true;
}

QVariant xToolsSerialPortTransmitterTool::headerData(int section,
                                                  Qt::Orientation orientation,
                                                  int role) const
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

xToolsCommunicationTool *xToolsSerialPortTransmitterTool::createTool()
{
    xToolsSerialPortTool *tool = new xToolsSerialPortTool();
    return tool;
}
