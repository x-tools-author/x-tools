/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QComboBox>
#include <QDateTime>
#include <QJsonObject>
#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QSettings>
#include <QSpinBox>
#include <QSplashScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>

#include "mainwindow.h"

#ifdef X_TOOLS_ENABLE_QSS
#include "qss/qssmgr.h"
#endif

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
    WebSocketServer,
    //------------------------------------------------------------------------------------------
    Hid = 0x00200000,
    SctpClient,
    SctpServer,
    //------------------------------------------------------------------------------------------
    ChartsTest = 0x00300000,
};
QList<int> supportedCommunicationTypes();
QString communicationName(int type);
void setupCommunicationTypes(QComboBox *comboBox);

/**********************************************************************************************/
enum class TextFormat { Bin, Oct, Dec, Hex, Ascii, Utf8 };
QList<int> supportedTextFormats();
QString textFormatName(TextFormat format);
void setupTextFormat(QComboBox *comboBox);
QString bytes2string(const QByteArray &bytes, int format);
QByteArray string2bytes(const QString &text, int format);
QByteArray arrayAppendArray(const QByteArray &a1, const QByteArray &a2);
void setupTextFormatValidator(QLineEdit *lineEdit, int format, int maxLen = 32767);

/**********************************************************************************************/
enum class Affixes { None, R, N, RN, NR };
QList<int> supportedAffixes();
QString additionName(int affixes);
void setupAddition(QComboBox *comboBox);
QByteArray cookedAffixes(int affixes);

/**********************************************************************************************/
enum class EscapeCharacter { None, R, N, RN, NR, R_N };
QList<int> supportedEscapeCharacters();
QString escapeCharacterName(int character);
void setupEscapeCharacter(QComboBox *comboBox);
QString cookedEscapeCharacter(const QString &text, int escapeCharacter);

/**********************************************************************************************/
enum class WebSocketDataChannel { Text, Binary };
QString webSocketDataChannelName(WebSocketDataChannel channel);
void setupWebSocketDataChannel(QComboBox *comboBox);

/**********************************************************************************************/
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

/**********************************************************************************************/
QString jsonValue2hexString(const QJsonValue &value);
QJsonValue hexString2jsonValue(const QString &hexString);
QString systemDateFormat();
QString systemTimeFormat();

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
TextItem defaultTextItem();
QString textItem2string(const TextItem &context);
QByteArray textItem2array(const TextItem &context);
TextItem loadTextItem(const QJsonObject &obj);
QJsonObject saveTextItem(const TextItem &context);

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
SerialPortItem defaultSerialPortItem();
QJsonObject saveSerialPortItem(const SerialPortItem &context);
SerialPortItem loadSerialPortItem(const QJsonObject &obj);
#if defined(X_TOOLS_ENABLE_SERIAL_PORT)
void setupPortName(QComboBox *comboBox);
void setupBaudRate(QComboBox *comboBox);
void setupDataBits(QComboBox *comboBox);
void setupParity(QComboBox *comboBox);
void setupStopBits(QComboBox *comboBox);
void setupFlowControl(QComboBox *comboBox);
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
    QString multicastAddress;
    quint16 multicastPort;
    bool enableMulticast;
    bool justMulticast;
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
    const QString multicastAddress{"multicastAddress"};
    const QString multicastPort{"multicastPort"};
    const QString enableMulticast{"enableMulticast"};
    const QString justMulticast{"justMulticast"};
};
SocketItem defaultSocketItem();
QJsonObject saveSocketItem(const SocketItem &context);
SocketItem loadSocketItem(const QJsonObject &obj);
void setupSocketAddress(QComboBox *cb);
void setupSocketPort(QSpinBox *spinBox);

/**********************************************************************************************/
enum class TransferType { Disabled, Bidirectional, Unidirectional };
QList<int> supportedTransferTypes();
QString transferTypeName(int type);
void setupTransferType(QComboBox *comboBox);

