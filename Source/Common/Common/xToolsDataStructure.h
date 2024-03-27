/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QMap>
#include <QObject>
#include <QTextEdit>

class xToolsDataStructure : public QObject
{
    Q_OBJECT
public:
    xToolsDataStructure(QObject *parent = Q_NULLPTR);
    ~xToolsDataStructure();

    enum SAKEnumTextFormat {
        TextFormatBin,
        TextFormatOct,
        TextFormatDec,
        TextFormatHex,
        TextFormatAscii,
        TextFormatUtf8,
        TextFormatSystem
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

    enum SAKEnumAffixes { AffixesNone, AffixesR, AffixesN, AffixesRN, AffixesNR };
    Q_ENUM(SAKEnumAffixes)
    Q_INVOKABLE static QString cookedAffixes(int affixes);

    enum EDEnumResponseOptions {
        ResponseOptionDisable,
        ResponseOptionEcho,
        ResponseOptionAlways,
        ResponseOptionInputEqualReference,
        ResponseOptionInputContainReference,
        ResponseOptionInputDiscontainReference
    };
    Q_ENUM(EDEnumResponseOptions)

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    enum SAKHdpiPolicy {
        HdpiPolicyRound = int(Qt::HighDpiScaleFactorRoundingPolicy::Round),
        HdpiPolicyCeil = int(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
        HdpiPolicyFloor = int(Qt::HighDpiScaleFactorRoundingPolicy::Floor),
        HdpiPolicyRoundPreferFloor = int(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor),
        HdpiPolicyPassThrough = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough),
        HdpiPolicySystem = 999
    };
    Q_ENUM(SAKHdpiPolicy)
#endif

public:
    static QString affixesName(int affixes);
    static QByteArray affixesData(int affixes);
    static QString cookedString(int escapeCharacter, const QString &str);
    static QString textFormatName(int textFormat);
    Q_INVOKABLE static QString cookEscapeCharacter(int option, const QString &str);

    // Input text format
    enum SAKEnumTextFormatInput {
        InputFormatBin,
        InputFormatOct,
        InputFormatDec,
        InputFormatHex,
        InputFormatAscii,
        InputFormatLocal
    };
    Q_ENUM(SAKEnumTextFormatInput);

    // Output text format
    enum SAKEnumTextFormatOutput {
        OutputFormatBin,
        OutputFormatOct,
        OutputFormatDec,
        OutputFormatHex,
        OutputFormatUcs4,
        OutputFormatUtf8,
        OutputFormatUtf16,
        OutputFormatAscii,
        OutputFormatLocal
    };
    Q_ENUM(SAKEnumTextFormatOutput);

    // Web socket sending type
    enum SAKEnumWebSocketSendingType {
        WebSocketSendingTypeText,
        WebSocketSendingTypeBin,
    };
    Q_ENUM(SAKEnumWebSocketSendingType);

    enum SAKEmnuSuffixType {
        SuffixsTypeNone,
        SuffixsTypeR,
        SuffixsTypeN,
        SuffixsTypeRN,
        SuffixsTypeNR
    };
    Q_ENUM(SAKEmnuSuffixType);

    enum SAKEnumPrefixType { PrefixTypeNone, PrefixTypeR, PrefixTypeN, PrefixTypeRN, PrefixTypeNR };
    Q_ENUM(SAKEnumPrefixType);

public:
    /**
     * @brief setComboBoxTextOutputFormat: Add output text format items to combo box.
     * @param comboBox: Targat combo box.
     */
    static void setComboBoxTextOutputFormat(QComboBox *comboBox);

    /**
     * @brief setComboBoxTextInputFormat: Add input text format items to combo Box.
     * @param comboBox: Target combo box.
     */
    static void setComboBoxTextInputFormat(QComboBox *comboBox);

    /**
     * @brief setComboBoxTextWebSocketSendingType: Add supported types to combo box.
     * @param comboBox: Target combo box.
     */
    static void setComboBoxTextWebSocketSendingType(QComboBox *comboBox);

    /**
     * @brief formattingString: Formatting input text of text edit.
     * @param textEdit: Target text edit.
     * @param format: See SAKEnumTextInputFormat for more information.
     */
    static QString formattingString(QString &origingString, SAKEnumTextFormatInput format);

    /**
     * @brief stringToByteArray: Transmit a QString to a QByteArray.
     * @param origingString: Origin string.
     * @param format: See SAKEnumTextInputFormat for more information.
     * @return A QByteArray.
     */
    static QByteArray stringToByteArray(QString &origingString, SAKEnumTextFormatInput format);
    static QByteArray stringToByteArray(QString &origingString, int format);

    /**
     * @brief byteArrayToString: Transmit a QByteArray to a QString.
     * @param origingString: Origin byte array.
     * @param format: See SAKEnumTextOutputFormat for more information.
     * @return A QString.
     */
    static QString byteArrayToString(const QByteArray &origingData, SAKEnumTextFormatOutput format);
    static QString byteArrayToString(const QByteArray &origingData, int format);

    /**
     * @brief setLineEditTextFormat: Formating input
     * @param lineEdit: Target component
     * @param format: (SAKEnumTextInputFormat)
     */
    static void setLineEditTextFormat(QLineEdit *lineEdit, SAKEnumTextFormatInput format);
    static void setLineEditTextFormat(QLineEdit *lineEdit, int format);

    static QString suffix(SAKEmnuSuffixType type);
    static QString suffix(int type);
    static QString friendlySuffix(SAKEmnuSuffixType type);
    static QString prefix(int type);
    static QString friendlyPrefix(SAKEnumPrefixType type);
    static void setupSuffix(QComboBox *comboBox);

    static void formattingInputText(QTextEdit *textEdit, int model);

private:
    static void setComboBoxItems(QComboBox *comboBox,
                                 QMap<int, QString> &formatMap,
                                 int currentData);
};
