/***************************************************************************************************
 * Copyright 2018-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakdatastructure.h"

#include <QLineEdit>
#include <QMetaEnum>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStandardItemModel>

#include "sakcrcinterface.h"
#include "sakinterface.h"

SAKDataStructure::SAKDataStructure(QObject *parent)
    : QObject(parent)
{}

SAKDataStructure::~SAKDataStructure() {}

void SAKDataStructure::setComboBoxTextOutputFormat(QComboBox *comboBox)
{
    if (comboBox) {
        QMap<int, QString> formatMap;
        formatMap.insert(OutputFormatBin, QString("BIN"));
        formatMap.insert(OutputFormatOct, QString("OCT"));
        formatMap.insert(OutputFormatDec, QString("DEC"));
        formatMap.insert(OutputFormatHex, QString("HEX"));
        formatMap.insert(OutputFormatUtf8, QString("UTF8"));
        formatMap.insert(OutputFormatUcs4, QString("UCS4"));
        formatMap.insert(OutputFormatAscii, QString("ASCII"));
        formatMap.insert(OutputFormatUtf16, QString("UTF16"));
        formatMap.insert(OutputFormatLocal, QString("SYSTEM"));
        setComboBoxItems(comboBox, formatMap, OutputFormatHex);
    }
}

void SAKDataStructure::setComboBoxTextInputFormat(QComboBox *comboBox)
{
    if (comboBox) {
        if (comboBox) {
            QMap<int, QString> formatMap;
            formatMap.insert(InputFormatBin, QString("BIN"));
            formatMap.insert(InputFormatOct, QString("OTC"));
            formatMap.insert(InputFormatDec, QString("DEC"));
            formatMap.insert(InputFormatHex, QString("HEX"));
            formatMap.insert(InputFormatAscii, QString("ASCII"));
            formatMap.insert(InputFormatLocal, QString("SYSTEM"));
            setComboBoxItems(comboBox, formatMap, InputFormatLocal);
        }
    }
}

void SAKDataStructure::setComboBoxTextWebSocketSendingType(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->addItem(tr("BIN"), SAKDataStructure::WebSocketSendingTypeBin);
        comboBox->addItem(tr("TEXT"), SAKDataStructure::WebSocketSendingTypeText);
    }
}

QString SAKDataStructure::formattingString(QString &origingString, SAKEnumTextFormatInput format)
{
    QString cookedString;
    if (format == SAKDataStructure::InputFormatBin) {
        origingString.remove(QRegularExpression("[^0-1]"));
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 8 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == SAKDataStructure::InputFormatOct) {
        origingString.remove(QRegularExpression("[^0-7]"));
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 2 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == SAKDataStructure::InputFormatDec) {
        origingString.remove(QRegularExpression("[^0-9]"));
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 2 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == SAKDataStructure::InputFormatHex) {
        origingString.remove(QRegularExpression("[^0-9a-fA-F]"));
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 2 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == SAKDataStructure::InputFormatAscii) {
        for (int i = 0; i < origingString.length(); i++) {
            if (origingString.at(i).unicode() <= 127) {
                cookedString.append(origingString.at(i));
            }
        }
    } else if (format == SAKDataStructure::InputFormatLocal) {
        cookedString = origingString;
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input model!");
    }

    return cookedString;
}

QByteArray SAKDataStructure::stringToByteArray(QString &origingString, SAKEnumTextFormatInput format)
{
    QByteArray data;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    auto behavior = QString::SkipEmptyParts;
#else
    auto behavior = Qt::SkipEmptyParts;
#endif
    if (format == SAKDataStructure::InputFormatBin) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 2);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == SAKDataStructure::InputFormatOct) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 8);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == SAKDataStructure::InputFormatDec) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 10);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == SAKDataStructure::InputFormatHex) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 16);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == SAKDataStructure::InputFormatAscii) {
        data = origingString.toLatin1();
    } else if (format == SAKDataStructure::InputFormatLocal) {
        data = origingString.toLocal8Bit();
    } else {
        data = origingString.toUtf8();
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input mode!");
    }

    return data;
}

QByteArray SAKDataStructure::stringToByteArray(QString &origingString, int format)
{
    auto cookedFormat = static_cast<SAKDataStructure::SAKEnumTextFormatInput>(format);
    return stringToByteArray(origingString, cookedFormat);
}

QString SAKDataStructure::byteArrayToString(const QByteArray &origingData,
                                            SAKEnumTextFormatOutput format)
{
    QString str;
    if (format == SAKDataStructure::OutputFormatBin) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 2),
                                   8,
                                   '0'));
        }
    } else if (format == SAKDataStructure::OutputFormatOct) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 8),
                                   3,
                                   '0'));
        }
    } else if (format == SAKDataStructure::OutputFormatDec) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 10)));
        }
    } else if (format == SAKDataStructure::OutputFormatHex) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 16),
                                   2,
                                   '0'));
        }
    } else if (format == SAKDataStructure::OutputFormatAscii) {
        str.append(QString::fromLatin1(origingData));
    } else if (format == SAKDataStructure::OutputFormatUtf8) {
        str.append(QString::fromUtf8(origingData));
    } else if (format == SAKDataStructure::OutputFormatUtf16) {
        str.append(QString::fromUtf16(reinterpret_cast<const char16_t *>(origingData.constData()),
                                      origingData.length() / sizeof(char16_t)));
    } else if (format == SAKDataStructure::OutputFormatUcs4) {
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t *>(origingData.constData()),
                                     origingData.length() / sizeof(char32_t)));
    } else if (format == SAKDataStructure::OutputFormatLocal) {
        str.append(QString::fromLocal8Bit(origingData));
    } else {
        str.append(QString::fromUtf8(origingData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknown output mode!");
    }

    return str;
}

QString SAKDataStructure::byteArrayToString(const QByteArray &origingData, int format)
{
    auto cookedFormat = static_cast<SAKDataStructure::SAKEnumTextFormatOutput>(format);
    return byteArrayToString(origingData, cookedFormat);
}

void SAKDataStructure::setLineEditTextFormat(QLineEdit *lineEdit, SAKEnumTextFormatInput format)
{
    QMap<int, QRegularExpressionValidator *> regExpMap;
    QRegularExpression binRegExp = QRegularExpression("([01][01][01][01][01][01][01][01][ ])*");
    QRegularExpression otcRegExp = QRegularExpression("([0-7][0-7][ ])*");
    QRegularExpression decRegExp = QRegularExpression("([0-9][0-9][ ])*");
    QRegularExpression hexRegExp = QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QRegularExpression asciiRegExp = QRegularExpression("([ -~])*");
    regExpMap.insert(SAKDataStructure::InputFormatBin, new QRegularExpressionValidator(binRegExp));
    regExpMap.insert(SAKDataStructure::InputFormatOct, new QRegularExpressionValidator(otcRegExp));
    regExpMap.insert(SAKDataStructure::InputFormatDec, new QRegularExpressionValidator(decRegExp));
    regExpMap.insert(SAKDataStructure::InputFormatHex, new QRegularExpressionValidator(hexRegExp));
    regExpMap.insert(SAKDataStructure::InputFormatAscii,
                     new QRegularExpressionValidator(asciiRegExp));
    regExpMap.insert(SAKDataStructure::InputFormatLocal, Q_NULLPTR);

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

void SAKDataStructure::setLineEditTextFormat(QLineEdit *lineEdit, int format)
{
    auto cookedFormat = static_cast<SAKEnumTextFormatInput>(format);
    SAKDataStructure::setLineEditTextFormat(lineEdit, cookedFormat);
}

QString SAKDataStructure::suffix(SAKEmnuSuffixType type)
{
    return suffix(int(type));
}

QString SAKDataStructure::suffix(int type)
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

QString SAKDataStructure::friendlySuffix(SAKEmnuSuffixType type)
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

QString SAKDataStructure::prefix(int type)
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

QString SAKDataStructure::friendlyPrefix(SAKEnumPrefixType type)
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

void SAKDataStructure::setupSuffix(QComboBox *comboBox)
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

void SAKDataStructure::formattingInputText(QTextEdit *textEdit, int model)
{
    if (textEdit) {
        textEdit->blockSignals(true);
        QString plaintext = textEdit->toPlainText();
        if (!plaintext.isEmpty()) {
            auto cookedModel = static_cast<SAKDataStructure::SAKEnumTextFormatInput>(model);
            QString cookedString = SAKDataStructure::formattingString(plaintext, cookedModel);
            textEdit->setText(cookedString);
            textEdit->moveCursor(QTextCursor::End);
        }
        textEdit->blockSignals(false);
    }
}

void SAKDataStructure::setComboBoxItems(QComboBox *comboBox,
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

QString SAKDataStructure::affixesName(int affixes)
{
    if (SAKDataStructure::AffixesNone == affixes) {
        return "None";
    } else if (SAKDataStructure::AffixesR == affixes) {
        return "//r";
    } else if (SAKDataStructure::AffixesN == affixes) {
        return "//n";
    } else if (SAKDataStructure::AffixesRN == affixes) {
        return "//r//n";
    } else if (SAKDataStructure::AffixesNR == affixes) {
        return "//n//r";
    }

    return "None";
}

QByteArray SAKDataStructure::affixesData(int affixes)
{
    if (affixes == SAKDataStructure::AffixesNone) {
        return QByteArray("");
    } else if (affixes == SAKDataStructure::AffixesR) {
        return QByteArray("\r");
    } else if (affixes == SAKDataStructure::AffixesN) {
        return QByteArray("\n");
    } else if (affixes == SAKDataStructure::AffixesRN) {
        return QByteArray("\r\n");
    } else if (affixes == SAKDataStructure::AffixesNR) {
        return QByteArray("\n\r");
    }

    return QByteArray("");
}

QString SAKDataStructure::cookedString(int escapeCharacter, const QString &str)
{
    return SAKDataStructure::cookEscapeCharacter(escapeCharacter, str);
}

QByteArray SAKDataStructure::dataItemBytes(const EDStructDataItem &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = SAKDataStructure::cookedString(item.itemTextEscapeChracter, text);
    bytes = SAKInterface::string2array(text, item.itemTextFormat);
    SAKCrcInterface sakCrc;
    QByteArray crcBytes = sakCrc.calculateBytes(bytes,
                                                item.itemCrcAlgorithm,
                                                item.itemCrcStartIndex,
                                                item.itemCrcEndIndex);
    QByteArray prefix = SAKDataStructure::affixesData(item.itemPrefix);
    QByteArray suffix = SAKDataStructure::affixesData(item.itemSuffix);

    bytes.prepend(prefix);
    bytes.append(crcBytes);
    bytes.append(suffix);

    return bytes;
}

QString SAKDataStructure::textFormatName(int textFormat)
{
    static QMap<int, QString> map;
    if (map.isEmpty()) {
        map.insert(TextFormatBin, tr("Bin"));
        map.insert(TextFormatOct, tr("Oct"));
        map.insert(TextFormatDec, tr("Dec"));
        map.insert(TextFormatHex, tr("Hex"));
        map.insert(TextFormatAscii, "Ascii");
        map.insert(TextFormatUtf8, "Utf8");
    }

    if (map.contains(textFormat)) {
        return map.value(textFormat);
    }

    return QString("Unknown");
}

QString SAKDataStructure::cookEscapeCharacter(int option, const QString &str)
{
    QString newStr = str;
    if (option == EscapeCharacterOptionR) {
        newStr.replace("\\r", "\r");
    } else if (option == EscapeCharacterOptionN) {
        newStr.replace("\\n", "\n");
    } else if (option == EscapeCharacterOptionRN) {
        newStr.replace("\\r\\n", "\r\n");
    } else if (option == EscapeCharacterOptionNR) {
        newStr.replace("\\n\\r", "\n\\r");
    } else if (option == EscapeCharacterOptionRAndN) {
        newStr.replace("\\r", "\r");
        newStr.replace("\\n", "\n");
    }

    return newStr;
}
