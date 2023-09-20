/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

#include "sakinterface.h"
#include "sakcrcinterface.h"
#include "SAKPrestorerTool.h"
#include "sakdatastructure.h"

SAKPrestorerTool::SAKPrestorerTool(QObject *parent)
    : SAKTableModelTool{"sak.prestorertool", parent}
{

}

int SAKPrestorerTool::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mItems.count();
}

int SAKPrestorerTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mTableColumnCount;
}

QVariant SAKPrestorerTool::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        int column = index.column();
        const Item &item = mItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool SAKPrestorerTool::setData(const QModelIndex &index,
                                     const QVariant &value,
                                     int role)
{
    Q_UNUSED(role);
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        auto item = mItems.at(row);
        int column = index.column();
        if (column >= 0 && column < headers().count()) {
            auto dataKey = headers().at(column);
            if (dataKey == mDataKeys.itemDescription) {
                item.itemDescription = value.toString();
            } else if (dataKey == mDataKeys.itemTextFormat) {
                item.itemTextFormat = value.toInt();
            } else if (dataKey == mDataKeys.itemEscapeCharacter) {
                item.itemEscapeCharacter = value.toInt();
            } else if (dataKey == mDataKeys.itemPrefix) {
                item.itemPrefix = value.toInt();
            } else if (dataKey == mDataKeys.itemSuffix) {
                item.itemSuffix = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcEnable) {
                item.itemCrcEnable = value.toBool();
            } else if (dataKey == mDataKeys.itemCrcBigEndian) {
                item.itemCrxBigEndian = value.toBool();
            }  else if (dataKey == mDataKeys.itemCrcAlgorithm) {
                item.itemCrcAlgorithm = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcStartIndex) {
                item.itemCrcStartIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcEndIndex) {
                item.itemCrcEndIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemText) {
                item.itemText = value.toString();
            } else {

            }

            mItems.replace(row, item);
        }
    }

    return true;
}

bool SAKPrestorerTool::insertRows(int row, int count,
                                        const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Item item{};
    for (int i = 0; i < count; i++) {
        mItems.insert(row, item);
    }

    return true;
}

bool SAKPrestorerTool::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    mItems.remove(row, count);
    return true;
}

QVariant SAKPrestorerTool::headerData(int section,
                                            Qt::Orientation orientation,
                                            int role) const
{
    Q_UNUSED(role);
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return mDataKeys.itemDescription;
        case 1: return mDataKeys.itemTextFormat;
        case 2: return mDataKeys.itemEscapeCharacter;
        case 3: return mDataKeys.itemPrefix;
        case 4: return mDataKeys.itemSuffix;
        case 5: return mDataKeys.itemCrcEnable;
        case 6: return mDataKeys.itemCrcBigEndian;
        case 7: return mDataKeys.itemCrcAlgorithm;
        case 8: return mDataKeys.itemCrcStartIndex;
        case 9: return mDataKeys.itemCrcEndIndex;
        case 10: return mDataKeys.itemText;
        default: return "";
        }
    }

    return QVariant("");
}

QByteArray SAKPrestorerTool::itemBytes(const Item &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = SAKDataStructure::cookedString(item.itemEscapeCharacter, text);
    bytes = SAKInterface::string2array(text, item.itemTextFormat);
    SAKCrcInterface sakCrc;
    QByteArray prefix = SAKDataStructure::affixesData(item.itemPrefix);
    QByteArray crcBytes;
    QByteArray suffix = SAKDataStructure::affixesData(item.itemSuffix);

    if (item.itemCrcEnable) {
        crcBytes = sakCrc.calculateBytes(bytes,
                                         item.itemCrcAlgorithm,
                                         item.itemCrcStartIndex,
                                         item.itemCrcEndIndex,
                                         item.itemCrxBigEndian);
    }

    bytes.prepend(prefix);
    bytes.append(crcBytes);
    bytes.append(suffix);

    return bytes;
}

QVariant SAKPrestorerTool::columnDisplayRoleData(const Item &item, int column) const
{
    if (column >= 0 && column < headers().count()) {
        const QString dataKey = headers().at(column);
        if (dataKey == mDataKeys.itemDescription) {
            return item.itemDescription;
        } else if (dataKey == mDataKeys.itemTextFormat) {
            return item.itemTextFormat;
        } else if (dataKey == mDataKeys.itemEscapeCharacter) {
            return item.itemEscapeCharacter;
        } else if (dataKey == mDataKeys.itemPrefix) {
            return item.itemPrefix;
        } else if (dataKey == mDataKeys.itemSuffix) {
            return item.itemSuffix;
        } else if (dataKey == mDataKeys.itemCrcEnable) {
            return item.itemCrcEnable;
        } else if (dataKey == mDataKeys.itemCrcBigEndian) {
            return item.itemCrxBigEndian;
        }  else if (dataKey == mDataKeys.itemCrcAlgorithm) {
            return item.itemCrcAlgorithm;
        } else if (dataKey == mDataKeys.itemCrcStartIndex) {
            return item.itemCrcStartIndex;
        } else if (dataKey == mDataKeys.itemCrcEndIndex) {
            return item.itemCrcEndIndex;
        } else if (dataKey == mDataKeys.itemText) {
            return item.itemText;
        } else {
            return "Error";
        }
    }

    return QVariant("Error");
}

