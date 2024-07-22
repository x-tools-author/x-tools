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
#include <QLineEdit>
#include <QObject>

#define COMMON_UNKNOWN -1
#define COMMON_UNKNOWN_STR "Unknown"

class xIO : public QObject
{
    Q_OBJECT
public:
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
        WebSocketServer
    };
    static QList<int> supportedDeviceTypes();
    static QString deviceName(DeviceType type);

    /***************************************************************************************************/
    enum class TextFormat { Bin, Oct, Dec, Hex, Ascii, Utf8 };
    static QList<int> supportedTextFormats();
    static QString textFormatName(TextFormat format);
    static void setupTextFormat(QComboBox *comboBox);
    static QString bytes2string(const QByteArray &bytes, TextFormat format);
    static QByteArray string2bytes(const QString &text, TextFormat format);
    static void setupTextFormatValidator(QLineEdit *lineEdit, TextFormat format);

    /**************************************************************************************************/
    enum class Affixes { None, R, N, RN, NR };
    static QList<int> supportedAffixes();
    static QString additionName(Affixes affixes);
    static void setupAddition(QComboBox *comboBox);
    static QByteArray cookedAffixes(Affixes affixes);

    /**************************************************************************************************/
    enum class EscapeCharacter { None, R, N, RN, NR, R_N };
    static QList<int> supportedEscapeCharacters();
    static QString escapeCharacterName(EscapeCharacter character);
    static void setupEscapeCharacter(QComboBox *comboBox);
    static QString cookedEscapeCharacter(const QString &text, EscapeCharacter escapeCharacter);

    /**************************************************************************************************/
    enum class CrcAlgorithm {
        CRC_8,
        CRC_8_ITU,
        CRC_8_ROHC,
        CRC_8_MAXIM,
        CRC_16_IBM,
        CRC_16_MAXIM,
        CRC_16_USB,
        CRC_16_MODBUS,
        CRC_16_CCITT,
        CRC_16_CCITT_FALSE,
        CRC_16_x25,
        CRC_16_XMODEM,
        CRC_16_DNP,
        CRC_32,
        CRC_32_MPEG2
    };
    static QList<int> supportedCrcAlgorithms();
    static QString crcAlgorithmName(CrcAlgorithm algorithm);
    static void setupCrcAlgorithm(QComboBox *comboBox);
    static QByteArray calculateCrc(const QByteArray &data, CrcAlgorithm algorithm);
    static QByteArray calculateCrc(const QByteArray &data, CrcAlgorithm algorithm, bool bigEndian);
    // clang-format off
    static QByteArray calculateCrc(const QByteArray &data, CrcAlgorithm algorithm, int startIndex, int endIndex, bool bigEndian);
    // clang-format on

    /**************************************************************************************************/
    enum class WebSocketDataChannel { Text, Binary };
    static void setupWebSocketDataChannel(QComboBox *comboBox);

    /**************************************************************************************************/
    static QString jsonValue2hexString(const QJsonValue &value);
    static QJsonValue hexString2jsonValue(const QString &hexString);
    static void setupIp(QComboBox *cb);
    static QString systemDateFormat();
    static QString systemTimeFormat();
    static void try2reboot();
};