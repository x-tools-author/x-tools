/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QJsonObject>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>

namespace xTools {

class xIO : public QObject
{
    Q_OBJECT
public:
    /**********************************************************************************************/
    enum class CommunicationType {
        SerialPort,
        BleCentral,
        BlePeripheral,
        UdpClient,
        UdpServer,
        TcpClient,
        TcpServer,
        WebSocketClient,
        WebSocketServer
    };
    static QList<int> supportedCommunicationTypes();
    static QString CommunicationName(CommunicationType type);
    static void setupCommunicationTypes(QComboBox *comboBox);

    /**********************************************************************************************/
    enum class TextFormat { Bin, Oct, Dec, Hex, Ascii, Utf8 };
    static QList<int> supportedTextFormats();
    static QString textFormatName(TextFormat format);
    static void setupTextFormat(QComboBox *comboBox);
    static QString bytes2string(const QByteArray &bytes, TextFormat format);
    static QByteArray string2bytes(const QString &text, TextFormat format);
    static void setupTextFormatValidator(QLineEdit *lineEdit, TextFormat format);

    /**********************************************************************************************/
    enum class Affixes { None, R, N, RN, NR };
    static QList<int> supportedAffixes();
    static QString additionName(Affixes affixes);
    static void setupAddition(QComboBox *comboBox);
    static QByteArray cookedAffixes(Affixes affixes);

    /**********************************************************************************************/
    enum class EscapeCharacter { None, R, N, RN, NR, R_N };
    static QList<int> supportedEscapeCharacters();
    static QString escapeCharacterName(EscapeCharacter character);
    static void setupEscapeCharacter(QComboBox *comboBox);
    static QString cookedEscapeCharacter(const QString &text, EscapeCharacter escapeCharacter);

    /**********************************************************************************************/
    enum class WebSocketDataChannel { Text, Binary };
    static QString webSocketDataChannelName(WebSocketDataChannel channel);
    static void setupWebSocketDataChannel(QComboBox *comboBox);

    /**********************************************************************************************/
    enum class ResponseOption {
        Echo,   // Response data is the data received.
        Always, // Response automatically when data received.
        InputEqualReference,
        InputContainReference,
        InputDoesNotContainReference
    };
    static QList<int> supportedResponseOptions();
    static QString responseOptionName(ResponseOption option);

    /**********************************************************************************************/
    static QString jsonValue2hexString(const QJsonValue &value);
    static QJsonValue hexString2jsonValue(const QString &hexString);
    static QString systemDateFormat();
    static QString systemTimeFormat();
    static void try2reboot();

    /**********************************************************************************************/
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
    static TextItem defaultTextItem();
    static QString textItem2string(const TextItem &context);
    static QByteArray textItem2array(const TextItem &context);
    static TextItem loadTextItem(const QJsonObject &obj);
    static QJsonObject saveTextItem(const TextItem &context);

    /**********************************************************************************************/
    struct SerialPortItem
    {
        QString portName;
        qint32 baudRate;
        int dataBits;
        int parity;
        int stopBits;
        int flowControl;
    };
    struct SerialPortItemKeys
    {
        const QString portName{"portName"};
        const QString baudRate{"baudRate"};
        const QString dataBits{"dataBits"};
        const QString parity{"parity"};
        const QString stopBits{"stopBits"};
        const QString flowControl{"flowControl"};
    };
    static SerialPortItem defaultSerialPortItem();
    static QJsonObject saveSerialPortItem(const SerialPortItem &context);
    static SerialPortItem loadSerialPortItem(const QJsonObject &obj);
#if defined(X_TOOLS_ENABLE_MODULE_SERIALPORT)
    static void setupPortName(QComboBox *comboBox);
    static void setupBaudRate(QComboBox *comboBox);
    static void setupDataBits(QComboBox *comboBox);
    static void setupParity(QComboBox *comboBox);
    static void setupStopBits(QComboBox *comboBox);
    static void setupFlowControl(QComboBox *comboBox);
#endif

    /**********************************************************************************************/
    struct SocketItem
    {
        QString clientAddress;
        quint16 clientPort;
        QString serverAddress;
        quint16 serverPort;
        WebSocketDataChannel dataChannel;
        bool authentication;
        QString username;
        QString password;
    };
    struct SocketItemKeys
    {
        const QString clientAddress{"clientAddress"};
        const QString clientPort{"clientPort"};
        const QString serverAddress{"serverAddress"};
        const QString serverPort{"serverPort"};
        const QString dataChannel{"dataChannel"};
        const QString authentication{"authentication"};
        const QString username{"username"};
        const QString password{"password"};
    };
    static SocketItem defaultSocketItem();
    static QJsonObject saveSocketItem(const SocketItem &context);
    static SocketItem loadSocketItem(const QJsonObject &obj);
    static void setupSocketAddress(QComboBox *cb);
    static void setupSocketPort(QSpinBox *spinBox);

    /**********************************************************************************************/
    enum class TransferType { Disabled, Didirectional, Unidirectional };
    static QList<int> supportedTransferTypes();
    static QString transferTypeName(int type);
    static void setupTransferType(QComboBox *comboBox);
};

} // namespace xTools
