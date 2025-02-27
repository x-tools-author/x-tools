/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "xtools.h"
#include "xtools_p.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QStyle>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QTranslator>
#include <QUrl>
#include <QVariant>

#if defined(X_TOOLS_ENABLE_SERIAL_PORT)
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include <glog/logging.h>

#include "device/utilities/crc.h"
// -------------------------------------------------------------------------------------------------

QList<int> supportedCommunicationTypes()
{
    static QList<int> deviceTypes;
    if (deviceTypes.isEmpty()) {
#ifdef X_TOOLS_ENABLE_SERIAL_PORT
        deviceTypes << static_cast<int>(CommunicationType::SerialPort);
#endif
#ifdef X_TOOLS_ENABLE_BLUETOOTH
        deviceTypes << static_cast<int>(CommunicationType::BleCentral);
#endif
#if 0
            deviceTypes << static_cast<int>(DeviceType::BlePeripheral);
#endif
        deviceTypes << static_cast<int>(CommunicationType::UdpClient);
        deviceTypes << static_cast<int>(CommunicationType::UdpServer);
        deviceTypes << static_cast<int>(CommunicationType::TcpClient);
        deviceTypes << static_cast<int>(CommunicationType::TcpServer);
#ifdef X_TOOLS_ENABLE_WEB_SOCKET
        deviceTypes << static_cast<int>(CommunicationType::WebSocketClient);
        deviceTypes << static_cast<int>(CommunicationType::WebSocketServer);
#endif
        deviceTypes << static_cast<int>(CommunicationType::ChartsTest);
    }

    return deviceTypes;
}

QString communicationName(int type)
{
    switch (type) {
    case static_cast<int>(CommunicationType::SerialPort):
        return QObject::QObject::tr("Serial Port");
    case static_cast<int>(CommunicationType::BleCentral):
        return QObject::QObject::tr("BLE Central");
    case static_cast<int>(CommunicationType::BlePeripheral):
        return QObject::QObject::tr("BLE Peripheral");
    case static_cast<int>(CommunicationType::UdpClient):
        return QObject::QObject::tr("UDP Client");
    case static_cast<int>(CommunicationType::UdpServer):
        return QObject::QObject::tr("UDP Server");
    case static_cast<int>(CommunicationType::TcpClient):
        return QObject::QObject::tr("TCP Client");
    case static_cast<int>(CommunicationType::TcpServer):
        return QObject::QObject::tr("TCP Server");
    case static_cast<int>(CommunicationType::WebSocketClient):
        return QObject::QObject::tr("WebSocket Client");
    case static_cast<int>(CommunicationType::WebSocketServer):
        return QObject::QObject::tr("WebSocket Server");
    case static_cast<int>(CommunicationType::ChartsTest):
        return QObject::QObject::tr("Charts Test");
    default:
        return "Unknown";
    }
}

void setupCommunicationTypes(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> deviceTypes = supportedCommunicationTypes();
    for (int type : deviceTypes) {
        comboBox->addItem(communicationName(type), type);
    }

    comboBox->setCurrentIndex(comboBox->findData(static_cast<int>(CommunicationType::SerialPort)));
}

QList<int> supportedTextFormats()
{
    static QList<int> textFormats;
    if (textFormats.isEmpty()) {
        textFormats << static_cast<int>(TextFormat::Bin);
        textFormats << static_cast<int>(TextFormat::Oct);
        textFormats << static_cast<int>(TextFormat::Dec);
        textFormats << static_cast<int>(TextFormat::Hex);
        textFormats << static_cast<int>(TextFormat::Ascii);
        textFormats << static_cast<int>(TextFormat::Utf8);
    }

    return textFormats;
}

QString textFormatName(TextFormat format)
{
    switch (format) {
    case TextFormat::Bin:
        return QObject::QObject::tr("Binary");
    case TextFormat::Oct:
        return QObject::QObject::tr("Octal");
    case TextFormat::Dec:
        return QObject::QObject::tr("Decimal");
    case TextFormat::Hex:
        return QObject::QObject::tr("Hexadecimal");
    case TextFormat::Ascii:
        return QObject::QObject::tr("ASCII");
    case TextFormat::Utf8:
        return QObject::QObject::tr("UTF-8");
    default:
        return "Unknown";
    }
}

void setupTextFormat(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> textFormats = supportedTextFormats();
    for (int format : textFormats) {
        comboBox->addItem(textFormatName(static_cast<TextFormat>(format)), format);
    }

    comboBox->setCurrentIndex(comboBox->findData(static_cast<int>(TextFormat::Hex)));
}

QString bytes2string(const QByteArray &bytes, int format)
{
    auto cookedArray = [](const QByteArray &array, int base, int len) -> QString {
        QString str, numStr;
        for (int i = 0; i < array.length(); i++) {
            if (base == 10 || base == 8) {
                numStr = QString::number(array.at(i), base);
                str.append(QString("%1 ").arg(numStr));
            } else {
                numStr = QString::number(static_cast<quint8>(array.at(i)), base);
                str.append(QString("%1 ").arg(numStr, len, '0'));
            }
        }
        return str.toUpper();
    };

    if (static_cast<int>(TextFormat::Bin) == format) {
        return cookedArray(bytes, 2, 8);
    } else if (static_cast<int>(TextFormat::Oct) == format) {
        return cookedArray(bytes, 8, 3);
    } else if (static_cast<int>(TextFormat::Dec) == format) {
        return cookedArray(bytes, 10, 3);
    } else if (static_cast<int>(TextFormat::Hex) == format) {
        return cookedArray(bytes, 16, 2);
    } else if (static_cast<int>(TextFormat::Ascii) == format) {
        return QString::fromLatin1(bytes);
    } else if (static_cast<int>(TextFormat::Utf8) == format) {
        return QString::fromUtf8(bytes);
    } else {
        return QString("Unsupported text format: %1").arg(static_cast<int>(format));
    }
}

