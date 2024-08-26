/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xIO.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QMetaEnum>
#include <QNetworkInterface>
#include <QProcess>

#if defined(X_TOOLS_ENABLE_MODULE_SERIALPORT)
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include "../Common/CRC.h"

namespace xTools {

QList<int> xIO::supportedCommunicationTypes()
{
    static QList<int> deviceTypes;
    if (deviceTypes.isEmpty()) {
        deviceTypes << static_cast<int>(CommunicationType::SerialPort);
#ifdef X_TOOLS_ENABLE_MODULE_BLUETOOTH
        deviceTypes << static_cast<int>(CommunicationType::BleCentral);
#endif
#if 0
        deviceTypes << static_cast<int>(DeviceType::BlePeripheral);
#endif
        deviceTypes << static_cast<int>(CommunicationType::UdpClient);
        deviceTypes << static_cast<int>(CommunicationType::UdpServer);
        deviceTypes << static_cast<int>(CommunicationType::TcpClient);
        deviceTypes << static_cast<int>(CommunicationType::TcpServer);
        deviceTypes << static_cast<int>(CommunicationType::WebSocketClient);
        deviceTypes << static_cast<int>(CommunicationType::WebSocketServer);
    }

    return deviceTypes;
}

QString xIO::CommunicationName(xIO::CommunicationType type)
{
    switch (type) {
    case CommunicationType::SerialPort:
        return QObject::tr("Serial Port");
    case CommunicationType::BleCentral:
        return QObject::tr("BLE Central");
    case CommunicationType::BlePeripheral:
        return QObject::tr("BLE Peripheral");
    case CommunicationType::UdpClient:
        return QObject::tr("UDP Client");
    case CommunicationType::UdpServer:
        return QObject::tr("UDP Server");
    case CommunicationType::TcpClient:
        return QObject::tr("TCP Client");
    case CommunicationType::TcpServer:
        return QObject::tr("TCP Server");
    case CommunicationType::WebSocketClient:
        return QObject::tr("WebSocket Client");
    case CommunicationType::WebSocketServer:
        return QObject::tr("WebSocket Server");
    default:
        return "Unknown";
    }
}

void xIO::setupCommunicationTypes(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> deviceTypes = supportedCommunicationTypes();
    for (int type : deviceTypes) {
        comboBox->addItem(CommunicationName(static_cast<CommunicationType>(type)), type);
    }

    comboBox->setCurrentIndex(comboBox->findData(static_cast<int>(CommunicationType::SerialPort)));
}

QList<int> xIO::supportedTextFormats()
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

QString xIO::textFormatName(TextFormat format)
{
    switch (format) {
    case TextFormat::Bin:
        return QObject::tr("Binary");
    case TextFormat::Oct:
        return QObject::tr("Octal");
    case TextFormat::Dec:
        return QObject::tr("Decimal");
    case TextFormat::Hex:
        return QObject::tr("Hexadecimal");
    case TextFormat::Ascii:
        return QObject::tr("ASCII");
    case TextFormat::Utf8:
        return QObject::tr("UTF-8");
    default:
        return "Unknown";
    }
}

void xIO::setupTextFormat(QComboBox *comboBox)
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

QString xIO::bytes2string(const QByteArray &bytes, TextFormat format)
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
        return str;
    };

    if (TextFormat::Bin == format) {
        return cookedArray(bytes, 2, 8);
    } else if (TextFormat::Oct == format) {
        return cookedArray(bytes, 8, 3);
    } else if (TextFormat::Dec == format) {
        return cookedArray(bytes, 10, 3);
    } else if (TextFormat::Hex == format) {
        return cookedArray(bytes, 16, 2);
    } else if (TextFormat::Ascii == format) {
        return QString::fromLatin1(bytes);
    } else if (TextFormat::Utf8 == format) {
        return QString::fromUtf8(bytes);
    } else {
        return QString("Unsupported text format: %1").arg(static_cast<int>(format));
    }
}

