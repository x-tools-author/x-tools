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
        cookString(str, 2);
    } else if (format == xToolsDataStructure::TextFormatOct) {
        cookString(str, 8);
    } else if (format == xToolsDataStructure::TextFormatDec) {
        cookString(str, 10);
    } else if (format == xToolsDataStructure::TextFormatHex) {
        cookString(str, 16);
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

QString xToolsDataStructure::cookedString(int escapeCharacter, const QString &str)
{
    return xToolsDataStructure::cookEscapeCharacter(escapeCharacter, str);
}

QString xToolsDataStructure::cookEscapeCharacter(int option, const QString &str)
{
    QString newStr = str;
    if (option == EscapeCharacterR) {
        newStr.replace("\\r", "\r");
    } else if (option == EscapeCharacterN) {
        newStr.replace("\\n", "\n");
    } else if (option == EscapeCharacterRN) {
        newStr.replace("\\r\\n", "\r\n");
    } else if (option == EscapeCharacterNR) {
        newStr.replace("\\n\\r", "\n\\r");
    } else if (option == EscapeCharacterRAndN) {
        newStr.replace("\\r", "\r");
        newStr.replace("\\n", "\n");
    }

    return newStr;
}

#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
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

#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
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

#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
bool xToolsDataStructure::isValidHighDpiPolicy(int policy)
{
    auto policies = supportedHighDpiPolicies();
    return policies.contains(QVariant(policy));
}
#endif

QString xToolsDataStructure::cookedAffixes(int affixes)
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
        return "//r";
    } else if (xToolsDataStructure::AffixesN == affixes) {
        return "//n";
    } else if (xToolsDataStructure::AffixesRN == affixes) {
        return "//r//n";
    } else if (xToolsDataStructure::AffixesNR == affixes) {
        return "//n//r";
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

void xToolsDataStructure::setComboBoxTextOutputFormat(QComboBox *comboBox)
{
    if (comboBox) {
        QMap<int, QString> formatMap;
        formatMap.insert(TextFormatBin, QString("BIN"));
        formatMap.insert(TextFormatOct, QString("OCT"));
        formatMap.insert(TextFormatDec, QString("DEC"));
        formatMap.insert(TextFormatHex, QString("HEX"));
        formatMap.insert(TextFormatUtf8, QString("UTF8"));
        formatMap.insert(TextFormatAscii, QString("ASCII"));
        formatMap.insert(TextFormatSystem, QString("SYSTEM"));
        setComboBoxItems(comboBox, formatMap, TextFormatHex);
    }
}

void xToolsDataStructure::setComboBoxTextInputFormat(QComboBox *comboBox)
{
    if (comboBox) {
        if (comboBox) {
            QMap<int, QString> formatMap;
            formatMap.insert(TextFormatBin, QString("BIN"));
            formatMap.insert(TextFormatOct, QString("OTC"));
            formatMap.insert(TextFormatDec, QString("DEC"));
            formatMap.insert(TextFormatHex, QString("HEX"));
            formatMap.insert(TextFormatAscii, QString("ASCII"));
            formatMap.insert(TextFormatSystem, QString("SYSTEM"));
            setComboBoxItems(comboBox, formatMap, TextFormatSystem);
        }
    }
}

void xToolsDataStructure::setComboBoxTextWebSocketSendingType(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->addItem(tr("BIN"), xToolsDataStructure::WebSocketSendingTypeBin);
        comboBox->addItem(tr("TEXT"), xToolsDataStructure::WebSocketSendingTypeText);
    }
}

void xToolsDataStructure::setLineEditTextFormat(QLineEdit *lineEdit, TextFormat format)
{
    QMap<int, QRegularExpressionValidator *> regExpMap;
    QRegularExpression binRegExp = QRegularExpression("([01][01][01][01][01][01][01][01][ ])*");
    QRegularExpression otcRegExp = QRegularExpression("([0-7][0-7][ ])*");
    QRegularExpression decRegExp = QRegularExpression("([0-9][0-9][ ])*");
    QRegularExpression hexRegExp = QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QRegularExpression asciiRegExp = QRegularExpression("([ -~])*");
    regExpMap.insert(xToolsDataStructure::TextFormatBin, new QRegularExpressionValidator(binRegExp));
    regExpMap.insert(xToolsDataStructure::TextFormatOct, new QRegularExpressionValidator(otcRegExp));
    regExpMap.insert(xToolsDataStructure::TextFormatDec, new QRegularExpressionValidator(decRegExp));
    regExpMap.insert(xToolsDataStructure::TextFormatHex, new QRegularExpressionValidator(hexRegExp));
    regExpMap.insert(xToolsDataStructure::TextFormatAscii,
                     new QRegularExpressionValidator(asciiRegExp));
    regExpMap.insert(xToolsDataStructure::TextFormatSystem, Q_NULLPTR);

    if (lineEdit) {
        if (lineEdit->validator()) {
            delete lineEdit->validator();
            lineEdit->setValidator(Q_NULLPTR);
        }

        if (regExpMap.contains(format)) {
            QRegularExpressionValidator *validator = regExpMap.value(format);
            lineEdit->setValidator(validator);
        } else {
            lineEdit->setValidator(Q_NULLPTR);
        }
    }
}

