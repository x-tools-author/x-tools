/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QJsonObject>
#include <QJsonDocument>

#include "SAKEmitterTool.hh"

#include "SAKCrcInterface.hh"
#include "SAKInterface.hh"
#include "SAKDataStructure.hh"

SAKEmitterTool::SAKEmitterTool(QObject *parent)
    : SAKTableModelTool{"SAK.EmitterTool", parent}
{

}

void SAKEmitterTool::inputBytes(const QByteArray &bytes,
                                const QVariant &context)
{
    Q_UNUSED(bytes)
    Q_UNUSED(context)
}

QVariant SAKEmitterTool::itemContext(int index)
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
        ctx.insert(itemCrcAlgorithm(), item.data.itemCrcAlgorithm);
        ctx.insert(itemCrcStartIndex(), item.data.itemCrcStartIndex);
        ctx.insert(itemCrcEndIndex(), item.data.itemCrcEndIndex);
        ctx.insert(itemText(), item.data.itemText);
    } else {
        ctx.insert(itemEnable(), true);
        ctx.insert(itemDescription(), "Demo");
        ctx.insert(itemTextFormat(), SAKDataStructure::TextFormatAscii);
        ctx.insert(itemEscapeCharacter(),
                   SAKDataStructure::EscapeCharacterOptionNone);
        ctx.insert(itemInterval(), 1000);
        ctx.insert(itemPrefix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemSuffix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemCrcEnable(), true);
        ctx.insert(itemCrcAlgorithm(), SAKCrcInterface::CRC_8);
        ctx.insert(itemCrcStartIndex(), 0);
        ctx.insert(itemCrcEndIndex(), 0);
        ctx.insert(itemText(), "This is a demo.");
    }
    mItemsMutex.unlock();

    return ctx;
}

int SAKEmitterTool::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mItems.count();
}

int SAKEmitterTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mTableColumnCount;
}

QVariant SAKEmitterTool::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        int column = index.column();
        const EmiterItem &item = mItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool SAKEmitterTool::setData(const QModelIndex &index,
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
            } else if (dataKey == mDataKeys.itemCrcAlgorithm) {
                item.data.itemCrcAlgorithm = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcStartIndex) {
                item.data.itemCrcStartIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemCrcEndIndex) {
                item.data.itemCrcEndIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemText) {
                item.data.itemText = value.toString();
            } else {
                outputMessage(QtWarningMsg, "Unknown data key:" + dataKey);
            }

            mItems.replace(row, item);
        }
    }

    return true;
}

bool SAKEmitterTool::insertRows(int row,
                                int count,
                                const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Data ctx;
    EmiterItem item;
    item.data = ctx;
    item.elapsedTime = 0;
    for (int i = 0; i < count; i++) {
        mItems.insert(row, item);
    }

    return true;
}

bool SAKEmitterTool::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    mItems.remove(row, count);
    return true;
}

QVariant SAKEmitterTool::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    Q_UNUSED(role);
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return mDataKeys.itemEnable;
        case 1: return mDataKeys.itemDescription;
        case 2: return mDataKeys.itemTextFormat;
        case 3: return mDataKeys.itemEscapeCharacter;
        case 4: return mDataKeys.itemInterval;
        case 5: return mDataKeys.itemPrefix;
        case 6: return mDataKeys.itemSuffix;
        case 7: return mDataKeys.itemCrcEnable;
        case 8: return mDataKeys.itemCrcAlgorithm;
        case 9: return mDataKeys.itemCrcStartIndex;
        case 10: return mDataKeys.itemCrcEndIndex;
        case 11: return mDataKeys.itemText;
        default: return "";
        }
    }

    return QVariant("");
}

void SAKEmitterTool::run()
{
    mEmittingTimer = new QTimer();
    mEmittingTimer->setInterval(mScanInterval);
    mEmittingTimer->setSingleShot(true);
    connect(mEmittingTimer, &QTimer::timeout, mEmittingTimer, [=](){
        try2emit();
    });
    mEmittingTimer->start();

    exec();

    if (mEmittingTimer) {
        mEmittingTimer->stop();
        mEmittingTimer->deleteLater();
        mEmittingTimer = nullptr;
    }
}

