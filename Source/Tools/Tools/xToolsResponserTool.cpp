/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsResponserTool.h"

#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>

#include "xToolsCrcInterface.h"
#include "xToolsDataStructure.h"

xToolsResponserTool::xToolsResponserTool(QObject *parent)
    : xToolsTableModelTool{parent}
{}

int xToolsResponserTool::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_iItems.count();
}

int xToolsResponserTool::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tableColumnCount;
}

QVariant xToolsResponserTool::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < m_iItems.count()) {
        int column = index.column();
        const ResponserData &item = m_iItems[row];
        if (role == Qt::DisplayRole) {
            return columnDisplayRoleData(item, column);
        }
    }

    return QVariant();
}

bool xToolsResponserTool::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    int row = index.row();
    if (row >= 0 && row < m_iItems.count()) {
        auto item = m_iItems.at(row);
        int column = index.column();
        if (column >= 0 && column < headers().count()) {
            auto dataKey = headers().at(column);
            if (dataKey == m_dataKeys.itemEnable) {
                item.data.itemEnable = value.toBool();
            } else if (dataKey == m_dataKeys.itemDescription) {
                item.data.itemDescription = value.toString();
            } else if (dataKey == m_dataKeys.itemOption) {
                item.data.itemOption = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceTextFormat) {
                item.data.itemReferenceTextFormat = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceEscapeCharacter) {
                item.data.itemReferenceEscapeCharacter = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferencePrefix) {
                item.data.itemReferencePrefix = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceSuffix) {
                item.data.itemReferenceSuffix = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceCrcEnable) {
                item.data.itemReferenceCrcEnable = value.toBool();
            } else if (dataKey == m_dataKeys.itemReferenceCrcBigEndian) {
                item.data.itemReferenceCrcBigEndian = value.toBool();
            } else if (dataKey == m_dataKeys.itemReferenceCrcAlgorithm) {
                item.data.itemReferenceCrcAlgorithm = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceCrcStartIndex) {
                item.data.itemReferenceCrcStartIndex = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceCrcEndIndex) {
                item.data.itemReferenceCrcEndIndex = value.toInt();
            } else if (dataKey == m_dataKeys.itemReferenceText) {
                item.data.itemReferenceText = value.toString();
            } else if (dataKey == m_dataKeys.itemResponseTextFormat) {
                item.data.itemResponseTextFormat = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseEscapeCharacter) {
                item.data.itemResponseEscapeCharacter = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponsePrefix) {
                item.data.itemResponsePrefix = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseSuffix) {
                item.data.itemResponseSuffix = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseCrcEnable) {
                item.data.itemResponseCrcEnable = value.toBool();
            } else if (dataKey == m_dataKeys.itemResponseCrcBigEndian) {
                item.data.itemResponseCrcBigEndian = value.toBool();
            } else if (dataKey == m_dataKeys.itemResponseCrcAlgorithm) {
                item.data.itemResponseCrcAlgorithm = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseCrcStartIndex) {
                item.data.itemResponseCrcStartIndex = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseCrcEndIndex) {
                item.data.itemResponseCrcEndIndex = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseDelay) {
                item.data.itemResponseDelay = value.toInt();
            } else if (dataKey == m_dataKeys.itemResponseText) {
                item.data.itemResponseText = value.toString();
            } else {
                // Nothing to do yet.
            }

            m_iItems.replace(row, item);
        }
    }

    return true;
}

bool xToolsResponserTool::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    ResponserItem ctx;
    ResponserData item;
    item.data = ctx;
    item.elapsedTime = 0;
    for (int i = 0; i < count; i++) {
        m_iItems.insert(row, item);
    }

    return true;
}

bool xToolsResponserTool::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    m_iItems.remove(row, count);
    return true;
}