QByteArray xIO::string2bytes(const QString &text, TextFormat format)
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
    if (format == TextFormat::Bin) {
        data = cookString(text, 2);
    } else if (format == TextFormat::Oct) {
        data = cookString(text, 8);
    } else if (format == TextFormat::Dec) {
        data = cookString(text, 10);
    } else if (format == TextFormat::Hex) {
        data = cookString(text, 16);
    } else if (format == TextFormat::Ascii) {
        data = text.toLatin1();
    } else {
        data = text.toUtf8();
    }

    return data;
}

void xIO::setupTextFormatValidator(QLineEdit *lineEdit, TextFormat format)
{
    static QMap<TextFormat, QRegularExpressionValidator *> regularExpressionMap;
    if (regularExpressionMap.isEmpty()) {
        QRegularExpressionValidator *urf8Validator = nullptr;
#if 0
        QRegularExpressionValidator *systemValidator = nullptr;
#endif

        const QString binStr = "([01][01][01][01][01][01][01][01][ ])*";
        const QString octStr = "^(0[0-7]{0,2}|[1-3][0-7]{2})( (0[0-7]{0,2}|[1-3][0-7]{2}))*$";
        const QString decStr = "^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9])( "
                               "(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]))*";
        const QString hexStr = "([0-9a-fA-F][0-9a-fA-F][ ])*";

        auto const binValidator = new QRegularExpressionValidator(QRegularExpression(binStr));
        auto const otcValidator = new QRegularExpressionValidator(
            QRegularExpression(octStr)); //0-377
        auto const decValidator = new QRegularExpressionValidator(
            QRegularExpression(decStr)); // 0-255;
        auto const hexValidator = new QRegularExpressionValidator(QRegularExpression(hexStr));
        auto const asciiValidator = new QRegularExpressionValidator(QRegularExpression("([ -~])*"));

        regularExpressionMap.insert(TextFormat::Bin, binValidator);
        regularExpressionMap.insert(TextFormat::Oct, otcValidator);
        regularExpressionMap.insert(TextFormat::Dec, decValidator);
        regularExpressionMap.insert(TextFormat::Hex, hexValidator);
        regularExpressionMap.insert(TextFormat::Ascii, asciiValidator);
        regularExpressionMap.insert(TextFormat::Utf8, urf8Validator);
    }

    if (!lineEdit || !regularExpressionMap.contains(format)) {
        qWarning() << "Invalid parameter, the operation will be ignored!";
        return;
    }

    lineEdit->setValidator(regularExpressionMap.value(format));
}

QList<int> xIO::supportedAffixes()
{
    QList<int> additions;
    additions << static_cast<int>(Affixes::None);
    additions << static_cast<int>(Affixes::R);
    additions << static_cast<int>(Affixes::N);
    additions << static_cast<int>(Affixes::RN);
    additions << static_cast<int>(Affixes::NR);
    return additions;
}

QString xIO::additionName(Affixes affixes)
{
    switch (affixes) {
    case Affixes::R:
        return "\\R";
    case Affixes::N:
        return "\\N";
    case Affixes::RN:
        return "\\R\\N";
    case Affixes::NR:
        return "\\N\\R";
    default:
        return QObject::tr("None");
    }
}

void xIO::setupAddition(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> additions = supportedAffixes();
    for (int addition : additions) {
        comboBox->addItem(additionName(static_cast<Affixes>(addition)), addition);
    }
}

QByteArray xIO::cookedAffixes(Affixes affixes)
{
    switch (affixes) {
    case Affixes::R:
        return QByteArray{"\r"};
    case Affixes::N:
        return QByteArray{"\n"};
    case Affixes::RN:
        return QByteArray{"\r\n"};
    case Affixes::NR:
        return QByteArray{"\n\r"};
    default:
        return QByteArray{};
    }
}

QList<int> xIO::supportedEscapeCharacters()
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

QString xIO::escapeCharacterName(EscapeCharacter escapeCharacter)
{
    switch (escapeCharacter) {
    case EscapeCharacter::R:
        return "\\r";
    case EscapeCharacter::N:
        return "\\n";
    case EscapeCharacter::RN:
        return "\\r\\n";
    case EscapeCharacter::NR:
        return "\\n\\r";
    case EscapeCharacter::R_N:
        return "\\r + \\n";
    default:
        return QObject::tr("None");
    }
}