QByteArray string2bytes(const QString &text, int format)
{
    auto cookString = [](const QString &str, const int base) -> QByteArray {
        QByteArray data;
        const QStringList strList = str.split(' ', Qt::SkipEmptyParts);
        for (auto &string : strList) {
            auto value = static_cast<qint8>(string.toInt(Q_NULLPTR, base));
            data.append(reinterpret_cast<char *>(&value), 1);
        }

        return data;
    };

    QByteArray data;
    if (format == static_cast<int>(TextFormat::Bin)) {
        data = cookString(text, 2);
    } else if (format == static_cast<int>(TextFormat::Oct)) {
        data = cookString(text, 8);
    } else if (format == static_cast<int>(TextFormat::Dec)) {
        data = cookString(text, 10);
    } else if (format == static_cast<int>(TextFormat::Hex)) {
        data = cookString(text, 16);
    } else if (format == static_cast<int>(TextFormat::Ascii)) {
        data = text.toLatin1();
    } else {
        data = text.toUtf8();
    }

    return data;
}

QByteArray arrayAppendArray(const QByteArray &a1, const QByteArray &a2)
{
    return a1 + a2;
}

void setupTextFormatValidator(QLineEdit *lineEdit, int format, int maxLen)
{
    static QMap<int, QRegularExpressionValidator *> regularExpressionMap;
    if (regularExpressionMap.isEmpty()) {
        // clang-format off
            const QString binStr = "([01][01][01][01][01][01][01][01][ ])*";
            const QString octStr = "^(0[0-7]{0,2}|[1-3][0-7]{2})( (0[0-7]{0,2}|[1-3][0-7]{2}))*";
            const QString decStr = "^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9])( (25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]))*";
            const QString hexStr = "([0-9a-fA-F][0-9a-fA-F][ ])*";
        // clang-format on

        // clang-format off
            auto const binValidator = new QRegularExpressionValidator(QRegularExpression(binStr));
            auto const otcValidator = new QRegularExpressionValidator(QRegularExpression(octStr)); //0-377
            auto const decValidator = new QRegularExpressionValidator(QRegularExpression(decStr)); // 0-255;
            auto const hexValidator = new QRegularExpressionValidator(QRegularExpression(hexStr));
            auto const asciiValidator = new QRegularExpressionValidator(QRegularExpression("([ -~])*"));
        // clang-format on

        regularExpressionMap.insert(static_cast<int>(TextFormat::Bin), binValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Oct), otcValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Dec), decValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Hex), hexValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Ascii), asciiValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Utf8), nullptr);
    }

    if (lineEdit && regularExpressionMap.contains(format)) {
        lineEdit->setMaxLength(maxLen);
        lineEdit->setValidator(regularExpressionMap.value(format));
    } else {
        qWarning() << "Invalid parameter, the operation will be ignored!";
    }
}

QList<int> supportedAffixes()
{
    QList<int> additions;
    additions << static_cast<int>(Affixes::None);
    additions << static_cast<int>(Affixes::R);
    additions << static_cast<int>(Affixes::N);
    additions << static_cast<int>(Affixes::RN);
    additions << static_cast<int>(Affixes::NR);
    return additions;
}

QString additionName(int affixes)
{
    switch (affixes) {
    case static_cast<int>(Affixes::R):
        return "\\R";
    case static_cast<int>(Affixes::N):
        return "\\N";
    case static_cast<int>(Affixes::RN):
        return "\\R\\N";
    case static_cast<int>(Affixes::NR):
        return "\\N\\R";
    default:
        return QObject::QObject::tr("None");
    }
}

void setupAddition(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> additions = supportedAffixes();
    for (int addition : additions) {
        comboBox->addItem(additionName(addition), addition);
    }
}

QByteArray cookedAffixes(int affixes)
{
    switch (affixes) {
    case static_cast<int>(Affixes::R):
        return QByteArray{"\r"};
    case static_cast<int>(Affixes::N):
        return QByteArray{"\n"};
    case static_cast<int>(Affixes::RN):
        return QByteArray{"\r\n"};
    case static_cast<int>(Affixes::NR):
        return QByteArray{"\n\r"};
    default:
        return QByteArray{};
    }
}

QList<int> supportedEscapeCharacters()
{
    QList<int> escapeCharacters;
    escapeCharacters << static_cast<int>(EscapeCharacter::None);
    escapeCharacters << static_cast<int>(EscapeCharacter::R);
    escapeCharacters << static_cast<int>(EscapeCharacter::N);
    escapeCharacters << static_cast<int>(EscapeCharacter::RN);
    escapeCharacters << static_cast<int>(EscapeCharacter::NR);
    escapeCharacters << static_cast<int>(EscapeCharacter::R_N);
    return escapeCharacters;
}

QString escapeCharacterName(int escapeCharacter)
{
    switch (escapeCharacter) {
    case static_cast<int>(EscapeCharacter::R):
        return "\\r";
    case static_cast<int>(EscapeCharacter::N):
        return "\\n";
    case static_cast<int>(EscapeCharacter::RN):
        return "\\r\\n";
    case static_cast<int>(EscapeCharacter::NR):
        return "\\n\\r";
    case static_cast<int>(EscapeCharacter::R_N):
        return "\\r + \\n";
    default:
        return QObject::QObject::tr("None");
    }
}

void setupEscapeCharacter(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> escapeCharacters = supportedEscapeCharacters();
    for (int esc : escapeCharacters) {
        comboBox->addItem(escapeCharacterName(esc), esc);
    }
}

