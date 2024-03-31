/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCrcInterface.h"

#include <QDebug>
#include <QMetaEnum>

xToolsCrcInterface::xToolsCrcInterface(QObject *parent)
    : QObject(parent)
{}

QString xToolsCrcInterface::calculateString(const QString &bytes, int format)
{
    Q_UNUSED(bytes);
    Q_UNUSED(format);
    return QString();
}

QByteArray xToolsCrcInterface::calculateBytes(
    const QByteArray &bytes, int arithmetic, int startIndex, int endIndex, bool bigEndian)
{
    auto parametersIsValid = [&]() -> bool {
        if (bytes.isEmpty() && (startIndex == 0) && (endIndex == 0)) {
            return true;
        }

        if (!(startIndex >= 0 && startIndex < bytes.length())) {
            qWarning() << "start index is invalid:" << startIndex;
            return false;
        }

        if (!(endIndex >= 0 && endIndex < bytes.length())) {
            qWarning() << "end index is invalid:" << endIndex;
            return false;
        }

        int cookedEndIndex = bytes.length() - endIndex - 1;
        int len = cookedEndIndex - startIndex;
        if (len < 0) {
            qWarning() << "crc data length is invalid";
            return false;
        }

        return true;
    };

    QByteArray retBytes;
    auto bw = bitsWidth(SAKEnumCrcAlgorithm(arithmetic));
    if (parametersIsValid()) {
        uint64_t len = bytes.length() - startIndex - endIndex;
        QByteArray temp = bytes;
        if (bw == 8) {
            uint8_t ret = crcCalculate<uint8_t>(reinterpret_cast<uint8_t *>(temp.data())
                                                    + startIndex,
                                                len,
                                                SAKEnumCrcAlgorithm(arithmetic));
            retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
        } else if (bw == 16) {
            uint16_t ret = crcCalculate<uint16_t>(reinterpret_cast<uint8_t *>(temp.data())
                                                      + startIndex,
                                                  len,
                                                  SAKEnumCrcAlgorithm(arithmetic));
            retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
        } else if (bw == 32) {
            uint32_t ret = crcCalculate<uint32_t>(reinterpret_cast<uint8_t *>(temp.data())
                                                      + startIndex,
                                                  len,
                                                  SAKEnumCrcAlgorithm(arithmetic));
            retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
        }
    } else {
        if (bw == 8) {
            uint8_t ret = crcCalculate<uint8_t>(nullptr, 0, SAKEnumCrcAlgorithm(arithmetic));
            retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
        } else if (bw == 16) {
            uint16_t ret = crcCalculate<uint8_t>(nullptr, 0, SAKEnumCrcAlgorithm(arithmetic));
            retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
        } else if (bw == 32) {
            uint32_t ret = crcCalculate<uint8_t>(nullptr, 0, SAKEnumCrcAlgorithm(arithmetic));
            retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
        }
    }

    if (bigEndian) {
        QByteArray temp;
        for (int i = retBytes.length() - 1; i >= 0; i--) {
            temp.append(retBytes.at(i));
        }
        retBytes = temp;
    }

    return retBytes;
}

QStringList xToolsCrcInterface::supportedParameterModels()
{
    m_modelStrings.clear();
    QMetaEnum models = QMetaEnum::fromType<SAKEnumCrcAlgorithm>();

    const char *ch = Q_NULLPTR;
    for (int i = 0; i < models.keyCount(); i++) {
        ch = models.valueToKey(i);
        if (ch) {
            m_modelStrings.append(QString(ch));
        }
    }

    return m_modelStrings;
}

uint32_t xToolsCrcInterface::poly(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    uint32_t poly = 0;

    switch (model) {
    case CRC_8:
    case CRC_8_ITU:
    case CRC_8_ROHC:
        poly = 0x07;
        break;
    case CRC_8_MAXIM:
        poly = 0x31;
        break;
    case CRC_16_IBM:
    case CRC_16_MAXIM:
    case CRC_16_USB:
    case CRC_16_MODBUS:
        poly = 0x8005;
        break;
    case CRC_16_CCITT:
    case CRC_16_XMODEM:
    case CRC_16_CCITT_FALSE:
    case CRC_16_x25:
        poly = 0x1021;
        break;
    case CRC_16_DNP:
        poly = 0x3d65;
        break;
    case CRC_32:
    case CRC_32_MPEG2:
        poly = 0x04c11db7;
        break;
    }

    return poly;
}

