/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKInterface.hh"
#include "SAKSerialPortTool.hh"
#include "SAKSerialPortTransmitterTool.hh"

SAKSerialPortTransmitterTool::SAKSerialPortTransmitterTool(QObject *parent)
    : SAKTransmitterTool{"SAK.SerialPortTransmitterTool", parent}
{

}

QVariant SAKSerialPortTransmitterTool::itemContext(int index)
{
    QJsonObject obj;
    ItemContextKey ctx;
    if (index >= 0 && index < mToolVector.count()) {
        SAKSerialPortTool *tool =
            qobject_cast<SAKSerialPortTool*>(mToolVector.value(index));
        obj.insert(ctx.baudRate, tool->baudRate());
        obj.insert(ctx.dataBits, tool->dataBits());
        obj.insert(ctx.enable, tool->enable());
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
        qCInfo(mLoggingCategory) << "get default value:" << obj;
    }

    return obj;
}

void SAKSerialPortTransmitterTool::inputBytes(const QByteArray &bytes,
                                              const QVariant &context)
{
    QByteArray ba = SAKInterface::arrayToHex(bytes, ' ');
    QString hex = QString::fromLatin1(ba);
    outputMessage(QtInfoMsg, QString("%1<-%2").arg(mToolName, hex));

    mToolVectorMutex.lock();
    for (auto tool : mToolVector) {
        tool->inputBytes(bytes, context);
    }
    mToolVectorMutex.unlock();
}

int SAKSerialPortTransmitterTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 7;
}

QVariant SAKSerialPortTransmitterTool::data(const QModelIndex &index,
                                            int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    auto ret = qobject_cast<SAKSerialPortTool*>(mToolVector.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKey ctx;
    if (key == ctx.enable) {
        return ret->enable();
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
        qCWarning(mLoggingCategory) << "unknown index:" << index;
        return false;
    }

    return true;
}

bool SAKSerialPortTransmitterTool::setData(const QModelIndex &index,
                                           const QVariant &value,
                                           int role)
{
    if (role != Qt::EditRole) {
        return false;
    }

    auto ret = qobject_cast<SAKSerialPortTool*>(mToolVector.value(index.row()));
    QString key = headerData(index.column(), Qt::Horizontal).toString();
    ItemContextKey ctx;
    if (key == ctx.enable) {
        ret->setEnable(value.toBool());
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

bool SAKSerialPortTransmitterTool::insertRows(int row,
                                              int count,
                                              const QModelIndex &parent)
{
    Q_UNUSED(parent)
    for (int i = 0; i < count; i++) {
        mToolVectorMutex.lock();
        auto tool = new SAKSerialPortTool(this);
        connect(this, &SAKSerialPortTransmitterTool::bytesInputted,
                tool, &SAKSerialPortTool::inputBytes);
        connect(tool, &SAKSerialPortTool::bytesOutputted,
                this, &SAKSerialPortTransmitterTool::bytesOutputted);
        connect(tool, &SAKSerialPortTool::finished, this, [=](){
            if (this->isRunning()) {
                tool->start();
            }
        });
        connect(this, &SAKSerialPortTransmitterTool::started,
                tool, [=](){tool->start();});
        connect(this, &SAKSerialPortTransmitterTool::finished,
                tool, [=](){tool->exit();});

        mToolVector.insert(row, tool);
        mToolVectorMutex.unlock();
    }

    return true;
}

QVariant SAKSerialPortTransmitterTool::headerData(int section,
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
