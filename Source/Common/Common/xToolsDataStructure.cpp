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
#include <QMetaEnum>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStandardItemModel>

xToolsDataStructure::xToolsDataStructure(QObject *parent)
    : QObject(parent)
{}

xToolsDataStructure::~xToolsDataStructure() {}

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

void xToolsDataStructure::setComboBoxTextOutputFormat(QComboBox *comboBox)
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

void xToolsDataStructure::setComboBoxTextInputFormat(QComboBox *comboBox)
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

void xToolsDataStructure::setComboBoxTextWebSocketSendingType(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->addItem(tr("BIN"), xToolsDataStructure::WebSocketSendingTypeBin);
        comboBox->addItem(tr("TEXT"), xToolsDataStructure::WebSocketSendingTypeText);
    }
}

QString xToolsDataStructure::formattingString(QString &origingString, SAKEnumTextFormatInput format)
{
    QString cookedString;
    if (format == xToolsDataStructure::InputFormatBin) {
        static auto tmp = QRegularExpression("[^0-1]");
        origingString.remove(tmp);
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 8 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == xToolsDataStructure::InputFormatOct) {
        static auto tmp = QRegularExpression("[^0-7]");
        origingString.remove(tmp);
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 2 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == xToolsDataStructure::InputFormatDec) {
        static auto tmp = QRegularExpression("[^0-9]");
        origingString.remove(tmp);
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 2 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == xToolsDataStructure::InputFormatHex) {
        static auto tmp = QRegularExpression("[^0-9a-fA-F]");
        origingString.remove(tmp);
        for (int i = 0; i < origingString.length(); i++) {
            if ((i != 0) && (i % 2 == 0)) {
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    } else if (format == xToolsDataStructure::InputFormatAscii) {
        for (int i = 0; i < origingString.length(); i++) {
            if (origingString.at(i).unicode() <= 127) {
                cookedString.append(origingString.at(i));
            }
        }
    } else if (format == xToolsDataStructure::InputFormatLocal) {
        cookedString = origingString;
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input model!");
    }

    return cookedString;
}

QByteArray xToolsDataStructure::stringToByteArray(QString &origingString, SAKEnumTextFormatInput format)
{
    QByteArray data;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    auto behavior = QString::SkipEmptyParts;
#else
    auto behavior = Qt::SkipEmptyParts;
#endif
    if (format == xToolsDataStructure::InputFormatBin) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 2);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::InputFormatOct) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 8);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::InputFormatDec) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 10);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::InputFormatHex) {
        QStringList strList = origingString.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 16);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::InputFormatAscii) {
        data = origingString.toLatin1();
    } else if (format == xToolsDataStructure::InputFormatLocal) {
        data = origingString.toLocal8Bit();
    } else {
        data = origingString.toUtf8();
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input mode!");
    }

    return data;
}

QByteArray xToolsDataStructure::stringToByteArray(QString &origingString, int format)
{
    auto cookedFormat = static_cast<xToolsDataStructure::SAKEnumTextFormatInput>(format);
    return stringToByteArray(origingString, cookedFormat);
}

QString xToolsDataStructure::byteArrayToString(const QByteArray &origingData,
                                            SAKEnumTextFormatOutput format)
{
    QString str;
    if (format == xToolsDataStructure::OutputFormatBin) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 2),
                                   8,
                                   '0'));
        }
    } else if (format == xToolsDataStructure::OutputFormatOct) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 8),
                                   3,
                                   '0'));
        }
    } else if (format == xToolsDataStructure::OutputFormatDec) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 10)));
        }
    } else if (format == xToolsDataStructure::OutputFormatHex) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(
                QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 16),
                                   2,
                                   '0'));
        }
    } else if (format == xToolsDataStructure::OutputFormatAscii) {
        str.append(QString::fromLatin1(origingData));
    } else if (format == xToolsDataStructure::OutputFormatUtf8) {
        str.append(QString::fromUtf8(origingData));
    } else if (format == xToolsDataStructure::OutputFormatUtf16) {
        str.append(QString::fromUtf16(reinterpret_cast<const char16_t *>(origingData.constData()),
                                      origingData.length() / sizeof(char16_t)));
    } else if (format == xToolsDataStructure::OutputFormatUcs4) {
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t *>(origingData.constData()),
                                     origingData.length() / sizeof(char32_t)));
    } else if (format == xToolsDataStructure::OutputFormatLocal) {
        str.append(QString::fromLocal8Bit(origingData));
    } else {
        str.append(QString::fromUtf8(origingData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknown output mode!");
    }

    return str;
}

QString xToolsDataStructure::byteArrayToString(const QByteArray &origingData, int format)
{
    auto cookedFormat = static_cast<xToolsDataStructure::SAKEnumTextFormatOutput>(format);
    return byteArrayToString(origingData, cookedFormat);
}

void xToolsDataStructure::setLineEditTextFormat(QLineEdit *lineEdit, SAKEnumTextFormatInput format)
{
    QMap<int, QRegularExpressionValidator *> regExpMap;
    QRegularExpression binRegExp = QRegularExpression("([01][01][01][01][01][01][01][01][ ])*");
    QRegularExpression otcRegExp = QRegularExpression("([0-7][0-7][ ])*");
    QRegularExpression decRegExp = QRegularExpression("([0-9][0-9][ ])*");
    QRegularExpression hexRegExp = QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QRegularExpression asciiRegExp = QRegularExpression("([ -~])*");
    regExpMap.insert(xToolsDataStructure::InputFormatBin, new QRegularExpressionValidator(binRegExp));
    regExpMap.insert(xToolsDataStructure::InputFormatOct, new QRegularExpressionValidator(otcRegExp));
    regExpMap.insert(xToolsDataStructure::InputFormatDec, new QRegularExpressionValidator(decRegExp));
    regExpMap.insert(xToolsDataStructure::InputFormatHex, new QRegularExpressionValidator(hexRegExp));
    regExpMap.insert(xToolsDataStructure::InputFormatAscii,
                     new QRegularExpressionValidator(asciiRegExp));
    regExpMap.insert(xToolsDataStructure::InputFormatLocal, Q_NULLPTR);

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
    auto cookedFormat = static_cast<SAKEnumTextFormatInput>(format);
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
            auto cookedModel = static_cast<xToolsDataStructure::SAKEnumTextFormatInput>(model);
            QString cookedString = xToolsDataStructure::formattingString(plaintext, cookedModel);
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

QString xToolsDataStructure::cookedString(int escapeCharacter, const QString &str)
{
    return xToolsDataStructure::cookEscapeCharacter(escapeCharacter, str);
}

QString xToolsDataStructure::textFormatName(int textFormat)
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

QString xToolsDataStructure::cookEscapeCharacter(int option, const QString &str)
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