QString cookedEscapeCharacter(const QString &text, int escapeCharacter)
{
    QString newStr = text;
    if (escapeCharacter == static_cast<int>(EscapeCharacter::R)) {
        newStr.replace("\\r", "\r");
    } else if (escapeCharacter == static_cast<int>(EscapeCharacter::N)) {
        newStr.replace("\\n", "\n");
    } else if (escapeCharacter == static_cast<int>(EscapeCharacter::RN)) {
        newStr.replace("\\r\\n", "\r\n");
    } else if (escapeCharacter == static_cast<int>(EscapeCharacter::NR)) {
        newStr.replace("\\n\\r", "\n\\r");
    } else if (escapeCharacter == static_cast<int>(EscapeCharacter::R_N)) {
        newStr.replace("\\r", "\r");
        newStr.replace("\\n", "\n");
    }

    return newStr;
}

QString webSocketDataChannelName(WebSocketDataChannel channel)
{
    if (channel == WebSocketDataChannel::Text) {
        return QObject::tr("Text");
    } else if (channel == WebSocketDataChannel::Binary) {
        return QObject::tr("Binary");
    }

    return "Unknown";
}

void setupWebSocketDataChannel(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        auto text = WebSocketDataChannel::Text;
        auto binary = WebSocketDataChannel::Binary;
        comboBox->addItem(webSocketDataChannelName(text), static_cast<int>(text));
        comboBox->addItem(webSocketDataChannelName(binary), static_cast<int>(binary));
    }
}

QList<int> supportedResponseOptions()
{
    static QList<int> list;
    if (list.isEmpty()) {
        list.append(static_cast<int>(ResponseOption::Echo));
        list.append(static_cast<int>(ResponseOption::Always));
        list.append(static_cast<int>(ResponseOption::InputEqualReference));
        list.append(static_cast<int>(ResponseOption::InputContainReference));
        list.append(static_cast<int>(ResponseOption::InputDoesNotContainReference));
    }

    return list;
}

QString responseOptionName(ResponseOption option)
{
    static QMap<ResponseOption, QString> map;
    if (map.isEmpty()) {
        map.insert(ResponseOption::Echo, QObject::tr("Echo"));
        map.insert(ResponseOption::Always, QObject::tr("Always"));
        map.insert(ResponseOption::InputEqualReference, QObject::tr("Input Equal Reference"));
        map.insert(ResponseOption::InputContainReference, QObject::tr("Input Contain Reference"));
        map.insert(ResponseOption::InputDoesNotContainReference,
                   QObject::tr("Input Does not Contain Reference"));
    }

    if (map.contains(option)) {
        return map[option];
    } else {
        return "Unknown";
    }
}

void setupResponseOption(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        QList<int> options = supportedResponseOptions();
        for (int option : options) {
            comboBox->addItem(responseOptionName(static_cast<ResponseOption>(option)), option);
        }
    }
}

QByteArray responseData(const QByteArray &data, int option, const QByteArray &reference)
{
    if (data.isEmpty()) {
        return QByteArray{};
    }

    if (option == static_cast<int>(ResponseOption::Echo)) {
        return data;
    } else if (option == static_cast<int>(ResponseOption::Always)) {
        return reference;
    } else if (option == static_cast<int>(ResponseOption::InputEqualReference)) {
        if (data == reference) {
            return reference;
        } else {
            return QByteArray{};
        }
    } else if (option == static_cast<int>(ResponseOption::InputContainReference)) {
        if (data.contains(reference)) {
            return reference;
        } else {
            return QByteArray{};
        }
    } else if (option == static_cast<int>(ResponseOption::InputDoesNotContainReference)) {
        if (!data.contains(reference)) {
            return reference;
        } else {
            return QByteArray{};
        }
    } else {
        return QByteArray{};
    }
}

QString jsonValue2hexString(const QJsonValue &value)
{
    QJsonDocument doc;
    if (value.isObject()) {
        doc.setObject(value.toObject());
    } else if (value.isArray()) {
        doc.setArray(value.toArray());
    } else {
        return QString{};
    }

    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact).toHex());
}

QJsonValue hexString2jsonValue(const QString &hexString)
{
    QByteArray bytes = QByteArray::fromHex(hexString.toUtf8());
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    if (doc.isArray()) {
        return doc.array();
    } else if (doc.isObject()) {
        return doc.object();
    } else {
        return QJsonValue{};
    }
}

QString systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

TextItem defaultTextItem()
{
    TextItem context;
    context.textFormat = TextFormat::Utf8;
    context.escapeCharacter = EscapeCharacter::None;
    context.prefix = Affixes::None;
    context.text = QString("Test data");
    context.suffix = Affixes::None;

    context.crc.enable = false;
    context.crc.bigEndian = true;
    context.crc.algorithm = static_cast<int>(CRC::Algorithm::CRC_8);
    context.crc.startIndex = 0;
    context.crc.endIndex = 0;
    return context;
}

QString textItem2string(const TextItem &context)
{
    QString prefix = additionName(static_cast<int>(context.prefix));
    QString payload = cookedEscapeCharacter(context.text, static_cast<int>(context.escapeCharacter));
    QString crc;
    if (context.crc.enable) {
        QByteArray data = string2bytes(context.text, static_cast<int>(context.textFormat));

        CRC::Context ctx;
        ctx.algorithm = static_cast<CRC::Algorithm>(context.crc.algorithm);
        ctx.startIndex = context.crc.startIndex;
        ctx.endIndex = context.crc.endIndex;
        ctx.bigEndian = context.crc.bigEndian;
        ctx.data = data;

        QByteArray crcArray = CRC::calculate(ctx);
        crc = QString::fromLatin1(crcArray.toHex());
        crc = crc.toUpper();
    }

    QString suffix = additionName(static_cast<int>(context.suffix));
    return QString("[%1][%2][%3][%4]").arg(prefix, payload, crc, suffix);
}

