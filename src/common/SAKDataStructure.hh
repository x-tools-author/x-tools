/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDDATASTRUCTURE_HPP
#define EDDATASTRUCTURE_HPP

#include <QObject>

class EDDataStructure : public QObject
{
    Q_OBJECT
public:
    explicit EDDataStructure(QObject *parent = nullptr);

    enum EDEnumTextFormat {
        TextFormatBin,
        TextFormatOct,
        TextFormatDec,
        TextFormatHex,
        TextFormatAscii,
        TextFormatUtf8
    };
    Q_ENUM(EDEnumTextFormat);

    enum EDEnumEscapeCharacterOption {
        EscapeCharacterOptionNone,
        EscapeCharacterOptionR,
        EscapeCharacterOptionN,
        EscapeCharacterOptionRN,
        EscapeCharacterOptionNR,
        EscapeCharacterOptionRAndN
    };
    Q_ENUM(EDEnumEscapeCharacterOption);

    enum EDEnumAffixes {
        AffixesNone,
        AffixesR,
        AffixesN,
        AffixesRN,
        AffixesNR
    };
    Q_ENUM(EDEnumAffixes);

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
    Q_ENUM(EDEnumResponseOptions);

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

#endif // EDDATASTRUCTURE_HPP
