/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QTimer>
#include <QJsonArray>
#include <QTableView>
#include <QHeaderView>
#include <QJsonDocument>
#include <QStandardItemModel>

#include "SAKInterface.hh"
#include "SAKCrcInterface.hh"
#include "SAKDataStructure.hh"
#include "SAKResponserTool.hh"

SAKResponserTool::SAKResponserTool(QObject *parent)
    : SAKTabelModelTool{"ED.ResponserTool", parent}
{

}

int SAKResponserTool::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mItems.count();
}

int SAKResponserTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mTableColumnCount;
}

QVariant SAKResponserTool::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < mItems.count()) {
        int column = index.column();
        const ResponserData &item = mItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool SAKResponserTool::setData(const QModelIndex &index,
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
            } else if (dataKey == mDataKeys.itemOption) {
                item.data.itemOption = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceTextFormat) {
                item.data.itemReferenceTextFormat = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceEscapeCharacter) {
                item.data.itemReferenceEscapeCharacter = value.toInt();
            } else if (dataKey == mDataKeys.itemReferencePrefix) {
                item.data.itemReferencePrefix = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceSuffix) {
                item.data.itemReferenceSuffix = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceCrcEnable) {
                item.data.itemReferenceCrcEnable = value.toBool();
            } else if (dataKey == mDataKeys.itemReferenceCrcAlgorithm) {
                item.data.itemReferenceCrcAlgorithm = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceCrcStartIndex) {
                item.data.itemReferenceCrcStartIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceCrcEndIndex) {
                item.data.itemReferenceCrcEndIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemReferenceText) {
                item.data.itemReferenceText = value.toString();
            } else if (dataKey == mDataKeys.itemResponseTextFormat) {
                item.data.itemResponseTextFormat = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseEscapeCharacter) {
                item.data.itemResponseEscapeCharacter = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseInterval) {
                item.data.itemResponseInterval = value.toInt();
            } else if (dataKey == mDataKeys.itemResponsePrefix) {
                item.data.itemResponsePrefix = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseSuffix) {
                item.data.itemResponseSuffix = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseCrcEnable) {
                item.data.itemResponseCrcEnable = value.toBool();
            } else if (dataKey == mDataKeys.itemResponseCrcAlgorithm) {
                item.data.itemResponseCrcAlgorithm = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseCrcStartIndex) {
                item.data.itemResponseCrcStartIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseCrcEndIndex) {
                item.data.itemResponseCrcEndIndex = value.toInt();
            } else if (dataKey == mDataKeys.itemResponseText) {
                item.data.itemResponseText = value.toString();
            } else {
                // Nothing to do yet.
            }

            mItems.replace(row, item);
        }
    }

    return true;
}

bool SAKResponserTool::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    ResponserData item{ResponserItem{}, 0};
    for (int i = 0; i < count; i++) {
        mItems.insert(row, item);
    }

    return true;
}

bool SAKResponserTool::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    mItems.remove(row, count);
    return true;
}

QVariant SAKResponserTool::headerData(int section,
                                            Qt::Orientation orientation,
                                            int role) const
{
    Q_UNUSED(role);
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:  return mDataKeys.itemEnable;
        case 1:  return mDataKeys.itemDescription;
        case 2:  return mDataKeys.itemOption;
        case 3:  return mDataKeys.itemReferenceTextFormat;
        case 4:  return mDataKeys.itemReferenceEscapeCharacter;
        case 5:  return mDataKeys.itemReferencePrefix;
        case 6:  return mDataKeys.itemReferenceSuffix;
        case 7:  return mDataKeys.itemReferenceCrcEnable;
        case 8:  return mDataKeys.itemReferenceCrcAlgorithm;
        case 9:  return mDataKeys.itemReferenceCrcStartIndex;
        case 10: return mDataKeys.itemReferenceCrcEndIndex;
        case 11: return mDataKeys.itemReferenceText;
        case 12: return mDataKeys.itemResponseTextFormat;
        case 13: return mDataKeys.itemResponseEscapeCharacter;
        case 14: return mDataKeys.itemResponseInterval;
        case 15: return mDataKeys.itemResponsePrefix;
        case 16: return mDataKeys.itemResponseSuffix;
        case 17: return mDataKeys.itemResponseCrcEnable;
        case 18: return mDataKeys.itemResponseCrcAlgorithm;
        case 19: return mDataKeys.itemResponseCrcStartIndex;
        case 20: return mDataKeys.itemResponseCrcEndIndex;
        case 21: return mDataKeys.itemResponseText;
        default: return "";
        }
    }

    return QVariant("");
}