/**********************************************************************************************/
enum class ChartsDataFormat { BinaryY, BinaryXY, TextY, TextXY };
QList<int> supportedChartsDataFormats();
QString chartsDataFormatName(int format);
void setupChartsDataFormat(QComboBox *comboBox);

// -------------------------------------------------------------------------------------------------

#define g_xTools xTools::singleton()

static void (*gOutputLog2Ui)(QtMsgType, const QMessageLogContext &, const QString &);

class xToolsPrivate;
class xTools : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(xTools)
private:
    explicit xTools(QObject *parent = Q_NULLPTR);
    xTools(const xTools &) = delete;
    xTools &operator=(const xTools &) = delete;

public:
    static xTools &singleton();
    void doSomethingBeforeAppCreated(char *argv[],
                                     const QString &appName,
                                     const QString &appVersion = QString("0.0.0"),
                                     bool forStore = false);
    void doSomethingAfterAppExited();
    void googleLogInitializing(char *argv0);
    void googleLogShutdown();
    void googleLogToQtLog(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    // About git info
    QString xToolsLastCommit();
    QString xToolsLastCommitTime();

signals:
    void languageChanged();

public:
    // About splash screen
    bool splashScreenIsEnable();
    void splashScreenSetIsEnable(bool enable);
    void splashScreenSetMessage(const QString &msg);
    void splashScreenShow();
    QSplashScreen *splashScreenGet();

    // About app info
    QString appFriendlyName();
    void appSetFriendlyName(const QString &name);
    void appInitializeHdpi(const QString &appName, bool forStore);
    QString appVersion();

    // About i18n
    QString languageDefaultLanguage();
    QStringList languageSupportedLanguages();
    QStringList languageSupportedPrefixes();
    void languageSetSupportedPrefixes(const QStringList &prefixes);
    void languageSetupAppLanguageWithPrefix(const QString &language, const QString &prefix);
    void languageSetupAppLanguage(const QString &language = QString());

    // About high dpi policy
    QVariantList hdpiSupportedPolicies();
    QString hdpiPolicyName(int policy);
    bool hdpiIsValidPolicy(int policy);

    // About formater
    QString formatStringToHexString(const QString &str);
    QString formatHexStringToString(const QString &str);
    QByteArray formatByteArray2Hex(const QByteArray &source, char separator = '\0');

    // About date time
    QString dtDateTimeString(const QString &format);
    QDateTime dtBuildDateTime();
    QString dtBuildDateTimeString(const QString &format);
    QString dtSystemDateFormat();
    QString dtSystemTimeFormat();

    // About system
    QString sysDesktopPath();
    QString sysClipboardText();
    void sysSetClipboardText(const QString &text);
    void sysOpenUrl(const QString &url);

    // About icon
    QIcon iconToThemeIcon(const QIcon &icon);
    QIcon iconCookedIconFile(const QString &iconFile, const QString &color);
    QIcon iconCookedIcon(const QIcon &icon, const QString &color);

    // About settings
    void settingsOpenSettingsFileDir();
    QString settingsPath();
    int settingsHdpiPolicy();
    void settingsSetHdpiPolicy(int policy);
    QString settingsAppStyle();
    void settingsSetAppStyle(const QString &style);
    QString settingsLanguage();
    void settingsSetLanguage(const QString &lan);
    bool settingsClearSettings();
    void settingsSetClearSettings(bool clear);
    int settingsColorScheme();
    void settingsSetColorScheme(const int colorScheme);
    QVariant settingsValue(const QString &key, const QVariant &value = QVariant()) const;
    void settingsSetValue(const QString &key, const QVariant &value);
    void settingsSetJsonObjectStringValue(const QString &key, const QString &value);
    QSettings *settings();

    // Other functions
    QMainWindow *mainWindow();
    void moveToScreenCenter(QWidget *widget);
    bool tryToReboot();
    void tryToClearSettings();
};
