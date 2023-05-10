/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include <QTimer>
#include <QJSValue>
#include <QJsonObject>
#include <QJsonDocument>

#include "EDEmitterTool.hpp"

#include "common/EDCrc.hpp"
#include "common/EDInterface.hpp"
#include "common/EDDataStructure.hpp"

EDEmitterTableModel::EDEmitterTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    for (int i = 0; i < mTableColumnCount; i++) {
        mHeaders << EDEmitterTableModel::headerData(i, Qt::Horizontal).toString();
    }
}

int EDEmitterTableModel::rowCount(const QModelIndex &parent) const
{
    return mItems.count();
}

int EDEmitterTableModel::columnCount(const QModelIndex &parent) const
{
    return mHeaders.length();
}

QVariant EDEmitterTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        int column = index.column();
        const EDEmiterItem &item = mItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool EDEmitterTableModel::setData(const QModelIndex &index,
                                  const QVariant &value,
                                  int role)
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        auto item = mItems.at(row);
        int column = index.column();
        if (column >= 0 && column < mHeaders.count()) {
            auto dataKey = mHeaders.at(column);
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

            }

            mItems.replace(row, item);
            emit dataChanged(index, index);
        }
    }

    return true;
}

bool EDEmitterTableModel::insertRows(int row, int count,
                                     const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertColumns(parent, row, rowCount() + count);
    EDEmiterItem item{EDEmiterData{}, 0};
    for (int i = 0; i < count; i++) {
        mItems.insert(row, item);
    }
    endInsertRows();

    return true;
}

bool EDEmitterTableModel::removeRows(int row, int count,
                                     const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    mItems.remove(row, count);
    endRemoveRows();
    return true;
}

QVariant EDEmitterTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
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

QByteArray EDEmitterTableModel::itemBytes(const EDEmiterData &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = EDDataStructure::cookedString(item.itemEscapeCharacter, text);
    bytes = EDInterface::string2array(text, item.itemTextFormat);
    EDCrc edCrc;
    QByteArray crcBytes = edCrc.calculateBytes(bytes,
                                               item.itemCrcAlgorithm,
                                               item.itemCrcStartIndex,
                                               item.itemCrcEndIndex);
    QByteArray prefix = EDDataStructure::affixesData(item.itemPrefix);
    QByteArray suffix = EDDataStructure::affixesData(item.itemSuffix);

    bytes.prepend(prefix);
    if (item.itemCrcEnable) {
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

QVariant EDEmitterTableModel::columnDisplayRoleData(
    const EDEmiterItem &item, int column) const
{
    if (column >= 0 && column < mHeaders.count()) {
        const QString dataKey = mHeaders.at(column);
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
            return "Error";
        }
    }

    return QVariant("Error");
}

EDEmitterTool::EDEmitterTool(QObject *parent)
    : EDBaseTool{"ED.EmitterTool", parent}
{
    mTableModel = new EDEmitterTableModel(this);
    mHeaders = mTableModel->mHeaders;
}

void EDEmitterTool::addItem(const QString &jsonCtx, int index)
{

    QByteArray json = jsonCtx.toLatin1();
    QJsonObject jsonObj = QJsonDocument::fromJson(json).object();
    if (!(index >= 0 && index < mTableModel->mItems.count())) {
        mTableModel->insertRows(mTableModel->rowCount(), 1);
        index = mTableModel->rowCount() - 1;
    }

    for (int i = 0; i < mHeaders.count(); i++) {
        auto key = mHeaders.at(i);
        auto modelIndex = mTableModel->index(index, i);
        mTableModel->setData(modelIndex, jsonObj.value(key), Qt::EditRole);
    }
}

QVariant EDEmitterTool::itemContext(int index)
{
    QJsonObject ctx;
    mTableModel->mItemsMutex.lock();
    if (index >= 0 && index < mTableModel->mItems.count()) {
        auto item = mTableModel->mItems.at(index);
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
        ctx.insert(itemTextFormat(), EDDataStructure::TextFormatAscii);
        ctx.insert(itemEscapeCharacter(),
                   EDDataStructure::EscapeCharacterOptionNone);
        ctx.insert(itemInterval(), 1000);
        ctx.insert(itemPrefix(), EDDataStructure::AffixesNone);
        ctx.insert(itemSuffix(), EDDataStructure::AffixesNone);
        ctx.insert(itemCrcEnable(), true);
        ctx.insert(itemCrcAlgorithm(), EDCrc::CRC_8);
        ctx.insert(itemCrcStartIndex(), 0);
        ctx.insert(itemCrcEndIndex(), 0);
        ctx.insert(itemText(), "This is a demo.");
    }
    mTableModel->mItemsMutex.unlock();

    return ctx;
}

QVariant EDEmitterTool::itemsContext()
{
    QVariantList varList;
    int rowCount = mTableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        varList.append(itemContext(i));
    }

    return varList;
}

bool EDEmitterTool::initialize(QString &errStr)
{
    // The way of timming sending should be improved.
    mEmittingTimer = new QTimer();
    mEmittingTimer->setInterval(mScanInterval);
    mEmittingTimer->setSingleShot(true);
    connect(mEmittingTimer, &QTimer::timeout, mEmittingTimer, [=](){
        emit invokeOutputBytes(EDPrivateSignal{});
    });
    mEmittingTimer->start();
    return true;
}

void EDEmitterTool::outputBytesHandler()
{
    mTableModel->mItemsMutex.lock();
    for (auto &item : mTableModel->mItems) {
        int elapsedTime = item.elapsedTime += mScanInterval;
        if (elapsedTime > item.data.itemInterval && item.data.itemEnable) {
            item.elapsedTime = 0;
            const auto bytes = mTableModel->itemBytes(item.data);
            outputBytes(QByteArray(bytes));
        }
    }
    mTableModel->mItemsMutex.unlock();
    mEmittingTimer->start();
}

void EDEmitterTool::uninitialize()
{
    if (mEmittingTimer) {
        mEmittingTimer->stop();
        mEmittingTimer->deleteLater();
    }
}