QVariant SAKResponserTool::columnDisplayRoleData(
    const ResponserData &item, int column) const
{
    if (column >= 0 && column < headers().count()) {
        const QString dataKey = headers().at(column);
        if (dataKey == mDataKeys.itemEnable) {
            return item.data.itemEnable;
        } else if (dataKey == mDataKeys.itemDescription) {
            return item.data.itemDescription;
        } else if (dataKey == mDataKeys.itemOption) {
            return item.data.itemOption;
        } else if (dataKey == mDataKeys.itemReferenceTextFormat) {
            return item.data.itemReferenceEscapeCharacter;
        } else if (dataKey == mDataKeys.itemReferenceEscapeCharacter) {
            return item.data.itemReferenceEscapeCharacter;
        } else if (dataKey == mDataKeys.itemReferencePrefix) {
            return item.data.itemReferencePrefix;
        } else if (dataKey == mDataKeys.itemReferenceSuffix) {
            return item.data.itemReferenceSuffix;
        } else if (dataKey == mDataKeys.itemReferenceCrcEnable) {
            return item.data.itemReferenceCrcEnable;
        } else if (dataKey == mDataKeys.itemReferenceCrcAlgorithm) {
            return item.data.itemReferenceCrcAlgorithm;
        } else if (dataKey == mDataKeys.itemReferenceCrcStartIndex) {
            return item.data.itemReferenceCrcStartIndex;
        } else if (dataKey == mDataKeys.itemReferenceCrcEndIndex) {
            return item.data.itemReferenceCrcEndIndex;
        } else if (dataKey == mDataKeys.itemReferenceText) {
            return item.data.itemReferenceText;
        } else if (dataKey == mDataKeys.itemResponseTextFormat) {
            return item.data.itemResponseEscapeCharacter;
        } else if (dataKey == mDataKeys.itemResponseEscapeCharacter) {
            return item.data.itemResponseEscapeCharacter;
        } else if (dataKey == mDataKeys.itemResponseInterval) {
            return item.data.itemResponseInterval;
        } else if (dataKey == mDataKeys.itemResponsePrefix) {
            return item.data.itemResponsePrefix;
        } else if (dataKey == mDataKeys.itemResponseSuffix) {
            return item.data.itemResponseSuffix;
        } else if (dataKey == mDataKeys.itemResponseCrcEnable) {
            return item.data.itemResponseCrcEnable;
        } else if (dataKey == mDataKeys.itemResponseCrcAlgorithm) {
            return item.data.itemResponseCrcAlgorithm;
        } else if (dataKey == mDataKeys.itemResponseCrcStartIndex) {
            return item.data.itemResponseCrcStartIndex;
        } else if (dataKey == mDataKeys.itemResponseCrcEndIndex) {
            return item.data.itemResponseCrcEndIndex;
        } else if (dataKey == mDataKeys.itemResponseText) {
            return item.data.itemResponseText;
        } else {
            return "Error";
        }
    }

    return QVariant("Error");
}

QByteArray SAKResponserTool::referenceBytes(const ResponserItem &item) const
{
    QByteArray bytes;
    QString text = item.itemReferenceText ;
    text = SAKDataStructure::cookedString(item.itemReferenceEscapeCharacter, text);
    bytes = SAKInterface::string2array(text, item.itemReferenceTextFormat);
    SAKCrcInterface edCrc;
    QByteArray crcBytes = edCrc.calculateBytes(bytes,
                                               item.itemReferenceCrcAlgorithm,
                                               item.itemReferenceCrcStartIndex,
                                               item.itemReferenceCrcEndIndex);
    QByteArray prefix = SAKDataStructure::affixesData(item.itemReferencePrefix);
    QByteArray suffix = SAKDataStructure::affixesData(item.itemReferenceSuffix);

    bytes.prepend(prefix);
    bytes.append(crcBytes);
    bytes.append(suffix);

    return bytes;
}

QByteArray SAKResponserTool::responseBytes(const ResponserItem &item) const
{
    QByteArray bytes;
    QString text = item.itemResponseText;
    text = SAKDataStructure::cookedString(item.itemResponseEscapeCharacter, text);
    bytes = SAKInterface::string2array(text, item.itemResponseTextFormat);
    SAKCrcInterface edCrc;
    QByteArray crcBytes = edCrc.calculateBytes(bytes,
                                               item.itemResponseCrcAlgorithm,
                                               item.itemResponseCrcStartIndex,
                                               item.itemResponseCrcEndIndex);
    QByteArray prefix = SAKDataStructure::affixesData(item.itemResponsePrefix);
    QByteArray suffix = SAKDataStructure::affixesData(item.itemResponseSuffix);

    bytes.prepend(prefix);
    bytes.append(crcBytes);
    bytes.append(suffix);

    return bytes;
}

