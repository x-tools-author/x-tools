/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTimer>
#include <QJSValue>
#include <QJsonObject>
#include <QJsonDocument>

#include "SAKPrestorerTool.hh"

#include "common/EDCrc.hpp"
#include "common/EDInterface.hpp"
#include "common/EDDataStructure.hpp"

SAKPrestorerTableModel::SAKPrestorerTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    for (int i = 0; i < mTableColumnCount; i++) {
        mHeaders << SAKPrestorerTableModel::headerData(i, Qt::Horizontal).toString();
    }
}

int SAKPrestorerTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mItems.count();
}

int SAKPrestorerTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mHeaders.length();
}

QVariant SAKPrestorerTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        int column = index.column();
        const EDPrestoreItem &item = mItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool SAKPrestorerTableModel::setData(const QModelIndex &index,
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
            } else if (dataKey == mDataKeys.itemCrcAlgorithm) {
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
            emit dataChanged(index, index);
        }
    }

    return true;
}

bool SAKPrestorerTableModel::insertRows(int row, int count,
                                     const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertColumns(parent, row, rowCount() + count);
    EDPrestoreItem item{};
    for (int i = 0; i < count; i++) {
        mItems.insert(row, item);
    }
    endInsertRows();

    return true;
}

bool SAKPrestorerTableModel::removeRows(int row, int count,
                                     const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    mItems.remove(row, count);
    endRemoveRows();
    return true;
}

QVariant SAKPrestorerTableModel::headerData(int section,
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
        case 6: return mDataKeys.itemCrcAlgorithm;
        case 7: return mDataKeys.itemCrcStartIndex;
        case 8: return mDataKeys.itemCrcEndIndex;
        case 9: return mDataKeys.itemText;
        default: return "";
        }
    }

    return QVariant("");
}

QByteArray SAKPrestorerTableModel::itemBytes(const EDPrestoreItem &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = EDDataStructure::cookedString(item.itemEscapeCharacter, text);
    bytes = EDInterface::string2array(text, item.itemTextFormat);
    EDCrc edCrc;
    QByteArray prefix = EDDataStructure::affixesData(item.itemPrefix);
    QByteArray crcBytes;
    QByteArray suffix = EDDataStructure::affixesData(item.itemSuffix);

    if (item.itemCrcEnable) {
        crcBytes = edCrc.calculateBytes(bytes,
                                        item.itemCrcAlgorithm,
                                        item.itemCrcStartIndex,
                                        item.itemCrcEndIndex);
    }

    bytes.prepend(prefix);
    bytes.append(crcBytes);
    bytes.append(suffix);

    return bytes;
}

QVariant SAKPrestorerTableModel::columnDisplayRoleData(const EDPrestoreItem &item, int column) const
{
    if (column >= 0 && column < mHeaders.count()) {
        const QString dataKey = mHeaders.at(column);
        if (dataKey == mDataKeys.itemDescription) {
            return item.itemDescription;
        } else if (dataKey == mDataKeys.itemTextFormat) {
            return item.itemEscapeCharacter;
        } else if (dataKey == mDataKeys.itemEscapeCharacter) {
            return item.itemEscapeCharacter;
        } else if (dataKey == mDataKeys.itemPrefix) {
            return item.itemPrefix;
        } else if (dataKey == mDataKeys.itemSuffix) {
            return item.itemSuffix;
        } else if (dataKey == mDataKeys.itemCrcEnable) {
            return item.itemCrcEnable;
        } else if (dataKey == mDataKeys.itemCrcAlgorithm) {
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

SAKPrestorerTool::SAKPrestorerTool(QObject *parent)
    : SAKBaseTool{"SAK.PresetDataTool", parent}
{
    mTableModel = new SAKPrestorerTableModel(this);
    mHeaders = mTableModel->mHeaders;
}

void SAKPrestorerTool::addItem(const QString &jsonCtx, int index)
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

QVariant SAKPrestorerTool::itemContext(int index)
{
    QJsonObject ctx;
    mTableModel->mItemsMutex.lock();
    if (index >= 0 && index < mTableModel->mItems.count()) {
        auto item = mTableModel->mItems.at(index);
        ctx.insert(itemDescription(), item.itemDescription);
        ctx.insert(itemTextFormat(), item.itemTextFormat);
        ctx.insert(itemEscapeCharacter(), item.itemEscapeCharacter);
        ctx.insert(itemPrefix(), item.itemPrefix);
        ctx.insert(itemSuffix(), item.itemSuffix);
        ctx.insert(itemCrcEnable(), item.itemCrcEnable);
        ctx.insert(itemCrcAlgorithm(), item.itemCrcAlgorithm);
        ctx.insert(itemCrcStartIndex(), item.itemCrcStartIndex);
        ctx.insert(itemCrcEndIndex(), item.itemCrcEndIndex);
        ctx.insert(itemText(), item.itemText);
    } else {
        ctx.insert(itemDescription(), "Demo");
        ctx.insert(itemTextFormat(), EDDataStructure::TextFormatAscii);
        ctx.insert(itemEscapeCharacter(),
                   EDDataStructure::EscapeCharacterOptionNone);
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

QVariant SAKPrestorerTool::itemsContext()
{
    QVariantList varList;
    int rowCount = mTableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        varList.append(itemContext(i));
    }

    return varList;
}

void SAKPrestorerTool::send(int index)
{
    if (isRunning()) {
        mIndexsMutex.lock();
        mIndexs.append(index);
        mIndexsMutex.unlock();
    }
}

void SAKPrestorerTool::inputBYtes(const QByteArray &bytes, const QVariant *context)
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

    mTableModel->mItemsMutex.lock();
    if (index >= 0 && index < mTableModel->mItems.count()) {
        auto item = mTableModel->mItems.at(index);
        QByteArray bytes = mTableModel->itemBytes(item);
        emit bytesOutputted(bytes, QVariant());
    }
    mTableModel->mItemsMutex.unlock();
}

QStringList SAKPrestorerTool::descriptions()
{
    mTableModel->mItemsMutex.lock();
    auto items = mTableModel->mItems;
    mTableModel->mItemsMutex.unlock();

    QStringList list;
    for (const auto &item : items) {
        list.append(item.itemDescription);
    }

    return list;
}