void SAKEmitterTool::try2emit()
{
    mItemsMutex.lock();
    for (auto &item : mItems) {
        int elapsedTime = item.elapsedTime += mScanInterval;
        if (elapsedTime > item.data.itemInterval && item.data.itemEnable) {
            item.elapsedTime = 0;
            const auto bytes = itemBytes(item.data);
            emit bytesOutputted(bytes, QVariant());
        }
    }
    mItemsMutex.unlock();
    mEmittingTimer->start();
}

QByteArray SAKEmitterTool::itemBytes(const SAKEmitterTool::Data &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = SAKDataStructure::cookedString(item.itemEscapeCharacter, text);
    bytes = SAKInterface::string2array(text, item.itemTextFormat);
    SAKCrcInterface sakCrc;
    QByteArray crcBytes = sakCrc.calculateBytes(bytes,
                                               item.itemCrcAlgorithm,
                                               item.itemCrcStartIndex,
                                               item.itemCrcEndIndex);
    QByteArray prefix = SAKDataStructure::affixesData(item.itemPrefix);
    QByteArray suffix = SAKDataStructure::affixesData(item.itemSuffix);

    bytes.prepend(prefix);
    if (item.itemCrcEnable) {
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

QVariant SAKEmitterTool::columnDisplayRoleData(
    const SAKEmitterTool::EmiterItem &item, int column) const
{
    DataKeys keys;
    QStringList hs = const_cast<SAKEmitterTool*>(this)->headers();
    if (column >= 0 && column < hs.count()) {
        const QString dataKey = hs.at(column);
        if (dataKey == mDataKeys.itemEnable) {
            return item.data.itemEnable;
        } else if (dataKey == mDataKeys.itemDescription) {
            return item.data.itemDescription;
        }  else if (dataKey == mDataKeys.itemTextFormat) {
            return item.data.itemEscapeCharacter;
        }  else if (dataKey == mDataKeys.itemEscapeCharacter) {
            return item.data.itemEscapeCharacter;
        }  else if (dataKey == mDataKeys.itemInterval) {
            return item.data.itemInterval;
        }  else if (dataKey == mDataKeys.itemPrefix) {
            return item.data.itemPrefix;
        }  else if (dataKey == mDataKeys.itemSuffix) {
            return item.data.itemSuffix;
        }  else if (dataKey == mDataKeys.itemCrcEnable) {
            return item.data.itemCrcEnable;
        }  else if (dataKey == mDataKeys.itemCrcAlgorithm) {
            return item.data.itemCrcAlgorithm;
        }  else if (dataKey == mDataKeys.itemCrcStartIndex) {
            return item.data.itemCrcStartIndex;
        }  else if (dataKey == mDataKeys.itemCrcEndIndex) {
            return item.data.itemCrcEndIndex;
        }  else if (dataKey == mDataKeys.itemText) {
            return item.data.itemText;
        } else {
            outputMessage(QtWarningMsg, "Unknown data key:" + dataKey);
            return "Error";
        }
    }

    return QVariant("Error");
}

QString SAKEmitterTool::itemEnable()
{
    return mDataKeys.itemEnable;
}

QString SAKEmitterTool::itemDescription()
{
    return mDataKeys.itemDescription;
}

QString SAKEmitterTool::itemTextFormat()
{
    return mDataKeys.itemTextFormat;
}

QString SAKEmitterTool::itemEscapeCharacter()
{
    return mDataKeys.itemEscapeCharacter;
}

QString SAKEmitterTool::itemInterval()
{
    return mDataKeys.itemInterval;
}

QString SAKEmitterTool::itemPrefix()
{
    return mDataKeys.itemPrefix;
}

QString SAKEmitterTool::itemSuffix()
{
    return mDataKeys.itemSuffix;
}

QString SAKEmitterTool::itemCrcEnable()
{
    return mDataKeys.itemCrcEnable;
}

QString SAKEmitterTool::itemCrcAlgorithm()
{
    return mDataKeys.itemCrcAlgorithm;
}

QString SAKEmitterTool::itemCrcStartIndex()
{
    return mDataKeys.itemCrcStartIndex;
}

QString SAKEmitterTool::itemCrcEndIndex()
{
    return mDataKeys.itemCrcEndIndex;
}

QString SAKEmitterTool::itemText()
{
    return mDataKeys.itemText;
}