void xToolsDataStructure::setLineEditTextFormat(QLineEdit *lineEdit, int format)
{
    auto cookedFormat = static_cast<TextFormat>(format);
    xToolsDataStructure::setLineEditTextFormat(lineEdit, cookedFormat);
}

QString xToolsDataStructure::suffix(SAKEmnuSuffixType type)
{
    return suffix(int(type));
}

QString xToolsDataStructure::suffix(int type)
{
    switch (type) {
    case SuffixsTypeNone:
        return "";
    case SuffixsTypeR:
        return "\r";
    case SuffixsTypeN:
        return "\n";
    case SuffixsTypeRN:
        return "\r\n";
    case SuffixsTypeNR:
        return "\n\r";
    default:
        return "";
    }
}

QString xToolsDataStructure::friendlySuffix(SAKEmnuSuffixType type)
{
    switch (type) {
    case SuffixsTypeNone:
        return tr("None");
    case SuffixsTypeR:
        return "\\r";
    case SuffixsTypeN:
        return "\\n";
    case SuffixsTypeRN:
        return "\\r\\n";
    case SuffixsTypeNR:
        return "\\n\\r";
    default:
        return tr("None");
    }
}

QString xToolsDataStructure::prefix(int type)
{
    switch (type) {
    case PrefixTypeNone:
        return "";
    case PrefixTypeR:
        return "\r";
    case PrefixTypeN:
        return "\n";
    case PrefixTypeRN:
        return "\r\n";
    case PrefixTypeNR:
        return "\n\r";
    default:
        return "";
    }
}

QString xToolsDataStructure::friendlyPrefix(SAKEnumPrefixType type)
{
    switch (type) {
    case PrefixTypeNone:
        return "";
    case PrefixTypeR:
        return "\\r";
    case PrefixTypeN:
        return "\\n";
    case PrefixTypeRN:
        return "\\r\\n";
    case PrefixTypeNR:
        return "\\n\\r";
    default:
        return "";
    }
}

void xToolsDataStructure::setupSuffix(QComboBox *comboBox)
{
    if (comboBox) {
        QMap<int, QString> formatMap;
        formatMap.insert(SuffixsTypeNone, friendlySuffix(SuffixsTypeNone));
        formatMap.insert(SuffixsTypeR, friendlySuffix(SuffixsTypeR));
        formatMap.insert(SuffixsTypeN, friendlySuffix(SuffixsTypeN));
        formatMap.insert(SuffixsTypeRN, friendlySuffix(SuffixsTypeRN));
        formatMap.insert(SuffixsTypeNR, friendlySuffix(SuffixsTypeNR));
        setComboBoxItems(comboBox, formatMap, SuffixsTypeNone);
    }
}

void xToolsDataStructure::formattingInputText(QTextEdit *textEdit, int model)
{
    if (textEdit) {
        textEdit->blockSignals(true);
        QString plaintext = textEdit->toPlainText();
        if (!plaintext.isEmpty()) {
            auto cookedModel = static_cast<xToolsDataStructure::TextFormat>(model);
            QString cookedString = xToolsDataStructure::formatString(plaintext, cookedModel);
            textEdit->setText(cookedString);
            textEdit->moveCursor(QTextCursor::End);
        }
        textEdit->blockSignals(false);
    }
}

void xToolsDataStructure::setComboBoxItems(QComboBox *comboBox,
                                        QMap<int, QString> &formatMap,
                                        int currentData)
{
    if (comboBox) {
        comboBox->clear();
        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentIndex(currentData);

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }

        // Try to set the current index.
        for (int i = 0; i < comboBox->count(); i++) {
            if (comboBox->itemData(i).toInt() == currentData) {
                comboBox->setCurrentIndex(i);
            }
        }
    }
}
