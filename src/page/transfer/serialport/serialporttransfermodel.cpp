/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialporttransfermodel.h"

#include "common/xtools.h"
#include "device/serialport.h"

SerialPortTransferModel::SerialPortTransferModel(QObject *parent)
    : TransferModel(parent)
{}

SerialPortTransferModel::~SerialPortTransferModel() {}

QVariantMap SerialPortTransferModel::saveRow(const int row)
{
    if (row < 0 || row >= rowCount()) {
        return {};
    }

    SerialPortItem item;
    item.portName = data(index(row, 1), Qt::EditRole).toString();
    item.baudRate = data(index(row, 2), Qt::EditRole).toInt();
    item.dataBits = data(index(row, 3), Qt::EditRole).toInt();
    item.stopBits = data(index(row, 4), Qt::EditRole).toInt();
    item.parity = data(index(row, 5), Qt::EditRole).toInt();
    item.flowControl = data(index(row, 6), Qt::EditRole).toInt();

    QJsonObject obj = saveSerialPortItem(item);
    obj.insert("enable", data(index(row, 0), Qt::EditRole).toBool());
    obj.insert("description", data(index(row, 7), Qt::EditRole).toString());

    return obj.toVariantMap();
}

void SerialPortTransferModel::loadRow(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= rowCount()) {
        return;
    }

    bool enable = item.value("enable").toBool();
    QString description = item.value("description").toString();
    SerialPortItem serialPortItem = loadSerialPortItem(QJsonObject::fromVariantMap(item));

    setData(index(row, 0), enable, Qt::EditRole);
    setData(index(row, 1), serialPortItem.portName, Qt::EditRole);
    setData(index(row, 2), serialPortItem.baudRate, Qt::EditRole);
    setData(index(row, 3), serialPortItem.dataBits, Qt::EditRole);
    setData(index(row, 4), serialPortItem.stopBits, Qt::EditRole);
    setData(index(row, 5), serialPortItem.parity, Qt::EditRole);
    setData(index(row, 6), serialPortItem.flowControl, Qt::EditRole);
    setData(index(row, 7), description, Qt::EditRole);
}

int SerialPortTransferModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 8;
}

QVariant SerialPortTransferModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    auto item = m_transfers.at(row);
    SerialPort *serialPort = qobject_cast<SerialPort *>(item.transfer);
    if (!serialPort) {
        return QVariant();
    }

    int column = index.column();
    QVariantMap data = serialPort->save();
    SerialPortItem serialPortItem = loadSerialPortItem(QJsonObject::fromVariantMap(data));

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            int option = m_transfers.at(row).option;
            return transferTypeName(option);
        } else if (column == 1) {
            return serialPortItem.portName;
        } else if (column == 2) {
            return QString::number(serialPortItem.baudRate);
        } else if (column == 3) {
            return QString::number(serialPortItem.dataBits);
        } else if (column == 4) {
            if (serialPortItem.stopBits == QSerialPort::OneStop) {
                return QString("1");
            } else if (serialPortItem.stopBits == QSerialPort::OneAndHalfStop) {
                return QString("1.5");
            } else if (serialPortItem.stopBits == QSerialPort::TwoStop) {
                return QString("2");
            } else {
                return QString("(Unknown)");
            }
        } else if (column == 5) {
            if (serialPortItem.parity == QSerialPort::OddParity) {
                return tr("Odd");
            } else if (serialPortItem.parity == QSerialPort::EvenParity) {
                return tr("Even");
            } else if (serialPortItem.parity == QSerialPort::MarkParity) {
                return tr("Mark");
            } else if (serialPortItem.parity == QSerialPort::SpaceParity) {
                return tr("Space");
            } else {
                return tr("None");
            }
        } else if (column == 6) {
            if (serialPortItem.flowControl == QSerialPort::HardwareControl) {
                return tr("Hardware");
            } else if (serialPortItem.flowControl == QSerialPort::SoftwareControl) {
                return tr("Software");
            } else {
                return tr("None");
            }
        } else if (column == 7) {
            return item.description;
        }
    } else if (role == Qt::EditRole) {
        if (column == 0) {
            return item.isEnable;
        } else if (column == 1) {
            return serialPortItem.portName;
        } else if (column == 2) {
            return serialPortItem.baudRate;
        } else if (column == 3) {
            return serialPortItem.dataBits;
        } else if (column == 4) {
            return serialPortItem.stopBits;
        } else if (column == 5) {
            return serialPortItem.parity;
        } else if (column == 6) {
            return serialPortItem.flowControl;
        } else if (column == 7) {
            return item.description;
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (column == 7) {
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

bool SerialPortTransferModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return false;
    }

    if (role != Qt::EditRole) {
        return false;
    }

    int column = index.column();
    Item item = m_transfers[row];
    if (column == 0) {
        item.option = value.toInt();

        if (item.option == static_cast<int>(TransferType::Disabled)) {
            item.isEnable = false;
        } else {
            item.isEnable = true;
        }

        m_transfers.replace(row, item);
    } else if (column == 7) {
        item.description = value.toString();
        m_transfers.replace(row, item);
    } else {
        auto item = m_transfers.at(row);
        SerialPort *serialPort = qobject_cast<SerialPort *>(item.transfer);
        if (!serialPort) {
            return false;
        }

        QVariantMap data = serialPort->save();
        auto serialPortItem = loadSerialPortItem(QJsonObject::fromVariantMap(data));
        if (column == 1) {
            serialPortItem.portName = value.toString();
        } else if (column == 2) {
            serialPortItem.baudRate = value.toInt();
        } else if (column == 3) {
            serialPortItem.dataBits = value.toInt();
        } else if (column == 4) {
            serialPortItem.stopBits = static_cast<QSerialPort::StopBits>(value.toInt());
        } else if (column == 5) {
            serialPortItem.parity = static_cast<QSerialPort::Parity>(value.toInt());
        } else if (column == 6) {
            serialPortItem.flowControl = static_cast<QSerialPort::FlowControl>(value.toInt());
        } else {
            return false;
        }

        auto parametres = saveSerialPortItem(serialPortItem);
        serialPort->load(parametres.toVariantMap());
    }

    emit dataChanged(index, index);
    return true;
}

QVariant SerialPortTransferModel::headerData(int section,
                                             Qt::Orientation orientation,
                                             int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Transfer Option");
        } else if (section == 1) {
            return tr("Port Name");
        } else if (section == 2) {
            return tr("Baud Rate");
        } else if (section == 3) {
            return tr("Data Bits");
        } else if (section == 4) {
            return tr("Stop Bits");
        } else if (section == 5) {
            return tr("Parity");
        } else if (section == 6) {
            return tr("Flow Control");
        } else if (section == 7) {
            return tr("Description");
        }
    } else if (role == Qt::TextAlignmentRole) {
        if (section == 7) {
            return Qt::AlignLeft;
        } else {
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

Qt::ItemFlags SerialPortTransferModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

Device *SerialPortTransferModel::createTransfer()
{
    auto sp = new SerialPort{this};
    auto item = defaultSerialPortItem();
    auto parameters = saveSerialPortItem(item);
    sp->load(parameters.toVariantMap());
    return sp;
}

bool SerialPortTransferModel::isEnableRestartColumn(int column) const
{
    if ((column == 0) && (column == (columnCount() - 1))) {
        return false;
    }

    return true;
}