void xIO::setupEscapeCharacter(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> escapeCharacters = supportedEscapeCharacters();
    for (int esc : escapeCharacters) {
        comboBox->addItem(escapeCharacterName(static_cast<EscapeCharacter>(esc)), esc);
    }
}

QString xIO::cookedEscapeCharacter(const QString &text, EscapeCharacter escapeCharacter)
{
    QString newStr = text;
    if (escapeCharacter == EscapeCharacter::R) {
        newStr.replace("\\r", "\r");
    } else if (escapeCharacter == EscapeCharacter::N) {
        newStr.replace("\\n", "\n");
    } else if (escapeCharacter == EscapeCharacter::RN) {
        newStr.replace("\\r\\n", "\r\n");
    } else if (escapeCharacter == EscapeCharacter::NR) {
        newStr.replace("\\n\\r", "\n\\r");
    } else if (escapeCharacter == EscapeCharacter::R_N) {
        newStr.replace("\\r", "\r");
        newStr.replace("\\n", "\n");
    }

    return newStr;
}

QString xIO::webSocketDataChannelName(WebSocketDataChannel channel)
{
    if (channel == WebSocketDataChannel::Text) {
        return tr("Text");
    } else if (channel == WebSocketDataChannel::Binary) {
        return tr("Binary");
    }

    return "Unknown";
}

void xIO::setupWebSocketDataChannel(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        auto text = WebSocketDataChannel::Text;
        auto binary = WebSocketDataChannel::Binary;
        comboBox->addItem(webSocketDataChannelName(text), static_cast<int>(text));
        comboBox->addItem(webSocketDataChannelName(binary), static_cast<int>(binary));
    }
}

QList<int> xIO::supportedResponseOptions()
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

QString xIO::responseOptionName(ResponseOption option)
{
    static QMap<ResponseOption, QString> map;
    if (map.isEmpty()) {
        map.insert(ResponseOption::Echo, tr("Echo"));
        map.insert(ResponseOption::Always, tr("Always"));
        map.insert(ResponseOption::InputEqualReference, tr("Input Equal Reference"));
        map.insert(ResponseOption::InputContainReference, tr("Input Contain Reference"));
        map.insert(ResponseOption::InputDoesNotContainReference,
                   tr("Input Does not Contain Reference"));
    }

    if (map.contains(option)) {
        return map[option];
    } else {
        return "Unknown";
    }
}

QString xIO::jsonValue2hexString(const QJsonValue &value)
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

QJsonValue xIO::hexString2jsonValue(const QString &hexString)
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

QString xIO::systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString xIO::systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

void xIO::try2reboot()
{
    int ret = QMessageBox::information(
        nullptr,
        QObject::tr("Need to Reboot"),
        QObject::tr("The operation need to reboot to effective, reboot the application now?"),
        QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath());
        QApplication::closeAllWindows();
    }
}

xIO::TextItem xIO::defaultTextItem()
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

QString xIO::textItem2string(const TextItem &context)
{
    QString prefix = additionName(context.prefix);
    QString payload = cookedEscapeCharacter(context.text, context.escapeCharacter);
    QString crc;
    if (context.crc.enable) {
        QByteArray data = string2bytes(context.text, context.textFormat);

        CRC::Context ctx;
        ctx.algorithm = static_cast<CRC::Algorithm>(context.crc.algorithm);
        ctx.startIndex = context.crc.startIndex;
        ctx.endIndex = context.crc.endIndex;
        ctx.bigEndian = context.crc.bigEndian;
        ctx.data = data;

        QByteArray crcArray = CRC::calculate(ctx);
        crc = QString::fromLatin1(crcArray.toHex());
    }

    QString suffix = additionName(context.suffix);
    return QString("[%1][%2][%3][%4]").arg(prefix, payload, crc, suffix);
}

