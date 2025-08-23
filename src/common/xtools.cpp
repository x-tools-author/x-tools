/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "xtools.h"
#include "escape.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QObject>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QSettings>
#include <QSpinBox>
#include <QSplashScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QTranslator>
#include <QUrl>
#include <QVariant>

#if defined(X_ENABLE_SERIALPORT)
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

#include "common/crc.h"

#ifdef X_ENABLE_ICONV
#include <iconv.h>
#endif

QList<int> supportedTextFormats()
{
    static QList<int> textFormats;
    if (textFormats.isEmpty()) {
        textFormats << static_cast<int>(TextFormat::Bin);
        textFormats << static_cast<int>(TextFormat::Oct);
        textFormats << static_cast<int>(TextFormat::Dec);
        textFormats << static_cast<int>(TextFormat::Hex);
        textFormats << static_cast<int>(TextFormat::HexWithoutSpace);
        textFormats << static_cast<int>(TextFormat::Ascii);
        textFormats << static_cast<int>(TextFormat::Utf8);
#if defined(X_ENABLE_ICONV)
        textFormats << static_cast<int>(TextFormat::GB2312);
        textFormats << static_cast<int>(TextFormat::CSGB2312);
        textFormats << static_cast<int>(TextFormat::GBK);
        textFormats << static_cast<int>(TextFormat::GB18030);
#endif
    }

    return textFormats;
}

QString textFormatName(TextFormat format)
{
    switch (format) {
    case TextFormat::Bin:
        return QObject::tr("Bin");
    case TextFormat::Oct:
        return QObject::tr("Oct");
    case TextFormat::Dec:
        return QObject::tr("Dec");
    case TextFormat::Hex:
        return QObject::tr("Hex");
    case TextFormat::HexWithoutSpace:
        return QObject::tr("Hex(No Space)");
    case TextFormat::Ascii:
        return QObject::tr("ASCII");
    case TextFormat::Utf8:
        return QObject::tr("UTF-8");
    case TextFormat::GB2312:
        return QString("GB2312");
    case TextFormat::CSGB2312:
        return QString("CSGB2312");
    case TextFormat::GBK:
        return QString("GBK");
    case TextFormat::GB18030:
        return QString("GB18030");

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
    for (int &format : textFormats) {
        comboBox->addItem(textFormatName(static_cast<TextFormat>(format)), format);
    }

    comboBox->setCurrentIndex(comboBox->findData(static_cast<int>(TextFormat::Hex)));
}

QByteArray convertEncoding(const QByteArray &input, const char *fromCharset, const char *toCharset)
{
#if defined(X_ENABLE_ICONV)
    iconv_t cd = iconv_open(toCharset, fromCharset);
    if (cd == (iconv_t) -1) {
        return QByteArray();
    }

    size_t inBytesLeft = input.size();
    size_t outBytesLeft = inBytesLeft * 4 + 1;
    QByteArray output(outBytesLeft, 0);
    const char *inBuf = input.constData();
    char *outBuf = output.data();
#if defined(Q_OS_WIN32)
    size_t result = iconv(cd, &inBuf, &inBytesLeft, &outBuf, &outBytesLeft);
#else
    char *tmp = const_cast<char *>(inBuf);
    size_t result = iconv(cd, &tmp, &inBytesLeft, &outBuf, &outBytesLeft);
#endif
    iconv_close(cd);

    if (result == (size_t) -1) {
        return input;
    }

    output.resize(output.size() - outBytesLeft);
    return output;
#else
    Q_UNUSED(fromCharset);
    Q_UNUSED(toCharset);
    return input;
#endif
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
    } else if (static_cast<int>(TextFormat::HexWithoutSpace) == format) {
        return cookedArray(bytes, 16, 2).replace(" ", "");
    } else if (static_cast<int>(TextFormat::Ascii) == format) {
        return QString::fromLatin1(bytes);
    } else if (static_cast<int>(TextFormat::Utf8) == format) {
        return QString::fromUtf8(bytes);
    }
#if defined(X_ENABLE_ICONV)
    else if (static_cast<int>(TextFormat::GB2312) == format) {
        QString name = textFormatName(TextFormat::GB2312);
        QByteArray utf8 = convertEncoding(bytes, name.toLatin1().data(), "UTF-8");
        return QString::fromUtf8(utf8);
    } else if (static_cast<int>(TextFormat::CSGB2312) == format) {
        QString name = textFormatName(TextFormat::CSGB2312);
        QByteArray utf8 = convertEncoding(bytes, name.toLatin1().data(), "UTF-8");
        return QString::fromUtf8(utf8);
    } else if (static_cast<int>(TextFormat::GBK) == format) {
        QString name = textFormatName(TextFormat::GBK);
        QByteArray utf8 = convertEncoding(bytes, name.toLatin1().data(), "UTF-8");
        return QString::fromUtf8(utf8);
    } else if (static_cast<int>(TextFormat::GB18030) == format) {
        QString name = textFormatName(TextFormat::GB18030);
        QByteArray utf8 = convertEncoding(bytes, name.toLatin1().data(), "UTF-8");
        return QString::fromUtf8(utf8);
    }
#endif
    else {
        return QString("Unsupported text format: %1").arg(static_cast<int>(format));
    }
}

