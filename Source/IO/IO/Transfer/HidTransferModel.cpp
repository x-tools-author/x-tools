/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "HidTransferModel.h"

#include "../../xIO.h"
#include "../Communication/Hid.h"

namespace xTools {

HidTransferModel::HidTransferModel(QObject *parent)
    : AbstractTransferModel(parent)
{}

HidTransferModel::~HidTransferModel() {}

int HidTransferModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 8;
}

QVariant HidTransferModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    auto item = m_transfers.at(row);
    Hid *Hid = qobject_cast<Hid *>(item.transfer);
    if (!Hid) {
        return QVariant();
    }

    int column = index.column();
    QVariantMap data = Hid->parameters();
    xIO::HidItem HidItem = xIO::loadHidItem(QJsonObject::fromVariantMap(data));

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            int option = m_transfers.at(row).option;
            return xIO::transferTypeName(option);
        } else if (column == 1) {
            return HidItem.portName;
        } else if (column == 2) {
            return QString::number(HidItem.baudRate);
        } else if (column == 3) {
            return QString::number(HidItem.dataBits);
        } else if (column == 4) {
            if (HidItem.stopBits == QHid::OneStop) {
                return tr("1");
            } else if (HidItem.stopBits == QHid::OneAndHalfStop) {
                return tr("1.5");
            } else if (HidItem.stopBits == QHid::TwoStop) {
                return tr("2");
            } else {
                return ("(Unknown)");
            }
        } else if (column == 5) {
            if (HidItem.parity == QHid::OddParity) {
                return tr("Odd");
            } else if (HidItem.parity == QHid::EvenParity) {
                return tr("Even");
            } else if (HidItem.parity == QHid::MarkParity) {
                return tr("Mark");
            } else if (HidItem.parity == QHid::SpaceParity) {
                return tr("Space");
            } else {
                return tr("None");
            }
        } else if (column == 6) {
            if (HidItem.flowControl == QHid::HardwareControl) {
                return tr("Hardware");
            } else if (HidItem.flowControl == QHid::SoftwareControl) {
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
            return HidItem.portName;
        } else if (column == 2) {
            return HidItem.baudRate;
        } else if (column == 3) {
            return HidItem.dataBits;
        } else if (column == 4) {
            return HidItem.stopBits;
        } else if (column == 5) {
            return HidItem.parity;
        } else if (column == 6) {
            return HidItem.flowControl;
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

bool HidTransferModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
        Hid *Hid = qobject_cast<Hid *>(item.transfer);
        if (!Hid) {
            return false;
        }

        QVariantMap data = Hid->parameters();
        auto HidItem = xIO::loadHidItem(QJsonObject::fromVariantMap(data));
        if (column == 1) {
            HidItem.portName = value.toString();
        } else if (column == 2) {
            HidItem.baudRate = value.toInt();
        } else if (column == 3) {
            HidItem.dataBits = value.toInt();
        } else if (column == 4) {
            HidItem.stopBits = static_cast<QHid::StopBits>(value.toInt());
        } else if (column == 5) {
            HidItem.parity = static_cast<QHid::Parity>(value.toInt());
        } else if (column == 6) {
            HidItem.flowControl = static_cast<QHid::FlowControl>(value.toInt());
        } else {
            return false;
        }

        auto parametres = xIO::saveHidItem(HidItem);
        Hid->setParameters(parametres.toVariantMap());
    }

    emit dataChanged(index, index);
    return true;
}

QVariant HidTransferModel::headerData(int section,
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

Qt::ItemFlags HidTransferModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

Communication *HidTransferModel::createTransfer()
{
    auto sp = new Hid{this};
    auto item = xIO::defaultHidItem();
    auto parametres = xIO::saveHidItem(item);
    sp->setParameters(parametres.toVariantMap());
    return sp;
}

bool HidTransferModel::isEnableRestartingColumn(int column) const
{
    if ((column == 0) && (column == (columnCount() - 1))) {
        return false;
    }

    return true;
}

} // namespace xTools
