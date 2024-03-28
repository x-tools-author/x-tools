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

public:
    enum TextFormat {
        TextFormatBin,
        TextFormatOct,
        TextFormatDec,
        TextFormatHex,
        TextFormatAscii,
        TextFormatUtf8,
        TextFormatSystem
    };
    Q_ENUM(TextFormat)
    Q_INVOKABLE static QVariantList supportedTextFormats();
    Q_INVOKABLE static QString textFormatName(int textFormat);
    Q_INVOKABLE static QString byteArrayToString(const QByteArray &array, int format);
    Q_INVOKABLE static QByteArray stringToByteArray(const QString &str, int format);
    Q_INVOKABLE static QString formatString(const QString &str, int format);

public:
    enum EscapeCharacter {
        EscapeCharacterNone,
        EscapeCharacterR,
        EscapeCharacterN,
        EscapeCharacterRN,
        EscapeCharacterNR,
        EscapeCharacterRAndN
    };
    Q_ENUM(EscapeCharacter)
    Q_INVOKABLE static QString cookedString(int escapeCharacter, const QString &str);
    Q_INVOKABLE static QString cookEscapeCharacter(int option, const QString &str);

public:
    enum SAKEnumAffixes { AffixesNone, AffixesR, AffixesN, AffixesRN, AffixesNR };
    Q_ENUM(SAKEnumAffixes)
    Q_INVOKABLE static QString cookedAffixes(int affixes);
    Q_INVOKABLE static QString affixesName(int affixes);
    Q_INVOKABLE static QByteArray affixesData(int affixes);

public:
#ifdef X_TOOLS_ENABLE_HIGH_DPI_POLICY
    enum HighDpiPolicy {
        HighDpiPolicyRound = int(Qt::HighDpiScaleFactorRoundingPolicy::Round),
        HighDpiPolicyCeil = int(Qt::HighDpiScaleFactorRoundingPolicy::Ceil),
        HighDpiPolicyFloor = int(Qt::HighDpiScaleFactorRoundingPolicy::Floor),
        HighDpiPolicyRoundPreferFloor = int(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor),
        HighDpiPolicyPassThrough = int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough),
#ifdef Q_OS_WIN
        HighDpiPolicySystem,
#endif
    };
    Q_ENUM(HighDpiPolicy)
    Q_INVOKABLE static QVariantList supportedHighDpiPolicies();
    Q_INVOKABLE static QString highDpiPolicyName(int policy);
    Q_INVOKABLE static bool isValidHighDpiPolicy(int policy);
#endif

public:
public:
    // Web socket sending type
    enum SAKEnumWebSocketSendingType {
        WebSocketSendingTypeText,
        WebSocketSendingTypeBin,
    };
    Q_ENUM(SAKEnumWebSocketSendingType);

public:
    enum SAKEmnuSuffixType {
        SuffixsTypeNone,
        SuffixsTypeR,
        SuffixsTypeN,
        SuffixsTypeRN,
        SuffixsTypeNR
    };
    Q_ENUM(SAKEmnuSuffixType);

public:
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
     * @brief setLineEditTextFormat: Formating input
     * @param lineEdit: Target component
     * @param format: (SAKEnumTextInputFormat)
     */
    static void setLineEditTextFormat(QLineEdit *lineEdit, TextFormat format);
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
