/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SerialPortTransferModel.h"

#include "../../xIO.h"
#include "../Communication/SerialPort.h"

namespace xTools {

SerialPortTransferModel::SerialPortTransferModel(QObject *parent)
    : AbstractTransferModel(parent)
{}

SerialPortTransferModel::~SerialPortTransferModel() {}

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
    QVariantMap data = serialPort->parameters();
    xIO::SerialPortItem serialPortItem = xIO::loadSerialPortItem(QJsonObject::fromVariantMap(data));

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            int option = m_transfers.at(row).option;
            return xIO::transferTypeName(option);
        } else if (column == 1) {
            return serialPortItem.portName;
        } else if (column == 2) {
            return QString::number(serialPortItem.baudRate);
        } else if (column == 3) {
            return QString::number(serialPortItem.dataBits);
        } else if (column == 4) {
            if (serialPortItem.stopBits == QSerialPort::OneStop) {
                return tr("1");
            } else if (serialPortItem.stopBits == QSerialPort::OneAndHalfStop) {
                return tr("1.5");
            } else if (serialPortItem.stopBits == QSerialPort::TwoStop) {
                return tr("2");
            } else {
                return ("(Unknown)");
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
            return item.transfer->isEnable();
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

        if (item.option == static_cast<int>(xIO::TransferType::Disabled)) {
            item.transfer->setIsEnable(false);
        } else {
            item.transfer->setIsEnable(true);
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

        QVariantMap data = serialPort->parameters();
        auto serialPortItem = xIO::loadSerialPortItem(QJsonObject::fromVariantMap(data));
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

        auto parametres = xIO::saveSerialPortItem(serialPortItem);
        serialPort->setParameters(parametres.toVariantMap());
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
            return tr("Poet Name");
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

Communication *SerialPortTransferModel::createTransfer()
{
    auto sp = new SerialPort{this};
    auto item = xIO::defaultSerialPortItem();
    auto parametres = xIO::saveSerialPortItem(item);
    sp->setParameters(parametres.toVariantMap());
    return sp;
}

bool SerialPortTransferModel::isEnableRestartingColumn(int column) const
{
    if ((column == 0) && (column == (columnCount() - 1))) {
        return false;
    }

    return true;
}

} // namespace xTools