QByteArray xIO::textItem2array(const TextItem &context)
{
    QByteArray prefix = xIO::cookedAffixes(static_cast<xIO::Affixes>(context.prefix));
    QString text = xIO::cookedEscapeCharacter(context.text, context.escapeCharacter);
    QByteArray payload = xIO::string2bytes(text, static_cast<xIO::TextFormat>(context.textFormat));

    CRC::Context ctx;
    ctx.algorithm = static_cast<CRC::Algorithm>(context.crc.algorithm);
    ctx.startIndex = context.crc.startIndex;
    ctx.endIndex = context.crc.endIndex;
    ctx.bigEndian = context.crc.bigEndian;
    ctx.data = payload;

    QByteArray crc = CRC::calculate(ctx);
    QByteArray suffix = xIO::cookedAffixes(static_cast<xIO::Affixes>(context.suffix));

    if (context.crc.enable) {
        return (prefix + payload + crc + suffix);
    } else {
        return (prefix + payload + suffix);
    }
}

xIO::TextItem xIO::loadTextItem(const QJsonObject &obj)
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

QJsonObject xIO::saveTextItem(const TextItem &context)
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

xIO::SerialPortItem xIO::defaultSerialPortItem()
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

QJsonObject xIO::saveSerialPortItem(const SerialPortItem &context)
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

xIO::SerialPortItem xIO::loadSerialPortItem(const QJsonObject &obj)
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

#if defined(X_TOOLS_ENABLE_MODULE_SERIALPORT)
void xIO::setupPortName(QComboBox *comboBox)
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (const auto &info : infos) {
        comboBox->addItem(info.portName());
    }
}

void xIO::setupBaudRate(QComboBox *comboBox)
{
    QList<int> baudRates = QSerialPortInfo::standardBaudRates();
    for (const auto &baudRate : baudRates) {
        comboBox->addItem(QString::number(baudRate), baudRate);
    }
    comboBox->setCurrentText("9600");
}

void xIO::setupDataBits(QComboBox *comboBox)
{
    comboBox->addItem("8", QSerialPort::Data8);
    comboBox->addItem("7", QSerialPort::Data7);
    comboBox->addItem("6", QSerialPort::Data6);
    comboBox->addItem("5", QSerialPort::Data5);
}

void xIO::setupParity(QComboBox *comboBox)
{
    comboBox->addItem(tr("None"), QSerialPort::NoParity);
    comboBox->addItem(tr("Even"), QSerialPort::EvenParity);
    comboBox->addItem(tr("Odd"), QSerialPort::OddParity);
    comboBox->addItem(tr("Space"), QSerialPort::SpaceParity);
    comboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
}

void xIO::setupStopBits(QComboBox *comboBox)
{
    comboBox->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    comboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    comboBox->addItem("2", QSerialPort::TwoStop);
}

void xIO::setupFlowControl(QComboBox *comboBox)
{
    comboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    comboBox->addItem(tr("Hardware(RTS/CTS)"), QSerialPort::HardwareControl);
    comboBox->addItem(tr("Software(X-ON/X-OFF)"), QSerialPort::SoftwareControl);
}
#endif

xIO::SocketItem xIO::defaultSocketItem()
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

QJsonObject xIO::saveSocketItem(const SocketItem &context)
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
    return obj;
}

xIO::SocketItem xIO::loadSocketItem(const QJsonObject &obj)
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
    return ctx;
}

void xIO::setupSocketAddress(QComboBox *cb)
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

void xIO::setupSocketPort(QSpinBox *spinBox)
{
    spinBox->setRange(0, 65535);
}

QList<int> xIO::supportedTransferTypes()
{
    static QList<int> types = {
        static_cast<int>(TransferType::Disabled),
        static_cast<int>(TransferType::Didirectional),
        static_cast<int>(TransferType::Unidirectional),
    };

    return types;
}

QString xIO::transferTypeName(int type)
{
    if (type == static_cast<int>(TransferType::Disabled)) {
        return tr("Diabled");
    } else if (type == static_cast<int>(TransferType::Didirectional)) {
        return tr("Didirectional");
    } else if (type == static_cast<int>(TransferType::Unidirectional)) {
        return tr("Unidirectional");
    } else {
        return "Unknown";
    }
}

void xIO::setupTransferType(QComboBox *comboBox)
{
    comboBox->clear();

    auto types = supportedTransferTypes();
    for (const auto &type : types) {
        comboBox->addItem(transferTypeName(type), type);
    }
}

} // namespace xTools
