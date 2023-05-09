/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDCrc.hpp"
#include "EDInterface.hpp"
#include "EDDataStructure.hpp"

EDDataStructure::EDDataStructure(QObject *parent)
    : QObject{parent}
{

}

QString EDDataStructure::affixesName(int affixes)
{
    if (EDDataStructure::AffixesNone) {
        return "None";
    } else if (EDDataStructure::AffixesR) {
        return "//r";
    } else if (EDDataStructure::AffixesN) {
        return "//n";
    } else if (EDDataStructure::AffixesRN) {
        return "//r//n";
    } else if (EDDataStructure::AffixesNR) {
        return "//n//r";
    }

    return "None";
}

QByteArray EDDataStructure::affixesData(int affixes)
{
    if (affixes == EDDataStructure::AffixesNone) {
        return QByteArray("");
    } else if (affixes == EDDataStructure::AffixesR) {
        return QByteArray("\r");
    } else if (affixes == EDDataStructure::AffixesN) {
        return QByteArray("\n");
    } else if (affixes == EDDataStructure::AffixesRN) {
        return QByteArray("\r\n");
    } else if (affixes == EDDataStructure::AffixesNR) {
        return QByteArray("\n\r");
    }

    return QByteArray("");
}


QString EDDataStructure::cookedString(int escapeCharacter, const QString &str)
{
    return EDDataStructure::cookEscapeCharacter(escapeCharacter, str);
}

QByteArray EDDataStructure::dataItemBytes(const EDStructDataItem &item)
{
    QByteArray bytes;
    QString text = item.itemText;
    text = EDDataStructure::cookedString(item.itemTextEscapeChracter, text);
    bytes = EDInterface::string2array(text, item.itemTextFormat);
    EDCrc edCrc;
    QByteArray crcBytes = edCrc.calculateBytes(bytes,
                                               item.itemCrcAlgorithm,
                                               item.itemCrcStartIndex,
                                               item.itemCrcEndIndex);
    QByteArray prefix = EDDataStructure::affixesData(item.itemPrefix);
    QByteArray suffix = EDDataStructure::affixesData(item.itemSuffix);

    bytes.prepend(prefix);
    bytes.append(crcBytes);
    bytes.append(suffix);

    return bytes;
}

QString EDDataStructure::cookEscapeCharacter(int option, const QString &str)
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

