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

/**************************************************************************************************/
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
QList<int> supportedTextFormats();
QString textFormatName(TextFormat format);
void setupTextFormat(QComboBox *comboBox);
QString bytes2string(const QByteArray &bytes, int format);
QByteArray string2bytes(const QString &text, int format);
QByteArray arrayAppendArray(const QByteArray &a1, const QByteArray &a2);
void setupTextFormatValidator(QLineEdit *lineEdit, int format, int maxLen = 32767);
void printSupportedIconvEncodings();

/**************************************************************************************************/
enum class Affixes { None, R, N, RN, NR };
QList<int> supportedAffixes();
QString additionName(int affixes);
void setupAddition(QComboBox *comboBox);
QByteArray cookedAffixes(int affixes);

/**************************************************************************************************/
enum class EscapeCharacter { None, R, N, RN, NR, R_N, C };
QList<int> supportedEscapeCharacters();
QString escapeCharacterName(int character);
void setupEscapeCharacter(QComboBox *comboBox);
QString cookedEscapeCharacter(const QString &text, int escapeCharacter);

/**************************************************************************************************/
enum class WebSocketDataChannel { Text, Binary };
QString webSocketDataChannelName(WebSocketDataChannel channel);
void setupWebSocketDataChannel(QComboBox *comboBox);

/**************************************************************************************************/
enum class ResponseOption {
    Echo,   // Response data is the data received.
    Always, // Response automatically when data received.
    InputEqualReference,
    InputContainReference,
    InputDoesNotContainReference
};
QList<int> supportedResponseOptions();
QString responseOptionName(ResponseOption option);
void setupResponseOption(QComboBox *comboBox);
QByteArray responseData(const QByteArray &data, int option, const QByteArray &reference);

/**************************************************************************************************/
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
TextItem defaultTextItem();
QString textItem2string(const TextItem &context);
QByteArray textItem2array(const TextItem &context);
TextItem loadTextItem(const QJsonObject &obj);
QJsonObject saveTextItem(const TextItem &context);

/**************************************************************************************************/
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
SerialPortItem defaultSerialPortItem();
QJsonObject saveSerialPortItem(const SerialPortItem &context);
SerialPortItem loadSerialPortItem(const QJsonObject &obj);
#if X_ENABLE_SERIALPORT
void setupPortName(QComboBox *comboBox);
void setupBaudRate(QComboBox *comboBox);
void setupDataBits(QComboBox *comboBox);
void setupParity(QComboBox *comboBox);
void setupStopBits(QComboBox *comboBox);
void setupFlowControl(QComboBox *comboBox);
#endif

/**************************************************************************************************/
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
SocketItem defaultSocketItem();
QVariantMap saveSocketItem(const SocketItem &context);
SocketItem loadSocketItem(const QVariantMap &obj);
void setupSocketAddress(QComboBox *cb);
void setupSocketPort(QSpinBox *spinBox);

/**************************************************************************************************/
enum class TransferType { Disabled, Bidirectional, Unidirectional };
QList<int> xSupportedTransferTypes();
QString xTransferTypeName(int type);
void xSetupTransferType(QComboBox *comboBox);

/**************************************************************************************************/
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