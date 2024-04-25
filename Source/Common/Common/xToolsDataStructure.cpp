/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsDataStructure.h"

#include <QLineEdit>
#include <QMap>
#include <QMetaEnum>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStandardItemModel>

#include "xToolsCompatibility.h"

xToolsDataStructure::xToolsDataStructure(QObject *parent)
    : QObject(parent)
{}

xToolsDataStructure::~xToolsDataStructure() {}

QVariantList xToolsDataStructure::supportedTextFormats()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<TextFormat>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}

QString xToolsDataStructure::textFormatName(int textFormat)
{
    static QMap<int, QString> textFormatMap;
    if (textFormatMap.isEmpty()) {
        textFormatMap.insert(TextFormatBin, QString("BIN"));
        textFormatMap.insert(TextFormatOct, QString("OCT"));
        textFormatMap.insert(TextFormatDec, QString("DEC"));
        textFormatMap.insert(TextFormatHex, QString("HEX"));
        textFormatMap.insert(TextFormatAscii, QString("ASCII"));
        textFormatMap.insert(TextFormatUtf8, QString("UTF8"));
        textFormatMap.insert(TextFormatSystem, QString("SYSTEM"));
    }

    return textFormatMap.value(textFormat, "Unknown");
}

QString xToolsDataStructure::byteArrayToString(const QByteArray &array, int format)
{
    auto cookedArray = [](const QByteArray &array, int base, int len) -> QString {
        QString str, numStr;
        for (int i = 0; i < array.length(); i++) {
            if (base == 10 || base == 8) {
                numStr = QString::number(array.at(i), base);
                str.append(QString("%1 ").arg(numStr));
            } else {
                numStr = QString::number(quint8(array.at(i)), base);
                str.append(QString("%1 ").arg(numStr, len, '0'));
            }
        }
        return str;
    };

    if (xToolsDataStructure::TextFormatBin == format) {
        return cookedArray(array, 2, 8);
    } else if (xToolsDataStructure::TextFormatOct == format) {
        return cookedArray(array, 8, 3);
    } else if (xToolsDataStructure::TextFormatDec == format) {
        return cookedArray(array, 10, 3);
    } else if (xToolsDataStructure::TextFormatHex == format) {
        return cookedArray(array, 16, 2);
    } else if (xToolsDataStructure::TextFormatAscii == format) {
        return QString::fromLatin1(array);
    } else if (xToolsDataStructure::TextFormatUtf8) {
        return QString::fromUtf8(array);
    } else if (xToolsDataStructure::TextFormatSystem) {
        return QString::fromLocal8Bit(array);
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unsupported text format");
        return QString("Unsupported text format: %1").arg(format);
    }
}

QByteArray xToolsDataStructure::stringToByteArray(const QString &str, int format)
{
    auto cookString = [](const QString &str, int base) -> QByteArray {
        QByteArray data;
        QStringList strList = str.split(' ', xToolsSkipEmptyParts);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, base);
            data.append(reinterpret_cast<char *>(&value), 1);
        }

        return data;
    };

    QByteArray data;
    if (format == xToolsDataStructure::TextFormatBin) {
        data = cookString(str, 2);
    } else if (format == xToolsDataStructure::TextFormatOct) {
        data = cookString(str, 8);
    } else if (format == xToolsDataStructure::TextFormatDec) {
        data = cookString(str, 10);
    } else if (format == xToolsDataStructure::TextFormatHex) {
        data = cookString(str, 16);
    } else if (format == xToolsDataStructure::TextFormatAscii) {
        data = str.toLatin1();
    } else if (format == xToolsDataStructure::TextFormatUtf8) {
        data = str.toUtf8();
    } else if (format == xToolsDataStructure::TextFormatSystem) {
        data = str.toLocal8Bit();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unsupported text format");
        data = str.toLocal8Bit();
    }

    return data;
}

