/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKCrcInterface.hh"
#include "SAKInterface.hh"
#include "SAKDataStructure.hh"

SAKDataStructure::SAKDataStructure(QObject *parent)
    : QObject{parent}
{

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