void SAKResponserTool::addItem(const QString &jsonCtx, int index)
{

    QByteArray json = jsonCtx.toLatin1();
    QJsonObject jsonObj = QJsonDocument::fromJson(json).object();
    if (!(index >= 0 && index < mItems.count())) {
        insertRows(rowCount(), 1);
        index = rowCount() - 1;
    }

    for (int i = 0; i < headers().count(); i++) {
        auto key = headers().at(i);
        auto modelIndex = mTableModel->index(index, i);
        setData(modelIndex, jsonObj.value(key), Qt::EditRole);
    }
}

QVariant SAKResponserTool::itemContext(int index)
{
    QJsonObject ctx;
    mItemsMutex.lock();
    if (index >= 0 && index < mItems.count()) {
        auto item = mItems.at(index);
        ctx.insert(itemEnable(), item.data.itemEnable);
        ctx.insert(itemDescription(), item.data.itemDescription);
        ctx.insert(itemOption(), item.data.itemOption);

        ctx.insert(itemReferenceTextFormat(), item.data.itemReferenceTextFormat);
        ctx.insert(itemReferenceEscapeCharacter(), item.data.itemReferenceEscapeCharacter);
        ctx.insert(itemReferencePrefix(), item.data.itemReferencePrefix);
        ctx.insert(itemReferenceSuffix(), item.data.itemReferenceSuffix);
        ctx.insert(itemReferenceCrcEnable(), item.data.itemReferenceCrcEnable);
        ctx.insert(itemReferenceCrcAlgorithm(), item.data.itemReferenceCrcAlgorithm);
        ctx.insert(itemReferenceCrcStartIndex(), item.data.itemReferenceCrcStartIndex);
        ctx.insert(itemReferenceCrcEndIndex(), item.data.itemReferenceCrcEndIndex);
        ctx.insert(itemReferenceText(), item.data.itemReferenceText);

        ctx.insert(itemResponseInterval(), item.data.itemResponseInterval);
        ctx.insert(itemResponseTextFormat(), item.data.itemResponseTextFormat);
        ctx.insert(itemResponseEscapeCharacter(), item.data.itemResponseEscapeCharacter);
        ctx.insert(itemResponsePrefix(), item.data.itemResponsePrefix);
        ctx.insert(itemResponseSuffix(), item.data.itemResponseSuffix);
        ctx.insert(itemResponseCrcEnable(), item.data.itemResponseCrcEnable);
        ctx.insert(itemResponseCrcAlgorithm(), item.data.itemResponseCrcAlgorithm);
        ctx.insert(itemResponseCrcStartIndex(), item.data.itemResponseCrcStartIndex);
        ctx.insert(itemResponseCrcEndIndex(), item.data.itemResponseCrcEndIndex);
        ctx.insert(itemResponseText(), item.data.itemResponseText);
    } else {
        // If index is out of range, return the default values.
        ctx.insert(itemEnable(), true);
        ctx.insert(itemDescription(), "Demo");
        ctx.insert(itemOption(), 0);
        
        ctx.insert(itemReferenceTextFormat(), SAKDataStructure::TextFormatAscii);
        ctx.insert(itemReferenceEscapeCharacter(), SAKDataStructure::EscapeCharacterOptionNone);
        ctx.insert(itemReferencePrefix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemReferenceSuffix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemReferenceCrcEnable(), true);
        ctx.insert(itemReferenceCrcAlgorithm(), SAKCrcInterface::CRC_8);
        ctx.insert(itemReferenceCrcStartIndex(), 0);
        ctx.insert(itemReferenceCrcEndIndex(), 0);
        ctx.insert(itemReferenceText(), "Reference data.");

        ctx.insert(itemResponseInterval(), 1000);
        ctx.insert(itemResponseTextFormat(), SAKDataStructure::TextFormatAscii);
        ctx.insert(itemResponseEscapeCharacter(), SAKDataStructure::EscapeCharacterOptionNone);
        ctx.insert(itemResponsePrefix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemResponseSuffix(), SAKDataStructure::AffixesNone);
        ctx.insert(itemResponseCrcEnable(), true);
        ctx.insert(itemResponseCrcAlgorithm(), SAKCrcInterface::CRC_8);
        ctx.insert(itemResponseCrcStartIndex(), 0);
        ctx.insert(itemResponseCrcEndIndex(), 0);
        ctx.insert(itemResponseText(), "Response data.");
    }
    mItemsMutex.unlock();

    return ctx;
}

QVariant SAKResponserTool::itemsContext()
{
    QVariantList varList;
    int rowCount = mTableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        varList.append(itemContext(i));
    }

    return varList;
}

void SAKResponserTool::inputBytes(const QByteArray &bytes, const QVariant &context)
{
    mInputContextListMutex.lock();
    mInputContextList.append({bytes, context});
    mInputContextListMutex.unlock();;
}

