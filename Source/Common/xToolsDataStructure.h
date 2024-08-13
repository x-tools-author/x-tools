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
    Q_INVOKABLE static QVariantList supportedEscapeCharacters();
    Q_INVOKABLE static QString escapeCharacterName(int escapeCharacter);
    Q_INVOKABLE static QString cookEscapeCharacter(int escapeCharacter, const QString &str);

public:
    enum SAKEnumAffixes { AffixesNone, AffixesR, AffixesN, AffixesRN, AffixesNR };
    Q_ENUM(SAKEnumAffixes)
    Q_INVOKABLE static QString affixesString(int affixes);
    Q_INVOKABLE static QString affixesName(int affixes);
    Q_INVOKABLE static QByteArray affixesData(int affixes);

public:
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
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
    // Web socket sending type
    enum WebSocketSendingType {
        WebSocketSendingTypeText,
        WebSocketSendingTypeBinary,
    };
    Q_ENUM(WebSocketSendingType);
    Q_INVOKABLE static QVariantList supportedWebSocketSendingTypes();
    Q_INVOKABLE static QString webSocketSendingTypeName(int type);

public:
    enum ResponseOption {
        ResponseOptionDisable,
        ResponseOptionEcho,   // Response the data that received.
        ResponseOptionAlways, // Response the data that set by user when data received.
        ResponseOptionInputEqualReference,
        ResponseOptionInputContainReference,
        ResponseOptionInputDiscontainReference
    };
    Q_ENUM(ResponseOption)
    Q_INVOKABLE static QVariantList supportedResponseOptions();
    Q_INVOKABLE static QString responseOptionName(int option);
};