QString SAKPrestorerTool::cookHeaderString(const QString &str)
{
    ItemKeys keys;
    if (str == keys.itemDescription) {
        return tr("Description");
    } else if (str == keys.itemTextFormat) {
        return tr("Format");
    } else if (str == keys.itemEscapeCharacter) {
        return tr("Escape");
    } else if (str == keys.itemPrefix) {
        return tr("Prefix");
    } else if (str == keys.itemSuffix) {
        return tr("Suffix");
    } else if (str == keys.itemCrcEnable) {
        return tr("CrcEnable");
    } else if (str == keys.itemCrcBigEndian) {
        return tr("BigEndian");
    } else if (str == keys.itemCrcAlgorithm) {
        return tr("Algorithm");
    } else if (str == keys.itemCrcStartIndex) {
        return tr("Start");
    } else if (str == keys.itemCrcEndIndex) {
        return tr("End");
    } else if (str == keys.itemText) {
        return tr("Data");
    }

    return "--";
}

QVariant SAKPrestorerTool::itemContext(int index)
{
    QJsonObject ctx;
    mItemsMutex.lock();
    if (index >= 0 && index < mItems.count()) {
        auto item =  mItems.at(index);
        ctx.insert(itemDescription(), item.itemDescription);
        ctx.insert(itemTextFormat(), item.itemTextFormat);
        ctx.insert(itemEscapeCharacter(), item.itemEscapeCharacter);
        ctx.insert(itemPrefix(), item.itemPrefix);
        ctx.insert(itemSuffix(), item.itemSuffix);
        ctx.insert(itemCrcEnable(), item.itemCrcEnable);
        ctx.insert(itemCrcBigEndian(), item.itemCrxBigEndian);
        ctx.insert(itemCrcAlgorithm(), item.itemCrcAlgorithm);
        ctx.insert(itemCrcStartIndex(), item.itemCrcStartIndex);
        ctx.insert(itemCrcEndIndex(), item.itemCrcEndIndex);
        ctx.insert(itemText(), item.itemText);
    } else {
        ctx.insert(itemDescription(), "Demo");
        ctx.insert(itemTextFormat(), SAKDataStructure::TextFormatAscii);
        ctx.insert(itemEscapeCharacter(),
                   SAKDataStructure::EscapeCharacterOptionNone);
        ctx.insert(itemPrefix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemSuffix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemCrcEnable(), false);
        ctx.insert(itemCrcBigEndian(), false);
        ctx.insert(itemCrcAlgorithm(), SAKCrcInterface::CRC_8);
        ctx.insert(itemCrcStartIndex(), 0);
        ctx.insert(itemCrcEndIndex(), 0);
        ctx.insert(itemText(), "This is a demo.");
    }
    mItemsMutex.unlock();

    return ctx;
}

QString SAKPrestorerTool::description(int index)
{
    QJsonObject obj = itemContext(index).toJsonObject();
    ItemKeys keys;
    return obj.value(keys.itemDescription).toString();
}

void SAKPrestorerTool::send(int index)
{
    if (isRunning()) {
        mIndexsMutex.lock();
        mIndexs.append(index);
        mIndexsMutex.unlock();
    }
}

void SAKPrestorerTool::inputBytes(const QByteArray &bytes,
                                  const QVariant &context)
{
    Q_UNUSED(bytes);
    Q_UNUSED(context);
}

void SAKPrestorerTool::run()
{
    QTimer *sendTimer = new QTimer();
    sendTimer->setInterval(mScanInterval);
    sendTimer->setSingleShot(true);
    connect(sendTimer, &QTimer::timeout, sendTimer, [=](){
        try2send();
        sendTimer->start();
    });
    sendTimer->start();

    exec();

    if (sendTimer) {
        sendTimer->stop();
        sendTimer->deleteLater();
    }
}

void SAKPrestorerTool::try2send()
{
    mIndexsMutex.lock();
    int index = mIndexs.isEmpty() ? -1 : mIndexs.takeFirst();
    mIndexsMutex.unlock();

    mItemsMutex.lock();
    if (index >= 0 && index < mItems.count()) {
        auto item = mItems.at(index);
        QByteArray bytes = itemBytes(item);
        emit bytesOutputted(bytes, QVariant());
    }
    mItemsMutex.unlock();
}

QString SAKPrestorerTool::itemDescription()
{
    return mDataKeys.itemDescription;
}

QString SAKPrestorerTool::itemTextFormat()
{
    return mDataKeys.itemTextFormat;
}

QString SAKPrestorerTool::itemEscapeCharacter()
{
    return mDataKeys.itemEscapeCharacter;
}

QString SAKPrestorerTool::itemPrefix()
{
    return mDataKeys.itemPrefix;
}

QString SAKPrestorerTool::itemSuffix()
{
    return mDataKeys.itemSuffix;
}

QString SAKPrestorerTool::itemCrcEnable()
{
    return mDataKeys.itemCrcEnable;
}

QString SAKPrestorerTool::itemCrcBigEndian()
{
    return mDataKeys.itemCrcBigEndian;
}

QString SAKPrestorerTool::itemCrcAlgorithm()
{
    return mDataKeys.itemCrcAlgorithm;
}

QString SAKPrestorerTool::itemCrcStartIndex()
{
    return mDataKeys.itemCrcStartIndex;
}

QString SAKPrestorerTool::itemCrcEndIndex()
{
    return mDataKeys.itemCrcEndIndex;
}

QString SAKPrestorerTool::itemText()
{
    return mDataKeys.itemText;
}

QStringList SAKPrestorerTool::descriptions()
{
    mItemsMutex.lock();
    auto items = mItems;
    mItemsMutex.unlock();

    QStringList list;
    for (const auto &item : items) {
        list.append(item.itemDescription);
    }

    return list;
}