QByteArray textItem2array(const TextItem &context)
{
    QByteArray prefix = cookedAffixes(static_cast<int>(context.prefix));
    int esc = static_cast<int>(context.escapeCharacter);
    QString text = cookedEscapeCharacter(context.text, esc);
    QByteArray payload = string2bytes(text, static_cast<int>(context.textFormat));

    CRC::Context ctx;
    ctx.algorithm = static_cast<CRC::Algorithm>(context.crc.algorithm);
    ctx.startIndex = context.crc.startIndex;
    ctx.endIndex = context.crc.endIndex;
    ctx.bigEndian = context.crc.bigEndian;
    ctx.data = payload;

    QByteArray crc = CRC::calculate(ctx);
    QByteArray suffix = cookedAffixes(static_cast<int>(context.suffix));

    if (context.crc.enable) {
        return (prefix + payload + crc + suffix);
    } else {
        return (prefix + payload + suffix);
    }
}

TextItem loadTextItem(const QJsonObject &obj)
{
    TextItem ctx;
    const TextItemKeys keys;
    ctx.text = obj.value(keys.text).toString();
    ctx.textFormat = static_cast<TextFormat>(obj.value(keys.textFormat).toInt());
    ctx.escapeCharacter = static_cast<EscapeCharacter>(obj.value(keys.escapeCharacter).toInt());
    ctx.prefix = static_cast<Affixes>(obj.value(keys.prefix).toInt());
    ctx.suffix = static_cast<Affixes>(obj.value(keys.suffix).toInt());
    ctx.crc.algorithm = obj.value(keys.crcAlgorithm).toInt();
    ctx.crc.bigEndian = obj.value(keys.crcBigEndian).toBool();
    ctx.crc.enable = obj.value(keys.crcEnable).toBool();
    ctx.crc.startIndex = obj.value(keys.crcStartIndex).toInt();
    ctx.crc.endIndex = obj.value(keys.crcEndIndex).toInt();
    return ctx;
}

QJsonObject saveTextItem(const TextItem &context)
{
    QJsonObject obj;
    const TextItemKeys keys;
    obj.insert(keys.text, context.text);
    obj.insert(keys.textFormat, static_cast<int>(context.textFormat));
    obj.insert(keys.escapeCharacter, static_cast<int>(context.escapeCharacter));
    obj.insert(keys.prefix, static_cast<int>(context.prefix));
    obj.insert(keys.suffix, static_cast<int>(context.suffix));
    obj.insert(keys.crcAlgorithm, static_cast<int>(context.crc.algorithm));
    obj.insert(keys.crcBigEndian, context.crc.bigEndian);
    obj.insert(keys.crcEnable, context.crc.enable);
    obj.insert(keys.crcStartIndex, context.crc.startIndex);
    obj.insert(keys.crcEndIndex, context.crc.endIndex);
    return obj;
}

SerialPortItem defaultSerialPortItem()
{
    SerialPortItem context;
    context.portName = "";
    context.baudRate = 9600;
    context.dataBits = 8;
    context.parity = 0;
    context.stopBits = 1;
    context.flowControl = 0;

    return context;
}

QJsonObject saveSerialPortItem(const SerialPortItem &context)
{
    QJsonObject obj;
    const SerialPortItemKeys keys;
    obj.insert(keys.portName, context.portName);
    obj.insert(keys.baudRate, context.baudRate);
    obj.insert(keys.dataBits, context.dataBits);
    obj.insert(keys.parity, context.parity);
    obj.insert(keys.stopBits, context.stopBits);
    obj.insert(keys.flowControl, context.flowControl);
    return obj;
}

SerialPortItem loadSerialPortItem(const QJsonObject &obj)
{
    SerialPortItem ctx;
    const SerialPortItemKeys keys;
    ctx.portName = obj.value(keys.portName).toString();
    ctx.baudRate = obj.value(keys.baudRate).toInt();
    ctx.dataBits = obj.value(keys.dataBits).toInt();
    ctx.parity = obj.value(keys.parity).toInt();
    ctx.stopBits = obj.value(keys.stopBits).toInt();
    ctx.flowControl = obj.value(keys.flowControl).toInt();
    return ctx;
}

#if defined(X_TOOLS_ENABLE_SERIAL_PORT)
void setupPortName(QComboBox *comboBox)
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (const auto &info : infos) {
        comboBox->addItem(info.portName());
    }
}

void setupBaudRate(QComboBox *comboBox)
{
    QList<int> baudRates = QSerialPortInfo::standardBaudRates();
    for (const auto &baudRate : baudRates) {
        comboBox->addItem(QString::number(baudRate), baudRate);
    }
    comboBox->setCurrentText("9600");
}

void setupDataBits(QComboBox *comboBox)
{
    comboBox->addItem("8", QSerialPort::Data8);
    comboBox->addItem("7", QSerialPort::Data7);
    comboBox->addItem("6", QSerialPort::Data6);
    comboBox->addItem("5", QSerialPort::Data5);
}

void setupParity(QComboBox *comboBox)
{
    comboBox->addItem(QObject::tr("None"), QSerialPort::NoParity);
    comboBox->addItem(QObject::tr("Even"), QSerialPort::EvenParity);
    comboBox->addItem(QObject::tr("Odd"), QSerialPort::OddParity);
    comboBox->addItem(QObject::tr("Space"), QSerialPort::SpaceParity);
    comboBox->addItem(QObject::tr("Mark"), QSerialPort::MarkParity);
}

void setupStopBits(QComboBox *comboBox)
{
    comboBox->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    comboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    comboBox->addItem("2", QSerialPort::TwoStop);
}

