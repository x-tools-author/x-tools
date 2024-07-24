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

QVariant Emitter::itemContext(int index)
{
    QJsonObject ctx;
    mItemsMutex.lock();
    if (index >= 0 && index < mItems.count()) {
        auto item = mItems.at(index);
        ctx.insert(itemEnable(), item.data.itemEnable);
        ctx.insert(itemDescription(), item.data.itemDescription);
        ctx.insert(itemTextFormat(), item.data.itemTextFormat);
        ctx.insert(itemEscapeCharacter(), item.data.itemEscapeCharacter);
        ctx.insert(itemInterval(), item.data.itemInterval);
        ctx.insert(itemPrefix(), item.data.itemPrefix);
        ctx.insert(itemSuffix(), item.data.itemSuffix);
        ctx.insert(itemCrcEnable(), item.data.itemCrcEnable);
        ctx.insert(itemCrcBigEndian(), item.data.itemCrcBigEndian);
        ctx.insert(itemCrcAlgorithm(), item.data.itemCrcAlgorithm);
        ctx.insert(itemCrcStartIndex(), item.data.itemCrcStartIndex);
        ctx.insert(itemCrcEndIndex(), item.data.itemCrcEndIndex);
        ctx.insert(itemText(), item.data.itemText);
    } else {
        ctx.insert(itemEnable(), true);
        ctx.insert(itemDescription(), "Demo");
        ctx.insert(itemTextFormat(), static_cast<int>(xIO::TextFormat::Ascii));
        ctx.insert(itemEscapeCharacter(), static_cast<int>(xIO::EscapeCharacter::None));
        ctx.insert(itemInterval(), 1000);
        ctx.insert(itemPrefix(), static_cast<int>(xIO::Affixes::None));
        ctx.insert(itemSuffix(), static_cast<int>(xIO::Affixes::None));
        ctx.insert(itemCrcEnable(), false);
        ctx.insert(itemCrcBigEndian(), false);
        ctx.insert(itemCrcAlgorithm(), static_cast<int>(xIO::CrcAlgorithm::CRC_8));
        ctx.insert(itemCrcStartIndex(), 0);
        ctx.insert(itemCrcEndIndex(), 0);
        ctx.insert(itemText(), "This is a demo.");
    }
    mItemsMutex.unlock();

    return ctx;
}

QString Emitter::cookHeaderString(const QString &str)
{
    DataKeys keys;
    if (str == keys.itemEnable) {
        return tr("Enable");
    } else if (str == keys.itemDescription) {
        return tr("Description");
    } else if (str == keys.itemTextFormat) {
        return tr("Format");
    } else if (str == keys.itemEscapeCharacter) {
        return tr("Escape Character");
    } else if (str == keys.itemPrefix) {
        return tr("Prefix");
    } else if (str == keys.itemSuffix) {
        return tr("Suffix");
    } else if (str == keys.itemInterval) {
        return tr("Interval");
    } else if (str == keys.itemCrcEnable) {
        return tr("Append CRC");
    } else if (str == keys.itemCrcBigEndian) {
        return tr("Big Endian");
    } else if (str == keys.itemCrcAlgorithm) {
        return tr("Algorithm");
    } else if (str == keys.itemCrcStartIndex) {
        return tr("Start Index");
    } else if (str == keys.itemCrcEndIndex) {
        return tr("End Index");
    } else if (str == keys.itemText) {
        return tr("Data");
    }

    return "--";
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
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        auto item = mItems.at(row);
        int column = index.column();
        if (column >= 0 && column < headers().count()) {
            auto dataKey = headers().at(column);
            if (dataKey == mDataKeys.itemEnable) {
                item.data.itemEnable = value.toBool();
            } else if (dataKey == mDataKeys.itemDescription) {
                item.data.itemDescription = value.toString();
            } else if (dataKey == mDataKeys.itemTextFormat) {
                item.data.itemTextFormat = value.toInt();
            } else if (dataKey == mDataKeys.itemEscapeCharacter) {
                item.data.itemEscapeCharacter = value.toInt();
            } else if (dataKey == mDataKeys.itemInterval) {
                item.data.itemInterval = value.toInt();
            } else if (dataKey == mDataKeys.itemPrefix) {
                item.data.itemPrefix = value.toInt();
            } else if (dataKey == mDataKeys.itemSuffix) {
                item.data.itemSuffix = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcEnable) {
                item.data.itemCrcEnable = value.toBool();
            } else if (dataKey == mDataKeys.itemCrcBigEndian) {
                item.data.itemCrcBigEndian = value.toBool();
            } else if (dataKey == mDataKeys.itemCrcAlgorithm) {
                item.data.itemCrcAlgorithm = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcStartIndex) {
                item.data.itemCrcStartIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcEndIndex) {
                item.data.itemCrcEndIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemText) {
                item.data.itemText = value.toString();
            } else {
                qWarning() << "Unknown data key:" + dataKey;
            }

            mItems.replace(row, item);
        }
    }

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
    DataKeys keys;
    QStringList hs = const_cast<Emitter *>(this)->headers();
    if (column >= 0 && column < hs.count()) {
        const QString dataKey = hs.at(column);
        if (dataKey == mDataKeys.itemEnable) {
            return item.data.itemEnable;
        } else if (dataKey == mDataKeys.itemDescription) {
            return item.data.itemDescription;
        } else if (dataKey == mDataKeys.itemTextFormat) {
            return item.data.itemEscapeCharacter;
        } else if (dataKey == mDataKeys.itemEscapeCharacter) {
            return item.data.itemEscapeCharacter;
        } else if (dataKey == mDataKeys.itemInterval) {
            return item.data.itemInterval;
        } else if (dataKey == mDataKeys.itemPrefix) {
            return item.data.itemPrefix;
        } else if (dataKey == mDataKeys.itemSuffix) {
            return item.data.itemSuffix;
        } else if (dataKey == mDataKeys.itemCrcEnable) {
            return item.data.itemCrcEnable;
        } else if (dataKey == mDataKeys.itemCrcBigEndian) {
            return item.data.itemCrcBigEndian;
        } else if (dataKey == mDataKeys.itemCrcAlgorithm) {
            return item.data.itemCrcAlgorithm;
        } else if (dataKey == mDataKeys.itemCrcStartIndex) {
            return item.data.itemCrcStartIndex;
        } else if (dataKey == mDataKeys.itemCrcEndIndex) {
            return item.data.itemCrcEndIndex;
        } else if (dataKey == mDataKeys.itemText) {
            return item.data.itemText;
        } else {
            qWarning() << "Unknown data key:" + dataKey;
            return "Error";
        }
    }

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
