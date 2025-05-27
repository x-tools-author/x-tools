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
// Compatibility
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#define xWebSocketErrorOccurred &QWebSocket::errorOccurred
#else
#define xWebSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QWebSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
#define xEnableColorScheme 1
#else
#define xEnableColorScheme 0
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#define xComboBoxActivated &QComboBox::activated
#else
#define xComboBoxActivated qOverload<int>(&QComboBox::activated)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#define xComboBoxIndexChanged &QComboBox::currentIndexChanged
#else
#define xComboBoxIndexChanged qOverload<int>(&QComboBox::currentIndexChanged)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#define xDefaultStyleName Application::style()->name()
#else
#define xDefaultStyleName QString("fusion")
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
#define xCheckStateChanged &QCheckBox::checkStateChanged
#else
#define xCheckStateChanged qOverload<int>(&QCheckBox::stateChanged)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xSkipEmptyParts Qt::SkipEmptyParts
#else
#define xSkipEmptyParts QString::SkipEmptyParts
#endif

/**************************************************************************************************/
enum class DeviceType {
    SerialPort,
    BleCentral,
    BlePeripheral,
    UdpClient,
    UdpServer,
    TcpClient,
    TcpServer,
    WebSocketClient,
    WebSocketServer,
    LocalSocket,
    LocalServer,
    //----------------------------------------------------------------------------------------------
    Hid = 0x00200000,
    SctpClient,
    SctpServer,
    //----------------------------------------------------------------------------------------------
    ChartsTest = 0x00300000
};
QList<int> supportedDeviceTypes();
QString deviceName(int type);
void setupDeviceTypes(QComboBox *comboBox);

/**************************************************************************************************/
enum class TextFormat { Bin, Oct, Dec, Hex, Ascii, Utf8 };
QList<int> supportedTextFormats();
QString textFormatName(TextFormat format);
void setupTextFormat(QComboBox *comboBox);
QString bytes2string(const QByteArray &bytes, int format);
QByteArray string2bytes(const QString &text, int format);
QByteArray arrayAppendArray(const QByteArray &a1, const QByteArray &a2);
void setupTextFormatValidator(QLineEdit *lineEdit, int format, int maxLen = 32767);

/**************************************************************************************************/
enum class Affixes { None, R, N, RN, NR };
QList<int> supportedAffixes();
QString additionName(int affixes);
void setupAddition(QComboBox *comboBox);
QByteArray cookedAffixes(int affixes);

/**************************************************************************************************/
enum class EscapeCharacter { None, R, N, RN, NR, R_N };
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
};
SerialPortItem defaultSerialPortItem();
QJsonObject saveSerialPortItem(const SerialPortItem &context);
SerialPortItem loadSerialPortItem(const QJsonObject &obj);
#if defined(X_ENABLE_SERIAL_PORT)
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
};
SocketItem defaultSocketItem();
QVariantMap saveSocketItem(const SocketItem &context);
SocketItem loadSocketItem(const QVariantMap &obj);
void setupSocketAddress(QComboBox *cb);
void setupSocketPort(QSpinBox *spinBox);

/**************************************************************************************************/
enum class TransferType { Disabled, Bidirectional, Unidirectional };
QList<int> supportedTransferTypes();
QString transferTypeName(int type);
void setupTransferType(QComboBox *comboBox);

/**************************************************************************************************/
enum class ChartsDataFormat { BinaryY, BinaryXY, TextY, TextXY };
QList<int> supportedChartsDataFormats();
QString chartsDataFormatName(int format);
void setupChartsDataFormat(QComboBox *comboBox);

/**************************************************************************************************/
QString systemDateFormat();
QString systemTimeFormat();
QString dateTimeString(const QString &format);
QDateTime buildDateTime();
QString buildDateTimeString(const QString &format);