void setupFlowControl(QComboBox *comboBox)
{
    comboBox->addItem(QObject::tr("None"), QSerialPort::NoFlowControl);
#if 0
        comboBox->addItem(QObject::tr("RTS/CTS"), QSerialPort::HardwareControl);
        comboBox->addItem(QObject::tr("X-ON/X-OFF"), QSerialPort::SoftwareControl);
#else
    comboBox->addItem(QObject::tr("Hardware"), QSerialPort::HardwareControl);
    comboBox->addItem(QObject::tr("Software"), QSerialPort::SoftwareControl);
#endif
}
#endif

SocketItem defaultSocketItem()
{
    SocketItem item;
    item.clientAddress = "127.0.0.1";
    item.clientPort = 54321;
    item.serverAddress = "127.0.0.1";
    item.serverPort = 12345;
    item.dataChannel = WebSocketDataChannel::Text;
    item.authentication = false;
    item.username = "";
    item.password = "";
    return item;
}

QJsonObject saveSocketItem(const SocketItem &context)
{
    QJsonObject obj;
    const SocketItemKeys keys;
    obj.insert(keys.clientAddress, context.clientAddress);
    obj.insert(keys.clientPort, context.clientPort);
    obj.insert(keys.serverAddress, context.serverAddress);
    obj.insert(keys.serverPort, context.serverPort);
    obj.insert(keys.dataChannel, static_cast<int>(context.dataChannel));
    obj.insert(keys.authentication, context.authentication);
    obj.insert(keys.username, context.username);
    obj.insert(keys.password, context.password);
    obj.insert(keys.multicastAddress, context.multicastAddress);
    obj.insert(keys.multicastPort, context.multicastPort);
    obj.insert(keys.enableMulticast, context.enableMulticast);
    obj.insert(keys.justMulticast, context.justMulticast);
    return obj;
}

SocketItem loadSocketItem(const QJsonObject &obj)
{
    SocketItem ctx;
    const SocketItemKeys keys;
    ctx.clientAddress = obj.value(keys.clientAddress).toString();
    ctx.clientPort = obj.value(keys.clientPort).toInt();
    ctx.serverAddress = obj.value(keys.serverAddress).toString();
    ctx.serverPort = obj.value(keys.serverPort).toInt();
    ctx.dataChannel = static_cast<WebSocketDataChannel>(obj.value(keys.dataChannel).toInt());
    ctx.authentication = obj.value(keys.authentication).toBool();
    ctx.username = obj.value(keys.username).toString();
    ctx.password = obj.value(keys.password).toString();
    ctx.multicastAddress = obj.value(keys.multicastAddress).toString();
    ctx.multicastPort = obj.value(keys.multicastPort).toInt();
    ctx.enableMulticast = obj.value(keys.enableMulticast).toBool();
    ctx.justMulticast = obj.value(keys.justMulticast).toBool();
    return ctx;
}

void setupSocketAddress(QComboBox *cb)
{
    if (!cb) {
        return;
    }

    QStringList ipv4List, ipv6List;
    auto addresses = QNetworkInterface::allAddresses();
    for (auto &address : addresses) {
        auto str = address.toString();

        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ipv4List.append(str);
        } else if (address.protocol() == QAbstractSocket::IPv6Protocol) {
            ipv6List.append(str);
        }
    }

    cb->clear();
    cb->addItems(ipv4List);
#if 0
        cb->addItems(ipv6List);
#endif
}

void setupSocketPort(QSpinBox *spinBox)
{
    spinBox->setRange(0, 65535);
}

QList<int> supportedTransferTypes()
{
    static QList<int> types = {
        static_cast<int>(TransferType::Disabled),
        static_cast<int>(TransferType::Bidirectional),
        static_cast<int>(TransferType::Unidirectional),
    };

    return types;
}

QString transferTypeName(int type)
{
    if (type == static_cast<int>(TransferType::Disabled)) {
        return QObject::tr("Disabled");
    } else if (type == static_cast<int>(TransferType::Bidirectional)) {
        return QObject::tr("Bidirectional");
    } else if (type == static_cast<int>(TransferType::Unidirectional)) {
        return QObject::tr("Unidirectional");
    } else {
        return "Unknown";
    }
}

void setupTransferType(QComboBox *comboBox)
{
    comboBox->clear();

    auto types = supportedTransferTypes();
    for (const auto &type : types) {
        comboBox->addItem(transferTypeName(type), type);
    }
}

QList<int> supportedChartsDataFormats()
{
    static QList<int> formats = {
        static_cast<int>(ChartsDataFormat::BinaryY),
        //static_cast<int>(ChartsDataFormat::BinaryXY),
        static_cast<int>(ChartsDataFormat::TextY),
        //static_cast<int>(ChartsDataFormat::TextXY),
    };

    return formats;
}

QString chartsDataFormatName(int format)
{
    if (format == static_cast<int>(ChartsDataFormat::BinaryY)) {
        return QObject::tr("Binary") + "-Y";
    } else if (format == static_cast<int>(ChartsDataFormat::BinaryXY)) {
        return QObject::tr("Binary") + "-XY";
    } else if (format == static_cast<int>(ChartsDataFormat::TextY)) {
        return QObject::tr("Text") + "-Y";
    } else if (format == static_cast<int>(ChartsDataFormat::TextXY)) {
        return QObject::tr("Text") + "-XY";
    } else {
        return "Unknown";
    }
}

void setupChartsDataFormat(QComboBox *comboBox)
{
    comboBox->clear();

    auto formats = supportedChartsDataFormats();
    for (const auto &format : formats) {
        comboBox->addItem(chartsDataFormatName(format), format);
    }
}

QString dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QDateTime buildDateTime()
{
    QString dateString = QString(__DATE__);
    QString timeString = QString(__TIME__);
    dateString = dateString.replace(QString("  "), " 0");
    QString dateTimeString = dateString + " " + timeString;
    QDateTime dateTime = QLocale(QLocale::English).toDateTime(dateTimeString, "MMM dd yyyy hh:mm:ss");
    return dateTime;
}

