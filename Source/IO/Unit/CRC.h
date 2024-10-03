/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QComboBox>
#include <QList>
#include <QObject>
#include <QString>

namespace xTools {

class CRC : public QObject
{
    Q_OBJECT
public:
    enum class Algorithm {
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
    Q_ENUM(Algorithm);

    static QList<int> supportedAlgorithms();
    static QString algorithmName(Algorithm algorithm);
    static void setupAlgorithm(QComboBox *comboBox);
    static uint32_t poly(Algorithm algorithm);
    static uint32_t xorValue(Algorithm algorithm);
    static uint32_t initialValue(Algorithm algorithm);
    static bool isInputReversal(Algorithm algorithm);
    static bool isOutputReversal(Algorithm algorithm);
    static int bitsWidth(Algorithm algorithm);
    static QString friendlyPoly(Algorithm algorithm);

    static Q_INVOKABLE QByteArray calculate(const QByteArray &data, int algorithm);
    static Q_INVOKABLE QByteArray calculate(const QByteArray &data, int algorithm, bool bigEndian);

    struct Context
    {
        bool bigEndian;      // The result is big endian or little endian.
        Algorithm algorithm; // The algorithm to use.
        int startIndex;      // The start index of the data to calculate, it is start from left.
        int endIndex;        // The end index of the data to calculate, it is start from right.
        QByteArray data;     // The data
    };
    static QByteArray calculate(const Context &ctx);
};

} // namespace xTools