QVariant xToolsResponserTool::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role);
    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return m_dataKeys.itemEnable;
        case 1:
            return m_dataKeys.itemDescription;
        case 2:
            return m_dataKeys.itemOption;
        case 3:
            return m_dataKeys.itemReferenceTextFormat;
        case 4:
            return m_dataKeys.itemReferenceEscapeCharacter;
        case 5:
            return m_dataKeys.itemReferencePrefix;
        case 6:
            return m_dataKeys.itemReferenceSuffix;
        case 7:
            return m_dataKeys.itemReferenceCrcEnable;
        case 8:
            return m_dataKeys.itemReferenceCrcBigEndian;
        case 9:
            return m_dataKeys.itemReferenceCrcAlgorithm;
        case 10:
            return m_dataKeys.itemReferenceCrcStartIndex;
        case 11:
            return m_dataKeys.itemReferenceCrcEndIndex;
        case 12:
            return m_dataKeys.itemReferenceText;
        case 13:
            return m_dataKeys.itemResponseTextFormat;
        case 14:
            return m_dataKeys.itemResponseEscapeCharacter;
        case 15:
            return m_dataKeys.itemResponsePrefix;
        case 16:
            return m_dataKeys.itemResponseSuffix;
        case 17:
            return m_dataKeys.itemResponseCrcEnable;
        case 18:
            return m_dataKeys.itemResponseCrcBigEndian;
        case 19:
            return m_dataKeys.itemResponseCrcAlgorithm;
        case 20:
            return m_dataKeys.itemResponseCrcStartIndex;
        case 21:
            return m_dataKeys.itemResponseCrcEndIndex;
        case 22:
            return m_dataKeys.itemResponseDelay;
        case 23:
            return m_dataKeys.itemResponseText;
        default:
            return "";
        }
    }

    return QVariant("");
}

QVariant xToolsResponserTool::columnDisplayRoleData(const ResponserData &item, int column) const
{
    if (column >= 0 && column < headers().count()) {
        const QString dataKey = headers().at(column);
        if (dataKey == m_dataKeys.itemEnable) {
            return item.data.itemEnable;
        } else if (dataKey == m_dataKeys.itemDescription) {
            return item.data.itemDescription;
        } else if (dataKey == m_dataKeys.itemOption) {
            return item.data.itemOption;
        } else if (dataKey == m_dataKeys.itemReferenceTextFormat) {
            return item.data.itemReferenceEscapeCharacter;
        } else if (dataKey == m_dataKeys.itemReferenceEscapeCharacter) {
            return item.data.itemReferenceEscapeCharacter;
        } else if (dataKey == m_dataKeys.itemReferencePrefix) {
            return item.data.itemReferencePrefix;
        } else if (dataKey == m_dataKeys.itemReferenceSuffix) {
            return item.data.itemReferenceSuffix;
        } else if (dataKey == m_dataKeys.itemReferenceCrcEnable) {
            return item.data.itemReferenceCrcEnable;
        } else if (dataKey == m_dataKeys.itemReferenceCrcBigEndian) {
            return item.data.itemReferenceCrcBigEndian;
        } else if (dataKey == m_dataKeys.itemReferenceCrcAlgorithm) {
            return item.data.itemReferenceCrcAlgorithm;
        } else if (dataKey == m_dataKeys.itemReferenceCrcStartIndex) {
            return item.data.itemReferenceCrcStartIndex;
        } else if (dataKey == m_dataKeys.itemReferenceCrcEndIndex) {
            return item.data.itemReferenceCrcEndIndex;
        } else if (dataKey == m_dataKeys.itemReferenceText) {
            return item.data.itemReferenceText;
        } else if (dataKey == m_dataKeys.itemResponseTextFormat) {
            return item.data.itemResponseEscapeCharacter;
        } else if (dataKey == m_dataKeys.itemResponseEscapeCharacter) {
            return item.data.itemResponseEscapeCharacter;
        } else if (dataKey == m_dataKeys.itemResponsePrefix) {
            return item.data.itemResponsePrefix;
        } else if (dataKey == m_dataKeys.itemResponseSuffix) {
            return item.data.itemResponseSuffix;
        } else if (dataKey == m_dataKeys.itemResponseCrcEnable) {
            return item.data.itemResponseCrcEnable;
        } else if (dataKey == m_dataKeys.itemResponseCrcBigEndian) {
            return item.data.itemResponseCrcBigEndian;
        } else if (dataKey == m_dataKeys.itemResponseCrcAlgorithm) {
            return item.data.itemResponseCrcAlgorithm;
        } else if (dataKey == m_dataKeys.itemResponseCrcStartIndex) {
            return item.data.itemResponseCrcStartIndex;
        } else if (dataKey == m_dataKeys.itemResponseCrcEndIndex) {
            return item.data.itemResponseCrcEndIndex;
        } else if (dataKey == m_dataKeys.itemResponseDelay) {
            return item.data.itemResponseDelay;
        } else if (dataKey == m_dataKeys.itemResponseText) {
            return item.data.itemResponseText;
        } else {
            return "Error";
        }
    }

    return QVariant("Error");
}

