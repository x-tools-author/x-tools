/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Emitter.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QVariant>

namespace xTools {

Emitter::Emitter(QObject *parent)
    : AbstractModel{parent}
{}

QVariantMap Emitter::saveItem(const int row) const
{
    if (row < 0 || row >= m_items.count()) {
        qWarning() << "Invalid index row: " << row;
        return QVariantMap();
    }

    QVariant var = m_tableModel->data(m_tableModel->index(row, 0), Qt::DisplayRole);
    QString description = var.toString();

    var = m_tableModel->data(m_tableModel->index(row, 1), Qt::EditRole);
    int interval = var.toInt();

    var = m_tableModel->data(m_tableModel->index(row, 2), Qt::EditRole);
    QJsonObject item = var.toJsonObject();

    QVariantMap map = item.toVariantMap();
    map.insert("description", description);
    map.insert("interval", interval);
    return map;
}

void Emitter::loadItem(const int row, const QVariantMap &item)
{
    if (row < 0 || row >= m_items.count()) {
        qWarning() << "Invalid index row: " << row;
        return;
    }

    QString description = item.value("description").toString();
    m_tableModel->setData(m_tableModel->index(row, 0), description, Qt::EditRole);

    int interval = item.value("interval").toInt();
    m_tableModel->setData(m_tableModel->index(row, 1), interval, Qt::EditRole);

    QJsonObject json = QJsonObject::fromVariantMap(item);
    m_tableModel->setData(m_tableModel->index(row, 2), json, Qt::EditRole);
}

void Emitter::inputBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes)
}

void Emitter::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() {
        trye2Emit();
        timer->start();
    });

#if 1
    timer->start();
#endif
    exec();

    timer->stop();
    timer->deleteLater();
}

int Emitter::rowCount(const QModelIndex &parent) const
{
    m_itemsMutex.lock();
    int count = m_items.count();
    m_itemsMutex.unlock();
    return count;
}

int Emitter::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant Emitter::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_items.count()) {
        return QVariant();
    }

    m_itemsMutex.lock();
    const Item item = m_items.at(row);
    m_itemsMutex.unlock();

    int column = index.column();
    if ((column == 0) && (role == Qt::DisplayRole)) {
        return item.description;
    } else if ((column == 1) && (role == Qt::DisplayRole)) {
        return item.interval;
    } else if ((column == 2) && (role == Qt::DisplayRole)) {
        return xIO::textItem2string(item.textContext);
    } else if ((column == 0) && (role == Qt::EditRole)) {
        return item.description;
    } else if ((column == 1) && (role == Qt::EditRole)) {
        return item.interval;
    } else if ((column == 2) && (role == Qt::EditRole)) {
        return xIO::saveTextItem(item.textContext);
    }

    return QVariant();
}

bool Emitter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    if (row < 0 || row >= m_items.count()) {
        return false;
    }

    m_itemsMutex.lock();
    bool result = true;
    Item &item = m_items[row];

    int column = index.column();
    if ((column == 0) && (role == Qt::EditRole)) {
        item.description = value.toString();
    } else if ((column == 1) && (role == Qt::EditRole)) {
        item.interval = value.toInt();
        item.interval = qMax(100, item.interval);
    } else if ((column == 2) && (role == Qt::EditRole)) {
        item.textContext = xIO::loadTextItem(value.toJsonObject());
    } else {
        result = false;
    }

    m_itemsMutex.unlock();
    return result;
}

bool Emitter::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    xIO::TextItem textContext = xIO::defaultTextItem();
    for (int i = 0; i < count; i++) {
        Item item{tr("Demo") + QString::number(rowCount(QModelIndex())), 1000, textContext};
        m_items.insert(row, item);
    }

    return true;
}

bool Emitter::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    m_items.remove(row, count);
    return true;
}

QVariant Emitter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Description");
        } else if (section == 1) {
            return tr("Interval");
        } else if (section == 2) {
            return tr("Data");
        }
    }

    return QVariant();
}

void Emitter::trye2Emit()
{
    m_itemsMutex.lock();
    for (auto &item : m_items) {
        item.elapsedTime += 10;
        if (item.elapsedTime >= item.interval) {
            item.elapsedTime = 0;
            QByteArray bytes = xIO::textItem2array(item.textContext);
            emit outputBytes(bytes);
        }
    }
    m_itemsMutex.unlock();
}

} // namespace xTools
