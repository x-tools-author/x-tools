/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "crc.h"

#include <utility>

QList<int> CRC::supportedAlgorithms()
{
    QList<int> Algorithms;
    Algorithms << static_cast<int>(Algorithm::CRC_8);
    Algorithms << static_cast<int>(Algorithm::CRC_8_ITU);
    Algorithms << static_cast<int>(Algorithm::CRC_8_ROHC);
    Algorithms << static_cast<int>(Algorithm::CRC_8_MAXIM);
    Algorithms << static_cast<int>(Algorithm::CRC_16_IBM);
    Algorithms << static_cast<int>(Algorithm::CRC_16_MAXIM);
    Algorithms << static_cast<int>(Algorithm::CRC_16_USB);
    Algorithms << static_cast<int>(Algorithm::CRC_16_MODBUS);
    Algorithms << static_cast<int>(Algorithm::CRC_16_CCITT);
    Algorithms << static_cast<int>(Algorithm::CRC_16_CCITT_FALSE);
    Algorithms << static_cast<int>(Algorithm::CRC_16_x25);
    Algorithms << static_cast<int>(Algorithm::CRC_16_XMODEM);
    Algorithms << static_cast<int>(Algorithm::CRC_16_DNP);
    Algorithms << static_cast<int>(Algorithm::CRC_32);
    Algorithms << static_cast<int>(Algorithm::CRC_32_MPEG2);
    return Algorithms;
}

QString CRC::algorithmName(Algorithm Algorithm)
{
    switch (Algorithm) {
    case Algorithm::CRC_8:
        return "CRC-8";
    case Algorithm::CRC_8_ITU:
        return "CRC-8/ITU";
    case Algorithm::CRC_8_ROHC:
        return "CRC-8/ROHC";
    case Algorithm::CRC_8_MAXIM:
        return "CRC-8/MAXIM";
    case Algorithm::CRC_16_IBM:
        return "CRC-16/IBM";
    case Algorithm::CRC_16_MAXIM:
        return "CRC-16/MAXIM";
    case Algorithm::CRC_16_USB:
        return "CRC-16/USB";
    case Algorithm::CRC_16_MODBUS:
        return "CRC-16/MODBUS";
    case Algorithm::CRC_16_CCITT:
        return "CRC-16/CCITT";
    case Algorithm::CRC_16_CCITT_FALSE:
        return "CRC-16/CCITT-FALSE";
    case Algorithm::CRC_16_x25:
        return "CRC-16/X-25";
    case Algorithm::CRC_16_XMODEM:
        return "CRC-16/XMODEM";
    case Algorithm::CRC_16_DNP:
        return "CRC-16/DNP";
    case Algorithm::CRC_32:
        return "CRC-32";
    case Algorithm::CRC_32_MPEG2:
        return "CRC-32/MPEG-2";
    default:
        return QObject::tr("Unknown");
    }
}

void CRC::setupAlgorithm(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    QList<int> algorithms = supportedAlgorithms();
    for (const int &algorithm : std::as_const(algorithms)) {
        comboBox->addItem(algorithmName(static_cast<Algorithm>(algorithm)), algorithm);
    }
}

uint32_t CRC::poly(CRC::Algorithm algorithm)
{
    uint32_t poly = 0;

    switch (algorithm) {
    case CRC::Algorithm::CRC_8:
    case CRC::Algorithm::CRC_8_ITU:
    case CRC::Algorithm::CRC_8_ROHC:
        poly = 0x07;
        break;
    case CRC::Algorithm::CRC_8_MAXIM:
        poly = 0x31;
        break;
    case CRC::Algorithm::CRC_16_IBM:
    case CRC::Algorithm::CRC_16_MAXIM:
    case CRC::Algorithm::CRC_16_USB:
    case CRC::Algorithm::CRC_16_MODBUS:
        poly = 0x8005;
        break;
    case CRC::Algorithm::CRC_16_CCITT:
    case CRC::Algorithm::CRC_16_XMODEM:
    case CRC::Algorithm::CRC_16_CCITT_FALSE:
    case CRC::Algorithm::CRC_16_x25:
        poly = 0x1021;
        break;
    case CRC::Algorithm::CRC_16_DNP:
        poly = 0x3d65;
        break;
    case CRC::Algorithm::CRC_32:
    case CRC::Algorithm::CRC_32_MPEG2:
        poly = 0x04c11db7;
        break;
    }

    return poly;
}

