/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Responser.h"

#include <QFile>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>

#include "../../xIO.h"

namespace xTools {

Responser::Responser(QObject *parent)
    : AbstractModel{parent}
{}

int Responser::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_iItems.count();
}

int Responser::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tableColumnCount;
}

QVariant Responser::data(const QModelIndex &index, int role) const
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

bool Responser::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);

    return true;
}

bool Responser::insertRows(int row, int count, const QModelIndex &parent)
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

bool Responser::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    m_iItems.remove(row, count);
    return true;
}

QVariant Responser::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant Responser::columnDisplayRoleData(const ResponserData &item, int column) const
{
    return QVariant("Error");
}

QByteArray Responser::referenceBytes(const ResponserItem &item) const
{
    QByteArray bytes;
    QString text = item.itemReferenceText;
    auto escapeCharacter = static_cast<xIO::EscapeCharacter>(item.itemReferenceEscapeCharacter);
    text = xIO::cookedEscapeCharacter(text, escapeCharacter);
    bytes = xIO::string2bytes(text, static_cast<xIO::TextFormat>(item.itemReferenceTextFormat));
    QByteArray prefix = xIO::cookedAffixes(static_cast<xIO::Affixes>(item.itemReferencePrefix));
    QByteArray suffix = xIO::cookedAffixes(static_cast<xIO::Affixes>(item.itemReferenceSuffix));

    bytes.prepend(prefix);
    if (item.itemReferenceCrcEnable) {
        auto algorithm = static_cast<xIO::CrcAlgorithm>(item.itemReferenceCrcAlgorithm);
        QByteArray crcBytes = xIO::calculateCrc(bytes,
                                                algorithm,
                                                item.itemReferenceCrcStartIndex,
                                                item.itemReferenceCrcEndIndex,
                                                item.itemReferenceCrcBigEndian);
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

QByteArray Responser::responseBytes(const ResponserItem &item) const
{
    QByteArray bytes;
    QString text = item.itemResponseText;
    auto escapeCharacter = static_cast<xIO::EscapeCharacter>(item.itemResponseEscapeCharacter);
    text = xIO::cookedEscapeCharacter(text, escapeCharacter);
    bytes = xIO::string2bytes(text, static_cast<xIO::TextFormat>(item.itemResponseTextFormat));
    QByteArray prefix = xIO::cookedAffixes(static_cast<xIO::Affixes>(item.itemResponsePrefix));
    QByteArray suffix = xIO::cookedAffixes(static_cast<xIO::Affixes>(item.itemResponseSuffix));

    bytes.prepend(prefix);
    if (item.itemResponseCrcEnable) {
        auto algorithm = static_cast<xIO::CrcAlgorithm>(item.itemResponseCrcAlgorithm);
        QByteArray crcBytes = xIO::calculateCrc(bytes,
                                                algorithm,
                                                item.itemResponseCrcStartIndex,
                                                item.itemResponseCrcEndIndex,
                                                item.itemResponseCrcBigEndian);
        bytes.append(crcBytes);
    }
    bytes.append(suffix);

    return bytes;
}

void Responser::inputBytes(const QByteArray &bytes)
{
    m_inputBytesListMutex.lock();
    m_inputBytesList.append(bytes);
    m_inputBytesListMutex.unlock();
}

void Responser::run()
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

void Responser::try2output(const QByteArray &bytes, QObject *receiver)
{
    m_itemsMutex.lock();
    auto items = m_iItems;
    m_itemsMutex.unlock();

    int always = static_cast<int>(xIO::ResponseOption::Always);
    int echo = static_cast<int>(xIO::ResponseOption::Echo);
    int contain = static_cast<int>(xIO::ResponseOption::InputContainReference);
    int discontain = static_cast<int>(xIO::ResponseOption::InputDiscontainReference);
    int eaual = static_cast<int>(xIO::ResponseOption::InputEqualReference);

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

QString Responser::itemEnable()
{
    return m_dataKeys.itemEnable;
}

QString Responser::itemDescription()
{
    return m_dataKeys.itemDescription;
}

QString Responser::itemOption()
{
    return m_dataKeys.itemOption;
}

QString Responser::itemReferenceTextFormat()
{
    return m_dataKeys.itemReferenceTextFormat;
}

QString Responser::itemReferenceEscapeCharacter()
{
    return m_dataKeys.itemReferenceEscapeCharacter;
}

QString Responser::itemReferencePrefix()
{
    return m_dataKeys.itemReferencePrefix;
}

QString Responser::itemReferenceSuffix()
{
    return m_dataKeys.itemReferenceSuffix;
}

QString Responser::itemReferenceCrcEnable()
{
    return m_dataKeys.itemReferenceCrcEnable;
}

QString Responser::itemReferenceCrcBigEndian()
{
    return m_dataKeys.itemReferenceCrcBigEndian;
}

QString Responser::itemReferenceCrcAlgorithm()
{
    return m_dataKeys.itemReferenceCrcAlgorithm;
}

QString Responser::itemReferenceCrcStartIndex()
{
    return m_dataKeys.itemReferenceCrcStartIndex;
}

QString Responser::itemReferenceCrcEndIndex()
{
    return m_dataKeys.itemReferenceCrcEndIndex;
}

QString Responser::itemReferenceText()
{
    return m_dataKeys.itemReferenceText;
}

QString Responser::itemResponseTextFormat()
{
    return m_dataKeys.itemResponseTextFormat;
}

QString Responser::itemResponseEscapeCharacter()
{
    return m_dataKeys.itemResponseEscapeCharacter;
}

QString Responser::itemResponsePrefix()
{
    return m_dataKeys.itemResponsePrefix;
}

QString Responser::itemResponseSuffix()
{
    return m_dataKeys.itemResponseSuffix;
}

QString Responser::itemResponseCrcEnable()
{
    return m_dataKeys.itemResponseCrcEnable;
}

QString Responser::itemResponseCrcBigEndian()
{
    return m_dataKeys.itemResponseCrcBigEndian;
}

QString Responser::itemResponseCrcAlgorithm()
{
    return m_dataKeys.itemResponseCrcAlgorithm;
}

QString Responser::itemResponseCrcStartIndex()
{
    return m_dataKeys.itemResponseCrcStartIndex;
}

QString Responser::itemResponseCrcEndIndex()
{
    return m_dataKeys.itemResponseCrcEndIndex;
}

QString Responser::itemResponseDelay()
{
    return m_dataKeys.itemResponseDelay;
}

QString Responser::itemResponseText()
{
    return m_dataKeys.itemResponseText;
}

} // namespace xTools
