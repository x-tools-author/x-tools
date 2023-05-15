/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKDATASTRUCTURE_HH
#define SAKDATASTRUCTURE_HH

#include <QObject>

class SAKDataStructure : public QObject
{
    Q_OBJECT
public:
    explicit SAKDataStructure(QObject *parent = nullptr);

    enum SAKEnumTextFormat {
        TextFormatBin,
        TextFormatOct,
        TextFormatDec,
        TextFormatHex,
        TextFormatAscii,
        TextFormatUtf8
    };
    Q_ENUM(SAKEnumTextFormat)

    enum SAKEnumEscapeCharacterOption {
        EscapeCharacterOptionNone,
        EscapeCharacterOptionR,
        EscapeCharacterOptionN,
        EscapeCharacterOptionRN,
        EscapeCharacterOptionNR,
        EscapeCharacterOptionRAndN
    };
    Q_ENUM(SAKEnumEscapeCharacterOption)

    enum SAKEnumAffixes {
        AffixesNone,
        AffixesR,
        AffixesN,
        AffixesRN,
        AffixesNR,
        AffixesNAndR
    };
    Q_ENUM(SAKEnumAffixes)

    /**
     * @brief 自动应答选项
     */
    enum EDEnumResponseOptions {
        ResponseOptionDisable,                  /// 禁止应答
        ResponseOptionEcho,                     /// 回显，接收到数据时总是发送接收到的数据
        ResponseOptionAways,                    /// 接收到数据时总是发送应答数据
        ResponseOptionInputEqualReference,      /// 接收到的数据等于参考数据时，发送应答数据
        ResponseOptionInputContainReference,    /// 接收到的数据包含参考数据时，发送应答数据
        ResponseOptionInputDiscontainReference  /// 接收到的数据不包含参考数据时，发送应答数据
    };
    Q_ENUM(EDEnumResponseOptions)

    struct EDStructDataItem {
        int itemTextFormat;
        int itemTextEscapeChracter;
        QString itemText;
        int itemPrefix;
        int itemSuffix;

        bool itemCrcEnable;
        int itemCrcAlgorithm;
        int itemCrcStartIndex;
        int itemCrcEndIndex;
    };

public:
    static QString affixesName(int affixes);
    static QByteArray affixesData(int affixes);
    static QString cookedString(int escapeCharacter, const QString &str);
    static QByteArray dataItemBytes(const EDStructDataItem &item);
    Q_INVOKABLE static QString cookEscapeCharacter(int option, const QString &str);
};

#endif // SAKDATASTRUCTURE_H