QString buildDateTimeString(const QString &format)
{
    return buildDateTime().toString(format);
}

// -------------------------------------------------------------------------------------------------

xTools::xTools(QObject *parent)
    : QObject(*new xToolsPrivate, parent)
{
    QStringList list;
    list << "xToolsCore";
    languageSetSupportedPrefixes(list);
}

xTools &xTools::singleton()
{
    static xTools singleton;
    return singleton;
}

void xTools::doSomethingBeforeAppCreated(char *argv[],
                                         const QString &appName,
                                         const QString &appVersion,
                                         bool forStore)
{
    QString cookedAppName = appName;
    if (forStore) {
        cookedAppName += QString("(Store)");
    }

    cookedAppName.remove(" ");
    QCoreApplication::setOrganizationName(QString("xTools"));
    QCoreApplication::setOrganizationDomain(QString("IT"));
    QCoreApplication::setApplicationName(cookedAppName);
    QCoreApplication::setApplicationVersion(appVersion);

    g_xTools.appSetFriendlyName(appName);
    g_xTools.tryToClearSettings();
    g_xTools.googleLogInitializing(argv[0]);
    g_xTools.appInitializeHdpi(appName, forStore);
#ifdef QT_RELEASE
    qInstallMessageHandler(googleLogToQtLog);
#endif

    qInfo() << "The settings path is:" << qPrintable(g_xTools.settingsPath());
    qInfo() << "The settings file is:" << qPrintable(g_xTools.settings()->fileName());
    qInfo() << "The application version is:" << qPrintable(g_xTools.appVersion());
    qInfo() << "The application friendly name is:" << qPrintable(g_xTools.appFriendlyName());
}

void xTools::doSomethingAfterAppExited()
{
    xTools::googleLogShutdown();
}

void xTools::googleLogInitializing(char *argv0)
{
    QString logPath = g_xTools.settingsPath();
    logPath += "/log";
    QDir dir(logPath);
    if (!dir.exists(logPath) && !dir.mkpath(logPath)) {
        qWarning() << "Make log directory failed";
    }

    auto keep = std::chrono::minutes(30 * 24 * 60);
    google::SetLogFilenameExtension(".log");     // The suffix of log file.
    google::EnableLogCleaner(keep);              // Keep the log file for 30 days.
    google::SetApplicationFingerprint("xTools"); // (It seem to be no use.)

    fLB::FLAGS_logtostdout = false;
    fLB::FLAGS_logtostderr = false;
    fLS::FLAGS_log_dir = logPath.toUtf8().data(); // The path of log.
    fLI::FLAGS_logbufsecs = 0;                    //
    fLU::FLAGS_max_log_size = 10;                 // The max size(MB) of log file.
    fLB::FLAGS_stop_logging_if_full_disk = true;  //
    fLB::FLAGS_alsologtostderr = true;            //

    google::InitGoogleLogging(argv0);
    qInfo() << "The logging path is:" << qPrintable(logPath);
}

void xTools::googleLogShutdown()
{
    google::ShutdownGoogleLogging();
}

void xTools::googleLogToQtLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toUtf8();
    const char *file = context.file ? context.file : "";
    const int line = context.line;

    switch (type) {
    case QtWarningMsg:
        google::LogMessage(file, line, google::GLOG_WARNING).stream() << localMsg.data();
        break;
    case QtCriticalMsg:
        google::LogMessage(file, line, google::GLOG_ERROR).stream() << localMsg.data();
        break;
    case QtFatalMsg:
        google::LogMessage(file, line, google::GLOG_FATAL).stream() << localMsg.data();
        break;
    default:
        google::LogMessage(file, line, google::GLOG_INFO).stream() << localMsg.data();
        break;
    }

    if (gOutputLog2Ui) {
        (*gOutputLog2Ui)(type, context, msg);
    }
}

QString xTools::xToolsLastCommit()
{
#if defined(X_TOOLS_GIT_COMMIT)
    return X_TOOLS_GIT_COMMIT;
#else
    return "Unknown";
#endif
}

QString xTools::xToolsLastCommitTime()
{
#if defined(X_TOOLS_GIT_COMMIT_TIME)
    return X_TOOLS_GIT_COMMIT_TIME;
#else
    return "Unknown";
#endif
}

bool xTools::splashScreenIsEnable()
{
    Q_D(xTools);
    return d->m_enableSplashScreen;
}

void xTools::splashScreenSetIsEnable(bool enable)
{
    Q_D(xTools);
    d->m_enableSplashScreen = enable;
}

void xTools::splashScreenSetMessage(const QString &msg)
{
    Q_D(xTools);
    d->m_splashScreen->showMessage(msg);
}

void xTools::splashScreenShow()
{
    if (!qApp) {
        return;
    }

    QFont font = qApp->font();
    font.setPixelSize(52);

    QFontMetrics fontMetrics(font);
    const QString displayName = g_xTools.appFriendlyName();
    int width = fontMetrics.boundingRect(displayName).width() * 1.2;

    QPixmap pixmap(width < 600 ? 600 : width, 260);
    pixmap.fill(QColor(0x2d2d30));

    QPainter painter(&pixmap);
    painter.setPen(QColor(Qt::white));
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignHCenter | Qt::AlignVCenter, displayName);
    painter.drawRect(pixmap.rect() - QMargins(1, 1, 1, 1));

    Q_D(xTools);
    d->m_splashScreen = new QSplashScreen(pixmap);
    d->m_splashScreen->show();
}

QSplashScreen *xTools::splashScreenGet()
{
    Q_D(xTools);
    return d->m_splashScreen;
}

QString xTools::appFriendlyName()
{
    Q_D(xTools);
    return d->m_appFriendlyName;
}

