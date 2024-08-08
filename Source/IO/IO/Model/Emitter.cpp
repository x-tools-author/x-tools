/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Emitter.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "../../xIO.h"

namespace xTools {

Emitter::Emitter(QObject *parent)
    : AbstractModel{parent}
{}

void Emitter::inputBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes)
}

int Emitter::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mItems.count();
}

int Emitter::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mTableColumnCount;
}

QVariant Emitter::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        int column = index.column();
        const EmitterItem &item = mItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool Emitter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);

    return true;
}

bool Emitter::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Data ctx;
    EmitterItem item;
    item.data = ctx;
    item.elapsedTime = 0;
    for (int i = 0; i < count; i++) {
        mItems.insert(row, item);
    }

    return true;
}

bool Emitter::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    mItems.remove(row, count);
    return true;
}

QVariant Emitter::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role);
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return mDataKeys.itemEnable;
        case 1:
            return mDataKeys.itemDescription;
        case 2:
            return mDataKeys.itemTextFormat;
        case 3:
            return mDataKeys.itemEscapeCharacter;
        case 4:
            return mDataKeys.itemInterval;
        case 5:
            return mDataKeys.itemPrefix;
        case 6:
            return mDataKeys.itemSuffix;
        case 7:
            return mDataKeys.itemCrcEnable;
        case 8:
            return mDataKeys.itemCrcBigEndian;
        case 9:
            return mDataKeys.itemCrcAlgorithm;
        case 10:
            return mDataKeys.itemCrcStartIndex;
        case 11:
            return mDataKeys.itemCrcEndIndex;
        case 12:
            return mDataKeys.itemText;
        default:
            return "";
        }
    }

    return QVariant("");
}

void Emitter::run()
{
    mEmittingTimer = new QTimer();
    mEmittingTimer->setInterval(mScanInterval);
    mEmittingTimer->setSingleShot(true);
    connect(mEmittingTimer, &QTimer::timeout, mEmittingTimer, [=]() { try2emit(); });
    mEmittingTimer->start();

    exec();

    if (mEmittingTimer) {
        mEmittingTimer->stop();
        mEmittingTimer->deleteLater();
        mEmittingTimer = nullptr;
    }
}

void Emitter::try2emit()
{
    mItemsMutex.lock();
    for (auto &item : mItems) {
        int elapsedTime = item.elapsedTime += mScanInterval;
        if (elapsedTime > item.data.itemInterval && item.data.itemEnable) {
            item.elapsedTime = 0;
            const auto bytes = itemBytes(item.data);
            emit outputBytes(bytes);
        }
    }
    mItemsMutex.unlock();
    mEmittingTimer->start();
}

QByteArray Emitter::itemBytes(const Emitter::Data &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    auto escapeCharacter = static_cast<xIO::EscapeCharacter>(item.itemEscapeCharacter);
    text = xIO::cookedEscapeCharacter(text, escapeCharacter);
    bytes = xIO::string2bytes(text, static_cast<xIO::TextFormat>(item.itemTextFormat));
    QByteArray prefix = xIO::cookedAffixes(static_cast<xIO::Affixes>(item.itemPrefix));
    QByteArray suffix = xIO::cookedAffixes(static_cast<xIO::Affixes>(item.itemSuffix));

    bytes.prepend(prefix);
    if (item.itemCrcEnable) {
        auto algorithm = static_cast<xIO::CrcAlgorithm>(item.itemCrcAlgorithm);
        QByteArray crcBytes = xIO::calculateCrc(bytes,
                                                algorithm,
                                                item.itemCrcStartIndex,
                                                item.itemCrcEndIndex,
                                                item.itemCrcBigEndian);
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

QVariant Emitter::columnDisplayRoleData(const Emitter::EmitterItem &item, int column) const
{
    return QVariant("Error");
}

QString Emitter::itemEnable()
{
    return mDataKeys.itemEnable;
}

QString Emitter::itemDescription()
{
    return mDataKeys.itemDescription;
}

QString Emitter::itemTextFormat()
{
    return mDataKeys.itemTextFormat;
}

QString Emitter::itemEscapeCharacter()
{
    return mDataKeys.itemEscapeCharacter;
}

QString Emitter::itemInterval()
{
    return mDataKeys.itemInterval;
}

QString Emitter::itemPrefix()
{
    return mDataKeys.itemPrefix;
}

QString Emitter::itemSuffix()
{
    return mDataKeys.itemSuffix;
}

QString Emitter::itemCrcEnable()
{
    return mDataKeys.itemCrcEnable;
}

QString Emitter::itemCrcBigEndian()
{
    return mDataKeys.itemCrcBigEndian;
}

QString Emitter::itemCrcAlgorithm()
{
    return mDataKeys.itemCrcAlgorithm;
}

QString Emitter::itemCrcStartIndex()
{
    return mDataKeys.itemCrcStartIndex;
}

QString Emitter::itemCrcEndIndex()
{
    return mDataKeys.itemCrcEndIndex;
}

QString Emitter::itemText()
{
    return mDataKeys.itemText;
}

} // namespace xTools