QString xToolsDataStructure::formatString(const QString &str, int format)
{
    static QRegularExpression binRegularExpression("[^0-1]");
    static QRegularExpression octRegularExpression("[^0-7]");
    static QRegularExpression decRegularExpression("[^0-9]");
    static QRegularExpression hexRegularExpression("[^0-9a-fA-F]");
    static QRegularExpression asciiRegularExpression("[^ -~]");

    auto cookString = [](const QString &str, int length, QRegularExpression &re) -> QString {
        QString rawString = str;
        QString cookedString;
        rawString.remove(re);
        for (int i = 0; i < rawString.length(); i++) {
            if ((i != 0) && (i % length == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(rawString.at(i));
        }

        return cookedString;
    };

    if (format == xToolsDataStructure::TextFormatBin) {
        return cookString(str, 8, binRegularExpression);
    } else if (format == xToolsDataStructure::TextFormatOct) {
        return cookString(str, 3, octRegularExpression);
    } else if (format == xToolsDataStructure::TextFormatDec) {
        return cookString(str, 3, decRegularExpression);
    } else if (format == xToolsDataStructure::TextFormatHex) {
        return cookString(str, 3, hexRegularExpression);
    } else if (format == xToolsDataStructure::TextFormatAscii) {
        return QString(str).remove(asciiRegularExpression);
    } else if (format == xToolsDataStructure::TextFormatUtf8) {
        return QString::fromUtf8(str.toUtf8());
    } else if (format == xToolsDataStructure::TextFormatSystem) {
        return QString::fromUtf8(str.toLocal8Bit());
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input model!");
        return QString::fromUtf8(str.toLocal8Bit());
    }
}

QVariantList xToolsDataStructure::supportedEscapeCharacters()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<EscapeCharacter>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}

QString xToolsDataStructure::escapeCharacterName(int escapeCharacter)
{
    static QMap<int, QString> escapeCharacterMap;
    if (escapeCharacterMap.isEmpty()) {
        escapeCharacterMap.insert(EscapeCharacterNone, tr("None"));
        escapeCharacterMap.insert(EscapeCharacterR, QString("\\r"));
        escapeCharacterMap.insert(EscapeCharacterN, QString("\\n"));
        escapeCharacterMap.insert(EscapeCharacterRN, QString("\\r\\n"));
        escapeCharacterMap.insert(EscapeCharacterNR, QString("\\n\\r"));
        escapeCharacterMap.insert(EscapeCharacterRAndN, tr("\\r and \\n"));
    }

    return escapeCharacterMap.value(escapeCharacter, "Unknown");
}

QString xToolsDataStructure::cookEscapeCharacter(int escapeCharacter, const QString &str)
{
    QString newStr = str;
    if (escapeCharacter == EscapeCharacterR) {
        newStr.replace("\\r", "\r");
    } else if (escapeCharacter == EscapeCharacterN) {
        newStr.replace("\\n", "\n");
    } else if (escapeCharacter == EscapeCharacterRN) {
        newStr.replace("\\r\\n", "\r\n");
    } else if (escapeCharacter == EscapeCharacterNR) {
        newStr.replace("\\n\\r", "\n\\r");
    } else if (escapeCharacter == EscapeCharacterRAndN) {
        newStr.replace("\\r", "\r");
        newStr.replace("\\n", "\n");
    }

    return newStr;
}

QString xToolsDataStructure::affixesString(int affixes)
{
    if (affixes == AffixesN) {
        return QString("\n");
    } else if (affixes == AffixesR) {
        return QString("\r");
    } else if (affixes == AffixesNR) {
        return QString("\n\r");
    } else if (affixes == AffixesRN) {
        return QString("\r\n");
    } else {
        return QString("");
    }
}

QString xToolsDataStructure::affixesName(int affixes)
{
    if (xToolsDataStructure::AffixesNone == affixes) {
        return "None";
    } else if (xToolsDataStructure::AffixesR == affixes) {
        return "\\r";
    } else if (xToolsDataStructure::AffixesN == affixes) {
        return "\\n";
    } else if (xToolsDataStructure::AffixesRN == affixes) {
        return "\\r\\n";
    } else if (xToolsDataStructure::AffixesNR == affixes) {
        return "\\n\\r";
    }

    return "None";
}

QByteArray xToolsDataStructure::affixesData(int affixes)
{
    if (affixes == xToolsDataStructure::AffixesNone) {
        return QByteArray("");
    } else if (affixes == xToolsDataStructure::AffixesR) {
        return QByteArray("\r");
    } else if (affixes == xToolsDataStructure::AffixesN) {
        return QByteArray("\n");
    } else if (affixes == xToolsDataStructure::AffixesRN) {
        return QByteArray("\r\n");
    } else if (affixes == xToolsDataStructure::AffixesNR) {
        return QByteArray("\n\r");
    }

    return QByteArray("");
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
QVariantList xToolsDataStructure::supportedHighDpiPolicies()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<HighDpiPolicy>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
QString xToolsDataStructure::highDpiPolicyName(int policy)
{
    static QMap<int, QString> policyMap;
    if (policyMap.isEmpty()) {
        policyMap.insert(HighDpiPolicyRound, tr("Round up for .5 and above"));
        policyMap.insert(HighDpiPolicyCeil, tr("Always round up"));
        policyMap.insert(HighDpiPolicyFloor, tr("Always round down"));
        policyMap.insert(HighDpiPolicyRoundPreferFloor, tr("Round up for .75 and above"));
        policyMap.insert(HighDpiPolicyPassThrough, tr("Don't round"));
#ifdef Q_OS_WIN
        policyMap.insert(HighDpiPolicySystem, tr("System"));
#endif
    }

    return policyMap.value(policy, "Unknown");
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
bool xToolsDataStructure::isValidHighDpiPolicy(int policy)
{
    auto policies = supportedHighDpiPolicies();
    return policies.contains(QVariant(policy));
}
#endif

QVariantList xToolsDataStructure::supportedWebSocketSendingTypes()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<WebSocketSendingType>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}

QString xToolsDataStructure::webSocketSendingTypeName(int type)
{
    static QMap<int, QString> typeMap;
    if (typeMap.isEmpty()) {
        typeMap.insert(WebSocketSendingTypeText, tr("Text"));
        typeMap.insert(WebSocketSendingTypeBinary, tr("Binary"));
    }

    return typeMap.value(type, "Unknown");
}

QVariantList xToolsDataStructure::supportedResponseOptions()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<ResponseOption>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}

QString xToolsDataStructure::responseOptionName(int option)
{
    static QMap<int, QString> optionMap;
    if (optionMap.isEmpty()) {
        // clang-format off
        optionMap.insert(ResponseOptionDisable, tr("Disable"));
        optionMap.insert(ResponseOptionEcho, tr("Echo"));
        optionMap.insert(ResponseOptionAlways, tr("Data Received"));
        optionMap.insert(ResponseOptionInputEqualReference, tr("Rx Data Equal Reference Data"));
        optionMap.insert(ResponseOptionInputContainReference, tr("Rx Data Contain Reference Data"));
        optionMap.insert(ResponseOptionInputDiscontainReference, tr("Rx Data Discontain Reference Data"));
        // clang-format on
    }

    return optionMap.value(option, "Unknown");
}