void xTools::appSetFriendlyName(const QString &name)
{
    Q_D(xTools);
    d->m_appFriendlyName = name;
}

void xTools::appInitializeHdpi(const QString &appName, bool forStore)
{
#if 0
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif

    Q_D(xTools);
    int policy = settingsHdpiPolicy();
    if (!hdpiIsValidPolicy(policy)) {
        qWarning() << "The value of hdpi policy is not specified, set to default value:"
                   << QGuiApplication::highDpiScaleFactorRoundingPolicy();
        return;
    }

    const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
    qInfo() << "The current high dpi policy is:" << cookedPolicy;
}

QString xTools::appVersion()
{
    return QApplication::applicationVersion();
}

QString xTools::languageDefaultLanguage()
{
    Q_D(xTools);
    QSettings *settings = d->m_settings;
    QString language = settings->value("language").toString();
    if (language.isEmpty()) {
        language = QLocale::system().name();
    }

    if (!d->m_languageFlagNameMap.contains(language)) {
        qWarning() << "The language name is not supported:" << language << ". \"en\" will be used.";
        language = "en";
    }

    return d->m_languageFlagNameMap.value(language);
}

QStringList xTools::languageSupportedLanguages()
{
    Q_D(xTools);
    return d->m_languageFlagNameMap.values();
}

QStringList xTools::languageSupportedPrefixes()
{
    Q_D(xTools);
    // Such as "xToolsCore", "xApp"
    return d->m_appSupportedLanguagePrefixes;
}

void xTools::languageSetSupportedPrefixes(const QStringList &prefixes)
{
    Q_D(xTools);
    d->m_appSupportedLanguagePrefixes = prefixes;
}

void xTools::languageSetupAppLanguageWithPrefix(const QString &language, const QString &prefix)
{
    Q_D(xTools);
    QString key = d->m_languageFlagNameMap.key(language);
    if (language.isEmpty()) {
        key = QLocale::system().name();
        qWarning() << "The language is not specified, system language will be used:" << key;
    }

    if (!d->m_languageFlagNameMap.contains(key)) {
        auto info = QString("The language(%1) isn't supported. English 'll be used.").arg(language);
        qWarning() << qPrintable(info);
        key = "en";
    }

    static QMap<QString, QTranslator *> fileTranslatorMap;
    QString fileName = QString(":/res/translations/%1_%2.qm").arg(prefix, key);
    if (!fileTranslatorMap.contains(prefix)) {
        QTranslator *translator = new QTranslator();
        fileTranslatorMap.insert(prefix, translator);
    } else {
        bool ret = QCoreApplication::removeTranslator(fileTranslatorMap.value(prefix));
        const QString status = ret ? "success" : "failed";
        auto info = QString("The translator(%1) removing: %2.").arg(prefix, status);
        qInfo() << qPrintable(info);
    }

    QTranslator *translator = fileTranslatorMap.value(prefix);
    if (!translator->load(fileName)) {
        auto info = QString("The file(%1) can not be load to translator.").arg(fileName);
        qWarning() << qPrintable(info);
        return;
    }

    if (QCoreApplication::installTranslator(translator)) {
        const QString name = d->m_languageFlagNameMap.value(key);
        auto info = QString("The language(%1) has been setup, current language is:").arg(fileName);
        qInfo() << qPrintable(info) << name;
        emit languageChanged();
    } else {
        translator->deleteLater();
        translator = Q_NULLPTR;
        auto info = QString("The translator(%1) install failed(qm file is empty?).").arg(fileName);
        qInfo() << qPrintable(info);
        return;
    }
}

void xTools::languageSetupAppLanguage(const QString &language)
{
    Q_D(xTools);
    QString tmp = language;
    if (tmp.isEmpty()) {
        tmp = d->m_settings->value("language").toString();
        if (tmp.isEmpty()) {
            tmp = languageDefaultLanguage();
        }
    }

    QStringList prefixes = languageSupportedPrefixes();
    for (const auto &prefix : prefixes) {
        languageSetupAppLanguageWithPrefix(tmp, prefix);
    }
}

QVariantList xTools::hdpiSupportedPolicies()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::HighDpiScaleFactorRoundingPolicy>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}

QString xTools::hdpiPolicyName(int policy)
{
    typedef Qt::HighDpiScaleFactorRoundingPolicy Policy;
    static QMap<Policy, QString> policyMap;
    if (policyMap.isEmpty()) {
        policyMap.insert(Policy::Unset, QObject::tr("System"));
        policyMap.insert(Policy::Round, QObject::tr("Round up for .5 and above"));
        policyMap.insert(Policy::Ceil, QObject::tr("Always round up"));
        policyMap.insert(Policy::Floor, QObject::tr("Always round down"));
        policyMap.insert(Policy::RoundPreferFloor, QObject::tr("Round up for .75 and above"));
        policyMap.insert(Policy::PassThrough, QObject::tr("Don't round"));
    }

    return policyMap.value(static_cast<Policy>(policy), "Unknown");
}

bool xTools::hdpiIsValidPolicy(int policy)
{
    auto policies = hdpiSupportedPolicies();
    return policies.contains(QVariant(policy));
}

QString xTools::formatStringToHexString(const QString &str)
{
    return QString::fromLatin1(str.toUtf8().toHex());
}

QString xTools::formatHexStringToString(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toUtf8());
    return QString::fromUtf8(arr);
}

