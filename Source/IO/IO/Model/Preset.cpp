/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Preset.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

namespace xTools {

Preset::Preset(QObject *parent)
    : AbstractModel{parent}
{

}

int Preset::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

int Preset::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant Preset::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count()) {
        qWarning() << "Invalid index row: " << index.row();
        return QVariant();
    }

    xIO::TextItemContext textContext = m_items.at(index.row()).textContext;
    QJsonObject json = xIO::saveTextItemContext(textContext);

    if (index.column() == 0 && role == Qt::DisplayRole) {
        return m_items.at(index.row()).itemDescription;
    } else if (index.column() == 0 && role == Qt::EditRole) {
        return m_items.at(index.row()).itemDescription;
    } else if (index.column() == 1 && role == Qt::DisplayRole) {
        return xIO::textItemContext2string(textContext);
    } else if (index.column() == 1 && role == Qt::EditRole) {
        return QVariant::fromValue(json);
    }

    return QVariant();
}

bool Preset::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= m_items.count()) {
        qWarning() << "Invalid index row: " << index.row();
        return false;
    }

    if (index.column() == 0 && role == Qt::EditRole) {
        m_items[index.row()].itemDescription = value.toString();
    } else if (index.column() == 1 && role == Qt::EditRole) {
        auto textContext = xIO::loadTextItemContext(value.toJsonObject());
        m_items[index.row()].textContext = textContext;
    } else {
        return false;
    }

    return true;
}

bool Preset::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    xIO::TextItemContext textContext = xIO::defaultTextItemContext();
    for (int i = 0; i < count; i++) {
        Item item{tr("Demo") + QString::number(rowCount()), textContext};
        m_items.insert(row, item);
    }

    qInfo() << "Insert rows: " << row << ", " << count;

    return true;
}

bool Preset::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    m_items.remove(row, count);
    return true;
}

QVariant Preset::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Description");
        } else if (section == 1) {
            return tr("Data");
        }
    }

    return QVariant();
}

void Preset::inputBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes)
}

void Preset::run()
{
    exec();
}

} // namespace xTools
