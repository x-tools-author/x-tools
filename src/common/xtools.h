/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QJsonObject>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>

// -------------------------------------------------------------------------------------------------
enum class TextFormat {
    Bin,
    Oct,
    Dec,
    Hex,
    Ascii,
    Utf8,
    GB2312,
    CSGB2312,
    GBK,
    GB18030,
    HexWithoutSpace
};
QList<int> xSupportedTextFormats();
QString xTextFormatName(TextFormat format);
void xSetupTextFormat(QComboBox *comboBox);
QString xBytes2string(const QByteArray &bytes, int format);
QByteArray xString2bytes(const QString &text, int format);
QByteArray xArrayAppendArray(const QByteArray &a1, const QByteArray &a2);
void xSetupTextFormatValidator(QLineEdit *lineEdit, int format, int maxLen = 32767);
void xPrintSupportedIconvEncodings();

// -------------------------------------------------------------------------------------------------
enum class Affixes { None, R, N, RN, NR };
QList<int> xSupportedAffixes();
QString xAdditionName(int affixes);
void xSetupAddition(QComboBox *comboBox);
QByteArray xCookedAffixes(int affixes);

// -------------------------------------------------------------------------------------------------
enum class EscapeCharacter { None, R, N, RN, NR, R_N, C };
QList<int> xSupportedEscapeCharacters();
QString xEscapeCharacterName(int character);
void xSetupEscapeCharacter(QComboBox *comboBox);
QString xCookedEscapeCharacter(const QString &text, int escapeCharacter);

// -------------------------------------------------------------------------------------------------
enum class WebSocketDataChannel { Text, Binary };
QString xWebSocketDataChannelName(WebSocketDataChannel channel);
void xSetupWebSocketDataChannel(QComboBox *comboBox);

// -------------------------------------------------------------------------------------------------
enum class ResponseOption {
    Echo,   // Response data is the data received.
    Always, // Response automatically when data received.
    InputEqualReference,
    InputContainReference,
    InputDoesNotContainReference
};
QList<int> xSupportedResponseOptions();
QString xResponseOptionName(ResponseOption option);
void xSetupResponseOption(QComboBox *comboBox);
QByteArray xResponseData(const QByteArray &data, int option, const QByteArray &reference);

// -------------------------------------------------------------------------------------------------
struct TextItem
{
    TextFormat textFormat;
    EscapeCharacter escapeCharacter;
    Affixes prefix;
    QString text;
    Affixes suffix;
    struct
    {
        bool enable;
        bool bigEndian;
        int algorithm;
        int startIndex;
        int endIndex;
    } crc;
};
struct TextItemKeys
{
    const QString textFormat{"textFormat"};
    const QString escapeCharacter{"escapeCharacter"};
    const QString prefix{"prefix"};
    const QString text{"text"};
    const QString suffix{"suffix"};
    const QString crcEnable{"crcEnable"};
    const QString crcBigEndian{"crcBigEndian"};
    const QString crcAlgorithm{"crcAlgorithm"};
    const QString crcStartIndex{"crcStartIndex"};
    const QString crcEndIndex{"crcEndIndex"};
};
TextItem xDefaultTextItem();
QString xTextItem2string(const TextItem &context);
QByteArray xTextItem2array(const TextItem &context);
TextItem xLoadTextItem(const QJsonObject &obj);
QJsonObject xSaveTextItem(const TextItem &context);