QByteArray xTools::formatByteArray2Hex(const QByteArray &source, char separator)
{
    if (source.isEmpty()) {
        return source;
    }

    auto toHex = [](quint8 value) -> char { return "0123456789abcdef"[value & 0xF]; };
    const int length = separator ? (source.size() * 3 - 1) : (source.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(source.constData());
    for (int i = 0, o = 0; i < source.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}

QString xTools::dtDateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QDateTime xTools::dtBuildDateTime()
{
    QString dateString = QString(__DATE__);
    QString timeString = QString(__TIME__);
    dateString = dateString.replace(QString("  "), " 0");
    QString dateTimeString = dateString + " " + timeString;
    QDateTime dateTime = QLocale(QLocale::English).toDateTime(dateTimeString, "MMM dd yyyy hh:mm:ss");
    return dateTime;
}

QString xTools::dtBuildDateTimeString(const QString &format)
{
    return dtBuildDateTime().toString(format);
}

QString xTools::dtSystemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString xTools::dtSystemTimeFormat()
{
    return QLocale::system().timeFormat();
}

QString xTools::sysDesktopPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

QString xTools::sysClipboardText()
{
    return QGuiApplication::clipboard()->text();
}

void xTools::sysSetClipboardText(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}

void xTools::sysOpenUrl(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}

QIcon xTools::iconToThemeIcon(const QIcon &icon)
{
    const QString color = qApp->palette().windowText().color().name();
    return iconCookedIcon(icon, color);
}

QIcon xTools::iconCookedIconFile(const QString &iconFile, const QString &color)
{
    QIcon icon(iconFile);
    return iconCookedIcon(icon, color);
}

QIcon xTools::iconCookedIcon(const QIcon &icon, const QString &color)
{
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), QColor(color));
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

void xTools::settingsOpenSettingsFileDir()
{
    QDesktopServices::openUrl(settingsPath());
}

QString xTools::settingsPath()
{
    Q_D(xTools);
    QString settingsFile(d->m_settings->fileName());
    QString path = settingsFile.left(settingsFile.lastIndexOf("/"));
    return path;
}

int xTools::settingsHdpiPolicy()
{
    Q_D(xTools);
    auto var = d->m_settings->value("hdpiPolicy");
    if (var.isValid()) {
        return var.toInt();
    }

    return int(QGuiApplication::highDpiScaleFactorRoundingPolicy());
}

void xTools::settingsSetHdpiPolicy(int policy)
{
    Q_D(xTools);
    d->m_settings->setValue("hdpiPolicy", policy);
}

QString xTools::settingsAppStyle()
{
    Q_D(xTools);
    auto var = d->m_settings->value("appStyle");
    if (var.isValid()) {
        var.toString();
    }

    if (QApplication::instance()) {
        return QApplication::style()->name();
    }

    return QString("");
}

void xTools::settingsSetAppStyle(const QString &style)
{
    Q_D(xTools);
    d->m_settings->setValue("appStyle", style);
}

QString xTools::settingsLanguage()
{
    Q_D(xTools);
    auto var = d->m_settings->value("language");
    if (var.isValid()) {
        return var.toString();
    }

    return QString("");
}

void xTools::settingsSetLanguage(const QString &lan)
{
    Q_D(xTools);
    d->m_settings->setValue("language", lan);
}

bool xTools::settingsClearSettings()
{
    Q_D(xTools);
    return d->m_settings->value("clearSettings").toBool();
}

void xTools::settingsSetClearSettings(bool clear)
{
    Q_D(xTools);
    d->m_settings->setValue("clearSettings", clear);
}

int xTools::settingsColorScheme()
{
    Q_D(xTools);
    auto var = d->m_settings->value("colorScheme", static_cast<int>(Qt::ColorScheme::Unknown));
    if (var.isValid()) {
        return var.toInt();
    }

    return 0;
}

void xTools::settingsSetColorScheme(const int colorScheme)
{
    Q_D(xTools);
    d->m_settings->setValue("colorScheme", colorScheme);
}

QVariant xTools::settingsValue(const QString &key, const QVariant &value) const
{
    Q_D(const xTools);
    return d->m_settings->value(key, value);
}

void xTools::settingsSetValue(const QString &key, const QVariant &value)
{
    Q_D(xTools);
    d->m_settings->setValue(key, value);
}

void xTools::settingsSetJsonObjectStringValue(const QString &key, const QString &value)
{
    Q_D(xTools);
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());
    if (doc.isNull()) {
        return;
    }

    d->m_settings->setValue(key, doc.toVariant());
}

QSettings *xTools::settings()
{
    Q_D(xTools);
    return d->m_settings;
}

QMainWindow *xTools::mainWindow()
{
    for (const auto &widget : qApp->topLevelWidgets()) {
        auto mainWindow = qobject_cast<QMainWindow *>(widget);
        if (mainWindow) {
            return mainWindow;
        }
    }

    return nullptr;
}

void xTools::moveToScreenCenter(QWidget *widget)
{
    QRect screenRect = QApplication::primaryScreen()->geometry();
    bool tooWidth = (widget->width() > screenRect.width());
    bool tooHeight = (widget->height() > screenRect.height());
    if (tooWidth || tooHeight) {
        widget->showMaximized();
        qInfo() << "The screen is too small.";
    } else {
        widget->move((screenRect.width() - widget->width()) / 2,
                     (screenRect.height() - widget->height()) / 2);
    }
}

bool xTools::tryToReboot()
{
    int ret = QMessageBox::information(
        nullptr,
        QObject::tr("Need to Reboot"),
        QObject::tr("The operation need to reboot to effected, reboot the application now?"),
        QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
        qApp->closeAllWindows();
        return true;
    }

    return false;
}

void xTools::tryToClearSettings()
{
    Q_D(xTools);
    if (!d->m_settings->value("clearSettings").toBool()) {
        return;
    }

    d->m_settings->setValue("value", false);
    QString path = d->settingsPath();
    QDir dir(path);
    if (dir.exists()) {
        if (dir.removeRecursively()) {
            qInfo() << "The settings path is removed.";
        } else {
            qWarning() << "The operation(remove settings path) failed!";
        }
    }
}
