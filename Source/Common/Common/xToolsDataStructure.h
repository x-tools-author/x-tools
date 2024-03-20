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

#ifdef SAK_IMPORT_MODULE_SERIALPORT
#include <QSerialPort>
#endif

#ifdef SAK_IMPORT_MODULE_BLE
#ifdef SAK_IMPORT_MODULE_BLE_CENTRAL
#include <QBluetoothDeviceInfo>
#endif
#endif

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

    enum SAKEnumPalette { PaletteSystem, PaletteLight, PaletteDark, PaletteCustom };
    Q_ENUM(SAKEnumPalette)
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

    struct EDStructDataItem
    {
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

#ifdef SAK_IMPORT_MODULE_TEST
    struct SAKStructTestParametersContext
    {
        bool openFailed;
        bool readCircularly;
        int readInterval;
        bool writeCircularly;
        int writtingInterval;
        QString errorString;
        QString rxData;
        QString txData;
    };
#endif

#ifdef SAK_IMPORT_MODULE_SERIALPORT
    struct SAKStructSerialPortParametersContext
    {
        QString portName;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;

        int frameIntervel;
    };
#endif

#ifdef SAK_IMPORT_MODULE_UDP
#ifdef SAK_IMPORT_MODULE_UDP_CLIENT
    struct SAKStructUdpClientParametersContext
    {
        QString peerHost;
        quint16 peerPort;
        QString localHost;
        quint16 localPort;
        bool specifyLocalInfo;
        bool pauseSending;
    };
    struct SAKStructUdpServerParametersContext
    {
        QString serverHost;
        quint16 serverPort;

        QString currentClientHost;
        quint16 currentClientPort;
        QStringList clients;
    };
#endif
#endif

#ifdef SAK_IMPORT_MODULE_TCP
#ifdef SAK_IMPORT_MODULE_TCP_CLIENT
    struct SAKStructTcpClientParametersContext
    {
        QString localHost;
        quint16 localPort;
        QString serverHost;
        quint16 serverPort;
        bool specifyClientAddressAndPort;
        bool allowAutomaticConnection;
    };
#endif
#ifdef SAK_IMPORT_MODULE_TCP_SERVER
    struct SAKStructTcpServerParametersContext
    {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
    };
#endif
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_CLIENT
    struct SAKStructWSClientParametersContext
    {
        QString serverAddress;
        quint32 sendingType;
    };
#endif
#ifdef SAK_IMPORT_MODULE_WEBSOCKET_SERVER
    struct SAKStructWSServerParametersContext
    {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
        quint32 sendingType;
    };
#endif
#endif
#ifdef SAK_IMPORT_MODULE_BLE
#ifdef SAK_IMPORT_MODULE_BLE_CENTRAL
    struct SAKStructBleCentralParametersContext
    {
        QBluetoothDeviceInfo info;
        QString uuid;
    };
#endif
#endif
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