QByteArray string2bytes(const QString &text, int format)
{
    auto cookString = [](const QString &str, const int base) -> QByteArray {
        QByteArray data;
        const QStringList strList = str.split(' ', xSkipEmptyParts);
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
    } else if (format == static_cast<int>(TextFormat::HexWithoutSpace)) {
        if (text.length() % 2 != 0) {
            QString paddedText = text + "0"; // Pad with '0' if odd length
            data = QByteArray::fromHex(paddedText.toLatin1());
        } else {
            data = QByteArray::fromHex(text.toLatin1());
        }
    } else if (format == static_cast<int>(TextFormat::Ascii)) {
        data = text.toLatin1();
    }
#if defined(X_ENABLE_ICONV)
    else if (format == static_cast<int>(TextFormat::GB2312)) {
        QString name = textFormatName(TextFormat::GB2312);
        data = convertEncoding(text.toUtf8(), "UTF-8", name.toLatin1().data());
    } else if (format == static_cast<int>(TextFormat::CSGB2312)) {
        QString name = textFormatName(TextFormat::CSGB2312);
        data = convertEncoding(text.toUtf8(), "UTF-8", name.toLatin1().data());
    } else if (format == static_cast<int>(TextFormat::GBK)) {
        QString name = textFormatName(TextFormat::GBK);
        data = convertEncoding(text.toUtf8(), "UTF-8", name.toLatin1().data());
    } else if (format == static_cast<int>(TextFormat::GB18030)) {
        QString name = textFormatName(TextFormat::GB18030);
        data = convertEncoding(text.toUtf8(), "UTF-8", name.toLatin1().data());
    }
#endif
    else {
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
    if (!lineEdit) {
        return;
    }

    static QMap<int, QRegularExpressionValidator *> regularExpressionMap;
    if (regularExpressionMap.isEmpty()) {
        // clang-format off
        const QString binStr = "([01][01][01][01][01][01][01][01][ ])*";
        const QString octStr = "^(0[0-7]{0,2}|[1-3][0-7]{2})( (0[0-7]{0,2}|[1-3][0-7]{2}))*";
        const QString decStr = "^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9])( (25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]))*";
        const QString hexStr = "([0-9a-fA-F][0-9a-fA-F][ ])*";
        const QString hexWithoutSpaceStr = "([0-9a-fA-F][0-9a-fA-F])*"; // No space between hex digits
        // clang-format on

        // clang-format off
        auto const binValidator = new QRegularExpressionValidator(QRegularExpression(binStr));
        auto const otcValidator = new QRegularExpressionValidator(QRegularExpression(octStr)); //0-377
        auto const decValidator = new QRegularExpressionValidator(QRegularExpression(decStr)); // 0-255;
        auto const hexValidator = new QRegularExpressionValidator(QRegularExpression(hexStr));
        auto const hexWithoutSpaceValidator = new QRegularExpressionValidator(QRegularExpression(hexWithoutSpaceStr));
        auto const asciiValidator = new QRegularExpressionValidator(QRegularExpression("([ -~])*"));
        // clang-format on

        regularExpressionMap.insert(static_cast<int>(TextFormat::Bin), binValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Oct), otcValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Dec), decValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Hex), hexValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::HexWithoutSpace),
                                    hexWithoutSpaceValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Ascii), asciiValidator);
        regularExpressionMap.insert(static_cast<int>(TextFormat::Utf8), nullptr);
    }

    if (regularExpressionMap.contains(format)) {
        lineEdit->setMaxLength(maxLen);
        lineEdit->setValidator(regularExpressionMap.value(format));
    } else {
        lineEdit->setMaxLength(maxLen);
        lineEdit->setValidator(nullptr);
    }
}

