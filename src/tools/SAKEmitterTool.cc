/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QJSValue>
#include <QJsonObject>
#include <QJsonDocument>

#include "SAKEmitterTool.hh"

#include "common/SAKCrcInterface.hh"
#include "common/SAKInterface.hh"
#include "common/SAKDataStructure.hh"

SAKEmitterTableModel::SAKEmitterTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    for (int i = 0; i < mTableColumnCount; i++) {
        mHeaders << SAKEmitterTableModel::headerData(i, Qt::Horizontal).toString();
    }
}

int SAKEmitterTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mItems.count();
}

int SAKEmitterTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mHeaders.length();
}

QVariant SAKEmitterTableModel::data(const QModelIndex &index, int role) const
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

bool SAKEmitterTableModel::setData(const QModelIndex &index,
                                  const QVariant &value,
                                  int role)
{
    Q_UNUSED(role);
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

bool SAKEmitterTableModel::insertRows(int row, int count,
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

bool SAKEmitterTableModel::removeRows(int row, int count,
                                     const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    mItems.remove(row, count);
    endRemoveRows();
    return true;
}

QVariant SAKEmitterTableModel::headerData(int section,
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

QByteArray SAKEmitterTableModel::itemBytes(const EDEmiterData &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = SAKDataStructure::cookedString(item.itemEscapeCharacter, text);
    bytes = SAKInterface::string2array(text, item.itemTextFormat);
    SAKCrcInterface edCrc;
    QByteArray crcBytes = edCrc.calculateBytes(bytes,
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

QVariant SAKEmitterTableModel::columnDisplayRoleData(
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

SAKEmitterTool::SAKEmitterTool(QObject *parent)
    : SAKBaseTool{"SAK.EmitterTool", parent}
{
    mTableModel = new SAKEmitterTableModel(this);
    mHeaders = mTableModel->mHeaders;
}

void SAKEmitterTool::addItem(const QString &jsonCtx, int index)
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

QVariant SAKEmitterTool::itemContext(int index)
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
    mTableModel->mItemsMutex.unlock();

    return ctx;
}

QVariant SAKEmitterTool::itemsContext()
{
    QVariantList varList;
    int rowCount = mTableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        varList.append(itemContext(i));
    }

    return varList;
}

SAKEmitterTableModel *SAKEmitterTool::getModel()
{
    return mTableModel;
}

QStringList SAKEmitterTool::getHeaders()
{
    return mHeaders;
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
    }
}

void SAKEmitterTool::try2emit()
{
    mTableModel->mItemsMutex.lock();
    for (auto &item : mTableModel->mItems) {
        int elapsedTime = item.elapsedTime += mScanInterval;
        if (elapsedTime > item.data.itemInterval && item.data.itemEnable) {
            item.elapsedTime = 0;
            const auto bytes = mTableModel->itemBytes(item.data);
            emit bytesOutputted(bytes, QVariant());
        }
    }
    mTableModel->mItemsMutex.unlock();
    mEmittingTimer->start();
}