QByteArray xToolsResponserTool::referenceBytes(const ResponserItem &item) const
{
    QByteArray bytes;
    QString text = item.itemReferenceText;
    text = xToolsDataStructure::cookEscapeCharacter(item.itemReferenceEscapeCharacter, text);
    bytes = xToolsDataStructure::stringToByteArray(text, item.itemReferenceTextFormat);
    QByteArray prefix = xToolsDataStructure::affixesData(item.itemReferencePrefix);
    QByteArray suffix = xToolsDataStructure::affixesData(item.itemReferenceSuffix);

    bytes.prepend(prefix);
    if (item.itemReferenceCrcEnable) {
        xToolsCrcInterface sakCrc;
        QByteArray crcBytes = sakCrc.calculateBytes(bytes,
                                                    item.itemReferenceCrcAlgorithm,
                                                    item.itemReferenceCrcStartIndex,
                                                    item.itemReferenceCrcEndIndex,
                                                    item.itemReferenceCrcBigEndian);
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

QByteArray xToolsResponserTool::responseBytes(const ResponserItem &item) const
{
    QByteArray bytes;
    QString text = item.itemResponseText;
    text = xToolsDataStructure::cookEscapeCharacter(item.itemResponseEscapeCharacter, text);
    bytes = xToolsDataStructure::stringToByteArray(text, item.itemResponseTextFormat);
    QByteArray prefix = xToolsDataStructure::affixesData(item.itemResponsePrefix);
    QByteArray suffix = xToolsDataStructure::affixesData(item.itemResponseSuffix);

    bytes.prepend(prefix);
    if (item.itemResponseCrcEnable) {
        xToolsCrcInterface sakCrc;
        QByteArray crcBytes = sakCrc.calculateBytes(bytes,
                                                    item.itemResponseCrcAlgorithm,
                                                    item.itemResponseCrcStartIndex,
                                                    item.itemResponseCrcEndIndex,
                                                    item.itemResponseCrcBigEndian);
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

QVariant xToolsResponserTool::itemContext(int index)
{
    auto itemCtx = [=](int index) -> QJsonObject {
        QJsonObject ctx;
        if (index >= 0 && index < m_iItems.count()) {
            auto item = m_iItems.at(index);
            ctx.insert(itemEnable(), item.data.itemEnable);
            ctx.insert(itemDescription(), item.data.itemDescription);
            ctx.insert(itemOption(), item.data.itemOption);

            ctx.insert(itemReferenceTextFormat(), item.data.itemReferenceTextFormat);
            ctx.insert(itemReferenceEscapeCharacter(), item.data.itemReferenceEscapeCharacter);
            ctx.insert(itemReferencePrefix(), item.data.itemReferencePrefix);
            ctx.insert(itemReferenceSuffix(), item.data.itemReferenceSuffix);
            ctx.insert(itemReferenceCrcEnable(), item.data.itemReferenceCrcEnable);
            ctx.insert(itemReferenceCrcBigEndian(), item.data.itemReferenceCrcBigEndian);
            ctx.insert(itemReferenceCrcAlgorithm(), item.data.itemReferenceCrcAlgorithm);
            ctx.insert(itemReferenceCrcStartIndex(), item.data.itemReferenceCrcStartIndex);
            ctx.insert(itemReferenceCrcEndIndex(), item.data.itemReferenceCrcEndIndex);
            ctx.insert(itemReferenceText(), item.data.itemReferenceText);

            ctx.insert(itemResponseTextFormat(), item.data.itemResponseTextFormat);
            ctx.insert(itemResponseEscapeCharacter(), item.data.itemResponseEscapeCharacter);
            ctx.insert(itemResponsePrefix(), item.data.itemResponsePrefix);
            ctx.insert(itemResponseSuffix(), item.data.itemResponseSuffix);
            ctx.insert(itemResponseCrcEnable(), item.data.itemResponseCrcEnable);
            ctx.insert(itemResponseCrcBigEndian(), item.data.itemResponseCrcBigEndian);
            ctx.insert(itemResponseCrcAlgorithm(), item.data.itemResponseCrcAlgorithm);
            ctx.insert(itemResponseCrcStartIndex(), item.data.itemResponseCrcStartIndex);
            ctx.insert(itemResponseCrcEndIndex(), item.data.itemResponseCrcEndIndex);
            ctx.insert(itemResponseDelay(), item.data.itemResponseDelay);
            ctx.insert(itemResponseText(), item.data.itemResponseText);
        } else {
            // If index is out of range, return the default values.
            ctx.insert(itemEnable(), true);
            ctx.insert(itemDescription(), "Demo");
            ctx.insert(itemOption(), 0);

            ctx.insert(itemReferenceTextFormat(), xToolsDataStructure::TextFormatAscii);
            ctx.insert(itemReferenceEscapeCharacter(), xToolsDataStructure::EscapeCharacterNone);
            ctx.insert(itemReferencePrefix(), xToolsDataStructure::AffixesNone);
            ctx.insert(itemReferenceSuffix(), xToolsDataStructure::AffixesNone);
            ctx.insert(itemReferenceCrcEnable(), false);
            ctx.insert(itemReferenceCrcBigEndian(), false);
            ctx.insert(itemReferenceCrcAlgorithm(), xToolsCrcInterface::CRC_8);
            ctx.insert(itemReferenceCrcStartIndex(), 0);
            ctx.insert(itemReferenceCrcEndIndex(), 0);
            ctx.insert(itemReferenceText(), "Reference data.");

            ctx.insert(itemResponseTextFormat(), xToolsDataStructure::TextFormatAscii);
            ctx.insert(itemResponseEscapeCharacter(), xToolsDataStructure::EscapeCharacterNone);
            ctx.insert(itemResponsePrefix(), xToolsDataStructure::AffixesNone);
            ctx.insert(itemResponseSuffix(), xToolsDataStructure::AffixesNone);
            ctx.insert(itemResponseCrcEnable(), false);
            ctx.insert(itemResponseCrcBigEndian(), false);
            ctx.insert(itemResponseCrcAlgorithm(), xToolsCrcInterface::CRC_8);
            ctx.insert(itemResponseCrcStartIndex(), 0);
            ctx.insert(itemResponseCrcEndIndex(), 0);
            ctx.insert(itemResponseDelay(), 0);
            ctx.insert(itemResponseText(), "Response data.");
        }

        return ctx;
    };

    m_itemsMutex.lock();
    QJsonObject ctx = itemCtx(index);
    m_itemsMutex.unlock();

    return ctx;
}

QString xToolsResponserTool::cookHeaderString(const QString &str)
{
    ResponserItemKeys keys;
    if (str == keys.itemEnable) {
        return tr("Enable");
    } else if (str == keys.itemDescription) {
        return tr("Description");
    } else if (str == keys.itemOption) {
        return tr("Option");
    } else if (str == keys.itemReferenceTextFormat) {
        return tr("RefFormat");
    } else if (str == keys.itemReferenceEscapeCharacter) {
        return tr("RefEscape");
    } else if (str == keys.itemReferencePrefix) {
        return tr("RefPrefix");
    } else if (str == keys.itemReferenceSuffix) {
        return tr("RefSuffix");
    } else if (str == keys.itemReferenceCrcEnable) {
        return tr("RefCrcEnable");
    } else if (str == keys.itemReferenceCrcBigEndian) {
        return tr("RefCrcBigEndian");
    } else if (str == keys.itemReferenceCrcAlgorithm) {
        return tr("RefAlgorithm");
    } else if (str == keys.itemReferenceCrcStartIndex) {
        return tr("RefStart");
    } else if (str == keys.itemReferenceCrcEndIndex) {
        return tr("RefEnd");
    } else if (str == keys.itemReferenceText) {
        return tr("ReferenceData");
    } else if (str == keys.itemResponseTextFormat) {
        return tr("ResFormat");
    } else if (str == keys.itemResponseEscapeCharacter) {
        return tr("Escape");
    } else if (str == keys.itemResponsePrefix) {
        return tr("ResPrefix");
    } else if (str == keys.itemResponseSuffix) {
        return tr("ResSuffix");
    } else if (str == keys.itemResponseCrcEnable) {
        return tr("ResCrcEnable");
    } else if (str == keys.itemResponseCrcBigEndian) {
        return tr("ResCrcBigEndian");
    } else if (str == keys.itemResponseCrcAlgorithm) {
        return tr("ResAlgorithm");
    } else if (str == keys.itemResponseCrcStartIndex) {
        return tr("ResStart");
    } else if (str == keys.itemResponseCrcEndIndex) {
        return tr("ResEnd");
    } else if (str == keys.itemResponseDelay) {
        return tr("ResDelay");
    } else if (str == keys.itemResponseText) {
        return tr("ResponseData");
    }

    return "--";
}

void xToolsResponserTool::inputBytes(const QByteArray &bytes)
{
    m_inputBytesListMutex.lock();
    m_inputBytesList.append(bytes);
    m_inputBytesListMutex.unlock();
}

void xToolsResponserTool::run()
{
    QTimer *outputTimer = new QTimer();
    outputTimer->setInterval(5);
    outputTimer->setSingleShot(true);

    connect(outputTimer, &QTimer::timeout, outputTimer, [=]() {
        m_inputBytesListMutex.lock();
        while (!m_inputBytesList.isEmpty()) {
            auto bytes = m_inputBytesList.takeFirst();
            try2output(bytes, outputTimer);
        }
        m_inputBytesListMutex.unlock();
        outputTimer->start();
    });

    outputTimer->start();
    exec();

    m_inputBytesListMutex.lock();
    m_inputBytesList.clear();
    m_inputBytesListMutex.unlock();

    outputTimer->deleteLater();
    outputTimer = nullptr;
}

void xToolsResponserTool::try2output(const QByteArray &bytes, QObject *receiver)
{
    m_itemsMutex.lock();
    auto items = m_iItems;
    m_itemsMutex.unlock();

    int always = xToolsDataStructure::ResponseOptionAlways;
    int echo = xToolsDataStructure::ResponseOptionEcho;
    int contain = xToolsDataStructure::ResponseOptionInputContainReference;
    int discontain = xToolsDataStructure::ResponseOptionInputDiscontainReference;
    int eaual = xToolsDataStructure::ResponseOptionInputEqualReference;

    for (const auto &item : items) {
        if (!item.data.itemEnable) {
            continue;
        }

        auto refBytes = referenceBytes(item.data);
        auto resBytes = responseBytes(item.data);
#if 0
        qDebug() << QString::fromLatin1(ctx.bytes.toHex())
                 << QString::fromLatin1(refBytes.toHex())
                 << QString::fromLatin1(resBytes.toHex());
#endif
        bool enableResponse = false;
        if (item.data.itemOption == always) {
            enableResponse = true;
        } else if (item.data.itemOption == echo) {
            resBytes = bytes;
            enableResponse = true;
        } else if (item.data.itemOption == contain) {
            enableResponse = (bytes.contains(refBytes));
        } else if (item.data.itemOption == discontain) {
            enableResponse = (!bytes.contains(refBytes));
        } else if (item.data.itemOption == eaual) {
            enableResponse = (bytes == refBytes);
        }

        if (!enableResponse) {
            continue;
        }

        QTimer::singleShot(item.data.itemResponseDelay, receiver, [=]() {
            emit outputBytes(responseBytes(item.data));
        });
    }
}

QString xToolsResponserTool::itemEnable()
{
    return m_dataKeys.itemEnable;
}

QString xToolsResponserTool::itemDescription()
{
    return m_dataKeys.itemDescription;
}

QString xToolsResponserTool::itemOption()
{
    return m_dataKeys.itemOption;
}

QString xToolsResponserTool::itemReferenceTextFormat()
{
    return m_dataKeys.itemReferenceTextFormat;
}

QString xToolsResponserTool::itemReferenceEscapeCharacter()
{
    return m_dataKeys.itemReferenceEscapeCharacter;
}

QString xToolsResponserTool::itemReferencePrefix()
{
    return m_dataKeys.itemReferencePrefix;
}

QString xToolsResponserTool::itemReferenceSuffix()
{
    return m_dataKeys.itemReferenceSuffix;
}

QString xToolsResponserTool::itemReferenceCrcEnable()
{
    return m_dataKeys.itemReferenceCrcEnable;
}

QString xToolsResponserTool::itemReferenceCrcBigEndian()
{
    return m_dataKeys.itemReferenceCrcBigEndian;
}

QString xToolsResponserTool::itemReferenceCrcAlgorithm()
{
    return m_dataKeys.itemReferenceCrcAlgorithm;
}

QString xToolsResponserTool::itemReferenceCrcStartIndex()
{
    return m_dataKeys.itemReferenceCrcStartIndex;
}

QString xToolsResponserTool::itemReferenceCrcEndIndex()
{
    return m_dataKeys.itemReferenceCrcEndIndex;
}

QString xToolsResponserTool::itemReferenceText()
{
    return m_dataKeys.itemReferenceText;
}

QString xToolsResponserTool::itemResponseTextFormat()
{
    return m_dataKeys.itemResponseTextFormat;
}

QString xToolsResponserTool::itemResponseEscapeCharacter()
{
    return m_dataKeys.itemResponseEscapeCharacter;
}

QString xToolsResponserTool::itemResponsePrefix()
{
    return m_dataKeys.itemResponsePrefix;
}

QString xToolsResponserTool::itemResponseSuffix()
{
    return m_dataKeys.itemResponseSuffix;
}

QString xToolsResponserTool::itemResponseCrcEnable()
{
    return m_dataKeys.itemResponseCrcEnable;
}

QString xToolsResponserTool::itemResponseCrcBigEndian()
{
    return m_dataKeys.itemResponseCrcBigEndian;
}

QString xToolsResponserTool::itemResponseCrcAlgorithm()
{
    return m_dataKeys.itemResponseCrcAlgorithm;
}

QString xToolsResponserTool::itemResponseCrcStartIndex()
{
    return m_dataKeys.itemResponseCrcStartIndex;
}

QString xToolsResponserTool::itemResponseCrcEndIndex()
{
    return m_dataKeys.itemResponseCrcEndIndex;
}

QString xToolsResponserTool::itemResponseDelay()
{
    return m_dataKeys.itemResponseDelay;
}

QString xToolsResponserTool::itemResponseText()
{
    return m_dataKeys.itemResponseText;
}