int print_encoding(unsigned int namescount, const char *const *names, void *data)
{
    for (unsigned int i = 0; i < namescount; ++i) {
        qInfo() << names[i];
    }
    return 0;
}

void printSupportedIconvEncodings()
{
#ifdef X_ENABLE_ICONV
    iconvlist(print_encoding, nullptr);
#else
    qWarning() << "Iconv is not enabled in this build.";
#endif
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
        return QObject::tr("None");
    }
}

void setupAddition(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> additions = supportedAffixes();
    for (int &addition : additions) {
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
    escapeCharacters << static_cast<int>(EscapeCharacter::C);
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
    case static_cast<int>(EscapeCharacter::C):
        return "C-Style";
    default:
        return QObject::tr("None");
    }
}

void setupEscapeCharacter(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> escapeCharacters = supportedEscapeCharacters();
    for (int &esc : escapeCharacters) {
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
    } else if (escapeCharacter == static_cast<int>(EscapeCharacter::C)) {
        newStr = cEscape(text);
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
        for (int &option : options) {
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
    context.ignoredBusyDevices = false;
    context.ignoredBusyDevices = false;
    context.optimizedFrame = false;

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
    obj.insert(keys.ignoredBusyDevices, context.ignoredBusyDevices);
    obj.insert(keys.optimizedFrame, context.optimizedFrame);
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
    ctx.ignoredBusyDevices = obj.value(keys.ignoredBusyDevices).toBool();
    ctx.optimizedFrame = obj.value(keys.optimizedFrame).toBool();
    return ctx;
}

#if defined(X_ENABLE_SERIALPORT)
void setupPortName(QComboBox *comboBox)
{
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    for (auto &info : infos) {
        comboBox->addItem(info.portName());
    }
}

void setupBaudRate(QComboBox *comboBox)
{
    QList<int> baudRates = QSerialPortInfo::standardBaudRates();
    for (auto &baudRate : baudRates) {
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
    item.serverAddress = "127.0.0.1";
    item.serverPort = 54687;
    item.dataChannel = WebSocketDataChannel::Text;
    item.authentication = false;
    item.username = "";
    item.password = "";
    item.multicastAddress = "239.168.3.255";
    item.multicastPort = 53625;
    item.enableMulticast = false;
    item.justMulticast = false;
    item.path = "";
    return item;
}

QVariantMap saveSocketItem(const SocketItem &context)
{
    QVariantMap obj;
    const SocketItemKeys keys;
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
    obj.insert(keys.path, context.path);
    obj.insert(keys.secureMode, context.secureMode);
    return obj;
}

SocketItem loadSocketItem(const QVariantMap &obj)
{
    SocketItem ctx;
    const SocketItemKeys keys;
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
    ctx.path = obj.value(keys.path).toString();
    ctx.secureMode = obj.value(keys.secureMode).toBool();
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
    for (auto &type : types) {
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
        return QObject::tr("Binary"); // + "-Y";
    } else if (format == static_cast<int>(ChartsDataFormat::BinaryXY)) {
        return QObject::tr("Binary") + "-XY";
    } else if (format == static_cast<int>(ChartsDataFormat::TextY)) {
        return QObject::tr("Text"); // + "-Y";
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
    for (auto &format : formats) {
        comboBox->addItem(chartsDataFormatName(format), format);
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