// -------------------------------------------------------------------------------------------------
struct SerialPortItem
{
    QString portName;
    qint32 baudRate;
    int dataBits;
    int parity;
    int stopBits;
    int flowControl;
    bool ignoredBusyDevices;
    bool optimizedFrame;
    int interFrameDelayMilliseconds;
};
struct SerialPortItemKeys
{
    const QString portName{"portName"};
    const QString baudRate{"baudRate"};
    const QString dataBits{"dataBits"};
    const QString stopBits{"stopBits"};
    const QString parity{"parity"};
    const QString flowControl{"flowControl"};
    const QString ignoredBusyDevices{"ignoredBusyDevices"};
    const QString optimizedFrame{"optimizedFrame"};
    const QString interFrameDelayMilliseconds{"interFrameDelayMilliseconds"};
};
SerialPortItem xDefaultSerialPortItem();
QJsonObject xSaveSerialPortItem(const SerialPortItem &context);
SerialPortItem xLoadSerialPortItem(const QJsonObject &obj);
#if X_ENABLE_SERIALPORT
void xSetupPortName(QComboBox *comboBox);
void xSetupBaudRate(QComboBox *comboBox);
void xSetupDataBits(QComboBox *comboBox);
void xSetupParity(QComboBox *comboBox);
void xSetupStopBits(QComboBox *comboBox);
void xSetupFlowControl(QComboBox *comboBox);
#endif

// -------------------------------------------------------------------------------------------------
struct SocketItem
{
    QString serverAddress;
    quint16 serverPort;
    WebSocketDataChannel dataChannel;
    bool authentication;
    QString username;
    QString password;
    QString multicastAddress;
    quint16 multicastPort;
    bool enableMulticast;
    bool justMulticast;
    QString path;    // For WebSocket
    bool secureMode; // For WebSocket
};
struct SocketItemKeys
{
    const QString serverAddress{"serverAddress"};
    const QString serverPort{"serverPort"};
    const QString dataChannel{"dataChannel"};
    const QString authentication{"authentication"};
    const QString username{"username"};
    const QString password{"password"};
    const QString multicastAddress{"multicastAddress"};
    const QString multicastPort{"multicastPort"};
    const QString enableMulticast{"enableMulticast"};
    const QString justMulticast{"justMulticast"};
    const QString path{"path"};             // For WebSocket
    const QString secureMode{"secureMode"}; // For WebSocket
};
SocketItem xDefaultSocketItem();
QVariantMap xSaveSocketItem(const SocketItem &context);
SocketItem xLoadSocketItem(const QVariantMap &obj);
void xSetupSocketAddress(QComboBox *cb);
void xSetupSocketPort(QSpinBox *spinBox);

// -------------------------------------------------------------------------------------------------
enum class TransferType { Disabled, Bidirectional, Unidirectional };
QList<int> xSupportedTransferTypes();
QString xTransferTypeName(int type);
void xSetupTransferType(QComboBox *comboBox);

// -------------------------------------------------------------------------------------------------
enum class ChartsDataFormat { BinaryY, BinaryXY, TextY, TextXY };
QList<int> xSupportedChartsDataFormats();
QString xChartsDataFormatName(int format);
void xSetupChartsDataFormat(QComboBox *comboBox);

// -------------------------------------------------------------------------------------------------
QString xSystemDateFormat();
QString xSystemTimeFormat();
QString xDateTimeString(const QString &format);
QDateTime xBuildDateTime();
QString xBuildDateTimeString(const QString &format);

// -------------------------------------------------------------------------------------------------
void xSetNoneBorder(QWidget *widget);

// -------------------------------------------------------------------------------------------------
#define X_DECLARE_PRIVATE(Class) \
    inline Class##Private *x_d_func() \
    { \
        return reinterpret_cast<Class##Private *>(x_d_ptr); \
    } \
    inline const Class##Private *x_d_func() const \
    { \
        return reinterpret_cast<const Class##Private *>(x_d_ptr); \
    } \
    friend class Class##Private; \
    class Class##Private *x_d_ptr{nullptr};

#define X_D(Class) Class##Private *const x_d = x_d_func();
#define X_DC(Class) const Class##Private *const x_d = x_d_func();

// -------------------------------------------------------------------------------------------------
#define X_DECLARE_PUBLIC(Class) \
    inline Class *x_q_func() \
    { \
        return reinterpret_cast<Class *>(x_q_ptr); \
    } \
    inline const Class *x_q_func() const \
    { \
        return reinterpret_cast<const Class *>(x_q_ptr); \
    } \
    friend class Class; \
    Class *x_q_ptr{nullptr};

#define X_Q(Class) Class *const x_q = x_q_func();
#define X_QC(Class) const Class *const x_q = x_q_func();