uint32_t CRC::xorValue(CRC::Algorithm algorithm)
{
    uint32_t value = 0;

    switch (algorithm) {
    case CRC::Algorithm::CRC_8:
    case CRC::Algorithm::CRC_8_ROHC:
    case CRC::Algorithm::CRC_8_MAXIM:
        value = 0x00;
        break;
    case CRC::Algorithm::CRC_8_ITU:
        value = 0x55;
        break;
    case CRC::Algorithm::CRC_16_IBM:
    case CRC::Algorithm::CRC_16_MODBUS:
    case CRC::Algorithm::CRC_16_CCITT:
    case CRC::Algorithm::CRC_16_CCITT_FALSE:
    case CRC::Algorithm::CRC_16_XMODEM:
        value = 0x0000;
        break;
    case CRC::Algorithm::CRC_16_MAXIM:
    case CRC::Algorithm::CRC_16_USB:
    case CRC::Algorithm::CRC_16_x25:
    case CRC::Algorithm::CRC_16_DNP:
        value = 0xffff;
        break;
    case CRC::Algorithm::CRC_32:
        value = 0xffffffff;
        break;
    case CRC::Algorithm::CRC_32_MPEG2:
        value = 0x00000000;
        break;
    }

    return value;
}

uint32_t CRC::initialValue(CRC::Algorithm algorithm)
{
    uint32_t init = 0;

    switch (algorithm) {
    case CRC::Algorithm::CRC_8:
    case CRC::Algorithm::CRC_8_ITU:
    case CRC::Algorithm::CRC_8_MAXIM:
        init = 0x00;
        break;
    case CRC::Algorithm::CRC_8_ROHC:
        init = 0xff;
        break;
    case CRC::Algorithm::CRC_16_IBM:
    case CRC::Algorithm::CRC_16_MAXIM:
    case CRC::Algorithm::CRC_16_CCITT:
    case CRC::Algorithm::CRC_16_XMODEM:
    case CRC::Algorithm::CRC_16_DNP:
        init = 0x0000;
        break;
    case CRC::Algorithm::CRC_16_USB:
    case CRC::Algorithm::CRC_16_MODBUS:
    case CRC::Algorithm::CRC_16_CCITT_FALSE:
    case CRC::Algorithm::CRC_16_x25:
        init = 0xffff;
        break;
    case CRC::Algorithm::CRC_32:
    case CRC::Algorithm::CRC_32_MPEG2:
        init = 0xffffffff;
        break;
    }

    return init;
}

bool CRC::isInputReversal(CRC::Algorithm algorithm)
{
    bool reversal = true;

    switch (algorithm) {
    case CRC::Algorithm::CRC_8:
    case CRC::Algorithm::CRC_8_ITU:
    case CRC::Algorithm::CRC_16_CCITT_FALSE:
    case CRC::Algorithm::CRC_16_XMODEM:
    case CRC::Algorithm::CRC_32_MPEG2:
        reversal = false;
        break;
    case CRC::Algorithm::CRC_8_ROHC:
    case CRC::Algorithm::CRC_8_MAXIM:
    case CRC::Algorithm::CRC_16_IBM:
    case CRC::Algorithm::CRC_16_MAXIM:
    case CRC::Algorithm::CRC_16_USB:
    case CRC::Algorithm::CRC_16_MODBUS:
    case CRC::Algorithm::CRC_16_CCITT:
    case CRC::Algorithm::CRC_16_x25:
    case CRC::Algorithm::CRC_16_DNP:
    case CRC::Algorithm::CRC_32:
        reversal = true;
        break;
    }

    return reversal;
}

bool CRC::isOutputReversal(CRC::Algorithm algorithm)
{
    bool reversal = true;

    switch (algorithm) {
    case CRC::Algorithm::CRC_8:
    case CRC::Algorithm::CRC_8_ITU:
    case CRC::Algorithm::CRC_16_CCITT_FALSE:
    case CRC::Algorithm::CRC_16_XMODEM:
    case CRC::Algorithm::CRC_32_MPEG2:
        reversal = false;
        break;
    case CRC::Algorithm::CRC_8_ROHC:
    case CRC::Algorithm::CRC_8_MAXIM:
    case CRC::Algorithm::CRC_16_IBM:
    case CRC::Algorithm::CRC_16_MAXIM:
    case CRC::Algorithm::CRC_16_USB:
    case CRC::Algorithm::CRC_16_MODBUS:
    case CRC::Algorithm::CRC_16_CCITT:
    case CRC::Algorithm::CRC_16_x25:
    case CRC::Algorithm::CRC_16_DNP:
    case CRC::Algorithm::CRC_32:
        reversal = true;
        break;
    }

    return reversal;
}

int CRC::bitsWidth(CRC::Algorithm algorithm)
{
    int ret = -1;
    switch (algorithm) {
    case CRC::Algorithm::CRC_8:
    case CRC::Algorithm::CRC_8_ITU:
    case CRC::Algorithm::CRC_8_ROHC:
    case CRC::Algorithm::CRC_8_MAXIM:
        ret = 8;
        break;
    case CRC::Algorithm::CRC_16_IBM:
    case CRC::Algorithm::CRC_16_MAXIM:
    case CRC::Algorithm::CRC_16_USB:
    case CRC::Algorithm::CRC_16_MODBUS:
    case CRC::Algorithm::CRC_16_CCITT:
    case CRC::Algorithm::CRC_16_CCITT_FALSE:
    case CRC::Algorithm::CRC_16_x25:
    case CRC::Algorithm::CRC_16_XMODEM:
    case CRC::Algorithm::CRC_16_DNP:
        ret = 16;
        break;
    case CRC::Algorithm::CRC_32:
    case CRC::Algorithm::CRC_32_MPEG2:
        ret = 32;
        break;
    }
    return ret;
}