void SAKResponserTool::run()
{
    QTimer *outputTimer = new QTimer();
    outputTimer->setInterval(5);
    outputTimer->setSingleShot(true);

    connect(outputTimer, &QTimer::timeout, outputTimer, [=](){
        mInputContextListMutex.lock();
        while (!mInputContextList.isEmpty()) {
            QByteArray bytes = mInputContextList.takeFirst().bytes;
            try2output(bytes, outputTimer);
        }
        mInputContextListMutex.unlock();
        outputTimer->start();
    });

    outputTimer->start();
    exec();

    mInputContextListMutex.lock();
    mInputContextList.clear();
    mInputContextListMutex.unlock();

    outputTimer->deleteLater();
    outputTimer = nullptr;
}

void SAKResponserTool::try2output(const QByteArray &bytes, QObject *threadInnerObject)
{
    mItemsMutex.lock();
    auto items = mItems;
    mItemsMutex.unlock();

    for (const auto &item : items) {
        if (!item.data.itemEnable) {
            continue;
        }

        auto refBytes = referenceBytes(item.data);
        auto resBytes = responseBytes(item.data);
        bool enableResponse = false;
        if (item.data.itemOption == SAKDataStructure::ResponseOptionAways) {
            enableResponse = true;
        } else if (item.data.itemOption == SAKDataStructure::ResponseOptionEcho) {
            resBytes = bytes;
            enableResponse = true;
        } else if (item.data.itemOption == SAKDataStructure::ResponseOptionInputContainReference) {
            enableResponse = bytes.contains(refBytes);
        } else if (item.data.itemOption == SAKDataStructure::ResponseOptionInputDiscontainReference) {
            enableResponse = !bytes.contains(refBytes);
        } else if (item.data.itemOption == SAKDataStructure::ResponseOptionInputEqualReference) {
            enableResponse = bytes == refBytes;
        }

        if (enableResponse) {
            QTimer::singleShot(item.data.itemResponseInterval, threadInnerObject, [=](){
                emit bytesOutputted(item.data.itemResponseText.toUtf8(), QVariant());
            });
        }
    }
}

QString SAKResponserTool::itemEnable()
{
    return mDataKeys.itemEnable;
}

QString SAKResponserTool::itemDescription()
{
    return mDataKeys.itemDescription;
}

QString SAKResponserTool::itemOption()
{
    return mDataKeys.itemOption;
}

QString SAKResponserTool::itemReferenceTextFormat()
{
    return mDataKeys.itemReferenceTextFormat;
}

QString SAKResponserTool::itemReferenceEscapeCharacter()
{
    return mDataKeys.itemReferenceEscapeCharacter;
}

QString SAKResponserTool::itemReferencePrefix()
{
    return mDataKeys.itemReferencePrefix;
}

QString SAKResponserTool::itemReferenceSuffix()
{
    return mDataKeys.itemReferenceSuffix;
}

QString SAKResponserTool::itemReferenceCrcEnable()
{
    return mDataKeys.itemReferenceCrcEnable;
}

QString SAKResponserTool::itemReferenceCrcAlgorithm()
{
    return mDataKeys.itemReferenceCrcAlgorithm;
}

QString SAKResponserTool::itemReferenceCrcStartIndex()
{
    return mDataKeys.itemReferenceCrcStartIndex;
}

QString SAKResponserTool::itemReferenceCrcEndIndex()
{
    return mDataKeys.itemReferenceCrcEndIndex;
}

QString SAKResponserTool::itemReferenceText()
{
    return mDataKeys.itemReferenceText;
}

QString SAKResponserTool::itemResponseTextFormat()
{
    return mDataKeys.itemResponseTextFormat;
}

QString SAKResponserTool::itemResponseEscapeCharacter()
{
    return mDataKeys.itemResponseEscapeCharacter;
}

QString SAKResponserTool::itemResponseInterval()
{
    return mDataKeys.itemResponseInterval;
}

QString SAKResponserTool::itemResponsePrefix()
{
    return mDataKeys.itemResponsePrefix;
}

QString SAKResponserTool::itemResponseSuffix()
{
    return mDataKeys.itemResponseSuffix;
}

QString SAKResponserTool::itemResponseCrcEnable()
{
    return mDataKeys.itemResponseCrcEnable;
}

QString SAKResponserTool::itemResponseCrcAlgorithm()
{
    return mDataKeys.itemResponseCrcAlgorithm;
}

QString SAKResponserTool::itemResponseCrcStartIndex()
{
    return mDataKeys.itemResponseCrcStartIndex;
}

QString SAKResponserTool::itemResponseCrcEndIndex()
{
    return mDataKeys.itemResponseCrcEndIndex;
}

QString SAKResponserTool::itemResponseText()
{
    return mDataKeys.itemResponseText;
}
