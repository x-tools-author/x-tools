/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>
#include <QStringList>

class xToolsCrcInterface : public QObject
{
    Q_OBJECT
public:
    enum SAKEnumCrcAlgorithm {
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
    Q_ENUM(SAKEnumCrcAlgorithm)

public:
    xToolsCrcInterface(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE QString calculateString(const QString &bytes, int format);
    Q_INVOKABLE QByteArray calculateBytes(const QByteArray &bytes,
                                          int arithmetic,
                                          int startIndex,
                                          int endIndex,
                                          bool bigEndian = false);

    QStringList supportedParameterModels();
    uint32_t poly(xToolsCrcInterface::SAKEnumCrcAlgorithm model);
    uint32_t xorValue(xToolsCrcInterface::SAKEnumCrcAlgorithm model);
    uint32_t initialValue(xToolsCrcInterface::SAKEnumCrcAlgorithm model);
    QString friendlyPoly(xToolsCrcInterface::SAKEnumCrcAlgorithm model);
    bool isInputReversal(xToolsCrcInterface::SAKEnumCrcAlgorithm model);
    bool isOutputReversal(xToolsCrcInterface::SAKEnumCrcAlgorithm model);
    int bitsWidth(xToolsCrcInterface::SAKEnumCrcAlgorithm model);

public:
    template<typename T>
    T crcCalculate(uint8_t *input, uint64_t length, xToolsCrcInterface::SAKEnumCrcAlgorithm model)
    {
        T crcReg = static_cast<T>(initialValue(model));
        T rawPoly = static_cast<T>(poly(model));
        uint8_t byte = 0;

        T temp = 1;
        while (length--) {
            byte = *(input++);
            if (isInputReversal(model)) {
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

        if (isOutputReversal(model)) {
            reverseInt(crcReg, crcReg);
        }

        T crc = (crcReg ^ static_cast<T>(xorValue(model)));
        return crc;
    }

private:
    QStringList m_modelStrings;

private:
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
};