uint32_t xToolsCrcInterface::xorValue(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    uint32_t value = 0;

    switch (model) {
    case CRC_8:
    case CRC_8_ROHC:
    case CRC_8_MAXIM:
        value = 0x00;
        break;
    case CRC_8_ITU:
        value = 0x55;
        break;
    case CRC_16_IBM:

    case CRC_16_MODBUS:
    case CRC_16_CCITT:
    case CRC_16_CCITT_FALSE:
    case CRC_16_XMODEM:
        value = 0x0000;
        break;
    case CRC_16_MAXIM:
    case CRC_16_USB:
    case CRC_16_x25:
    case CRC_16_DNP:
        value = 0xffff;
        break;
    case CRC_32:
        value = 0xffffffff;
        break;
    case CRC_32_MPEG2:
        value = 0x00000000;
        break;
    }

    return value;
}

uint32_t xToolsCrcInterface::initialValue(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    uint32_t init = 0;

    switch (model) {
    case CRC_8:
    case CRC_8_ITU:
    case CRC_8_MAXIM:
        init = 0x00;
        break;
    case CRC_8_ROHC:
        init = 0xff;
        break;
    case CRC_16_IBM:
    case CRC_16_MAXIM:
    case CRC_16_CCITT:
    case CRC_16_XMODEM:
    case CRC_16_DNP:
        init = 0x0000;
        break;
    case CRC_16_USB:
    case CRC_16_MODBUS:
    case CRC_16_CCITT_FALSE:
    case CRC_16_x25:
        init = 0xffff;
        break;
    case CRC_32:
    case CRC_32_MPEG2:
        init = 0xffffffff;
        break;
    }

    return init;
}

QString xToolsCrcInterface::friendlyPoly(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    QString formula = QString("Error: Formula not found");

    switch (model) {
    case CRC_8:
    case CRC_8_ITU:
    case CRC_8_ROHC:
        formula = QString("x8 + x2 + x + 1");
        break;
    case CRC_8_MAXIM:
        formula = QString("x8 + x5 + x4 + 1");
        break;
    case CRC_16_IBM:
    case CRC_16_MAXIM:
    case CRC_16_USB:
    case CRC_16_MODBUS:
    case CRC_16_CCITT:
    case CRC_16_CCITT_FALSE:
    case CRC_16_x25:
    case CRC_16_XMODEM:
        formula = QString("x6 + x2 + x5 + 1");
        break;
    case CRC_16_DNP:
        formula = QString("x6+x3+x2+x1+x0+x8+x6+x5+x2+1");
        break;
    case CRC_32:
    case CRC_32_MPEG2:
        formula = QString("x32+x6+x3+x2+x6+x2+x1+x0+x8+x7+x5+x4+x2+x+1");
        break;
    }

    return formula;
}

bool xToolsCrcInterface::isInputReversal(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    bool reversal = true;

    switch (model) {
    case CRC_8:
    case CRC_8_ITU:
    case CRC_16_CCITT_FALSE:
    case CRC_16_XMODEM:
    case CRC_32_MPEG2:
        reversal = false;
        break;

    case CRC_8_ROHC:
    case CRC_8_MAXIM:
    case CRC_16_IBM:
    case CRC_16_MAXIM:
    case CRC_16_USB:
    case CRC_16_MODBUS:
    case CRC_16_CCITT:
    case CRC_16_x25:
    case CRC_16_DNP:
    case CRC_32:
        reversal = true;
        break;
    }

    return reversal;
}

bool xToolsCrcInterface::isOutputReversal(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    bool reversal = true;

    switch (model) {
    case CRC_8:
    case CRC_8_ITU:
    case CRC_16_CCITT_FALSE:
    case CRC_16_XMODEM:
    case CRC_32_MPEG2:
        reversal = false;
        break;

    case CRC_8_ROHC:
    case CRC_8_MAXIM:
    case CRC_16_IBM:
    case CRC_16_MAXIM:
    case CRC_16_USB:
    case CRC_16_MODBUS:
    case CRC_16_CCITT:
    case CRC_16_x25:
    case CRC_16_DNP:
    case CRC_32:
        reversal = true;
        break;
    }

    return reversal;
}

int xToolsCrcInterface::bitsWidth(xToolsCrcInterface::SAKEnumCrcAlgorithm model)
{
    int ret = -1;
    switch (model) {
    case CRC_8:
    case CRC_8_ITU:
    case CRC_8_ROHC:
    case CRC_8_MAXIM:
        ret = 8;
        break;
    case CRC_16_IBM:
    case CRC_16_MAXIM:
    case CRC_16_USB:
    case CRC_16_MODBUS:
    case CRC_16_CCITT:
    case CRC_16_CCITT_FALSE:
    case CRC_16_x25:
    case CRC_16_XMODEM:
    case CRC_16_DNP:
        ret = 16;
        break;
    case CRC_32:
    case CRC_32_MPEG2:
        ret = 32;
        break;
    }
    return ret;
}