QString CRC::friendlyPoly(Algorithm algorithm)
{
    QString formula = QString("Error: Formula not found");

    switch (algorithm) {
    case Algorithm::CRC_8:
    case Algorithm::CRC_8_ITU:
    case Algorithm::CRC_8_ROHC:
        formula = QString("x8 + x2 + x + 1");
        break;
    case Algorithm::CRC_8_MAXIM:
        formula = QString("x8 + x5 + x4 + 1");
        break;
    case Algorithm::CRC_16_IBM:
    case Algorithm::CRC_16_MAXIM:
    case Algorithm::CRC_16_USB:
    case Algorithm::CRC_16_MODBUS:
    case Algorithm::CRC_16_CCITT:
    case Algorithm::CRC_16_CCITT_FALSE:
    case Algorithm::CRC_16_x25:
    case Algorithm::CRC_16_XMODEM:
        formula = QString("x6 + x2 + x5 + 1");
        break;
    case Algorithm::CRC_16_DNP:
        formula = QString("x6+x3+x2+x1+x0+x8+x6+x5+x2+1");
        break;
    case Algorithm::CRC_32:
    case Algorithm::CRC_32_MPEG2:
        formula = QString("x32+x6+x3+x2+x6+x2+x1+x0+x8+x7+x5+x4+x2+x+1");
        break;
    }

    return formula;
}

template<typename T>
bool reverseInt(const T &input, T &output)
{
    int bitsWidth = sizeof(input) * 8;
    QString inputStr = QString("%1").arg(QString::number(input, 2), bitsWidth, '0');
    QString outputStr;
    outputStr.resize(bitsWidth);
    for (int i = 0; i < bitsWidth; i++) {
        outputStr.replace(i, 1, inputStr.at(bitsWidth - 1 - i));
    }

    bool ok;
    output = static_cast<T>(outputStr.toULongLong(&ok, 2));
    return ok;
}

template<typename T>
T crcCalculate(const uint8_t *input, uint64_t length, CRC::Algorithm algorithm)
{
    T crcReg = static_cast<T>(CRC::initialValue(algorithm));
    T rawPoly = static_cast<T>(CRC::poly(algorithm));
    uint8_t byte = 0;

    T temp = 1;
    while (length--) {
        byte = *(input++);
        if (CRC::isInputReversal(algorithm)) {
            reverseInt(byte, byte);
        }

        crcReg ^= static_cast<T>((byte << 8 * (sizeof(T) - 1)));
        for (int i = 0; i < 8; i++) {
            if (crcReg & (temp << (sizeof(T) * 8 - 1))) {
                crcReg = static_cast<T>((crcReg << 1) ^ rawPoly);
            } else {
                crcReg = static_cast<T>(crcReg << 1);
            }
        }
    }

    if (CRC::isOutputReversal(algorithm)) {
        reverseInt(crcReg, crcReg);
    }

    T crc = (crcReg ^ static_cast<T>(CRC::xorValue(algorithm)));
    return crc;
}

QByteArray CRC::calculate(const QByteArray &data, int algorithm)
{
    QByteArray retBytes;
    auto cookedAlgorithm = static_cast<CRC::Algorithm>(algorithm);
    auto const bw = bitsWidth(cookedAlgorithm);
    auto *ptr = reinterpret_cast<const uint8_t *>(data.constData());
    if (bw == 8) {
        auto ret = crcCalculate<uint8_t>(ptr, data.length(), cookedAlgorithm);
        retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
    } else if (bw == 16) {
        auto ret = crcCalculate<uint16_t>(ptr, data.length(), cookedAlgorithm);
        retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
    } else if (bw == 32) {
        auto ret = crcCalculate<uint32_t>(ptr, data.length(), cookedAlgorithm);
        retBytes = QByteArray(reinterpret_cast<char *>(&ret), sizeof(ret));
    }

    return retBytes;
}

QByteArray CRC::calculate(const QByteArray &data, int algorithm, bool bigEndian)
{
    QByteArray retBytes = calculate(data, algorithm);
    if (bigEndian) {
        std::reverse(retBytes.begin(), retBytes.end());
    }

    return retBytes;
}

QByteArray CRC::calculate(const Context &ctx)
{
    const int length = static_cast<int>(ctx.data.length()) - ctx.startIndex - ctx.endIndex;
    const QByteArray tmpData = ctx.data.mid(ctx.startIndex, length);
    return calculate(tmpData, static_cast<int>(ctx.algorithm), ctx.bigEndian);
}
