/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#if X_USING_QCRC_CALC
        CRC_3_GSM,
        CRC_3_ROHC,
        CRC_3_SAE_J1850,

        CRC_4_ITU,

        CRC_5_EPC,
        CRC_5_ITU,
        CRC_5_USB,

        CRC_6_CDMA2000_A,
        CRC_6_CDMA2000_B,
        CRC_6_DARC,
        CRC_6_ITU,
        CRC_6_GSM,

        CRC_7,
        CRC_7_ROHC,
        CRC_7_UMTS,

        CRC_8,
        CRC_8_AUTOSAR,
        CRC_8_CDMA2000,
        CRC_8_DARC,
        CRC_8_DVB_S2,
        CRC_8_EBU,
        CRC_8_GSM_A,
        CRC_8_GSM_B,
        CRC_8_I_CODE,
        CRC_8_ITU,
        CRC_8_LTE,
        CRC_8_MAXIM,
        CRC_8_OPENSAFETY,
        CRC_8_ROHC,
        CRC_8_SAE_J1850,
        CRC_8_WCDMA,
        CRC_10,
        CRC_10_CDMA2000,
        CRC_10_GSM,
        CRC_11,
        CRC_11_UMTS,
        CRC_12_3GPP,
        CRC_12_CDMA2000,
        CRC_12_DECT,
        CRC_12_GSM,
        CRC_13_BBC,
        CRC_14_DARC,
        CRC_14_GSM,
        CRC_15,
        CRC_15_MPT1327,

        CRC_16_ARC,
        CRC_16_AUG_CCITT,
        CRC_16_BUYPASS,
        CRC_16_CCITT_FALSE,
        CRC_16_CDMA2000,
        CRC_16_CMS,
        CRC_16_DDS_110,
        CRC_16_DECT_R,
        CRC_16_DECT_X,
        CRC_16_DNP,
        CRC_16_EN_13757,
        CRC_16_GENIBUS,
        CRC_16_GSM,
        CRC_16_LJ1200,
        CRC_16_MAXIM,
        CRC_16_MCRF4XX,
        CRC_16_OPENSAFETY_A,
        CRC_16_OPENSAFETY_B,
        CRC_16_PROFIBUS,
        CRC_16_RIELLO,
        CRC_16_T10_DIF,
        CRC_16_TELEDISK,
        CRC_16_TMS37157,
        CRC_16_USB,
        CRC_16_CRC_A,
        CRC_16_KERMIT,
        CRC_16_MODBUS,
        CRC_16_X_25,
        CRC_16_XMODEM,

        CRC_24,
        CRC_24_BLE,
        CRC_24_FLEXRAY_A,
        CRC_24_FLEXRAY_B,
        CRC_24_INTERLAKEN,
        CRC_24_LTE_A,
        CRC_24_LTE_B,

        CRC_30_CDMA,

        CRC_31_PHILIPS,

        CRC_32,
        CRC_32_AUTOSAR,
        CRC_32_BZIP2,
        CRC_32C,
        CRC_32D,
        CRC_32_MPEG_2,
        CRC_32_POSIX,
        CRC_32Q,
        CRC_32_JAMCRC,
        CRC_32_XFER,

        CRC_40_GSM,

        CRC_64,
        CRC_64_GO_ISO,
        CRC_64_WE,
        CRC_64_XZ,
#else
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
        CRC_32_MPEG2,
#endif
        SUM_8 = 0x00100000,
        SUM_16,
        SUM_32,
        SUM_64,
        LRC = 0x00200000
    };
    Q_ENUM(Algorithm);

    static QList<int> supportedAlgorithms(bool enableSumChecks = false, bool enableLrc = false);
    static QString algorithmName(Algorithm algorithm);
    static void setupAlgorithm(QComboBox *comboBox,
                               bool enableSumChecks = false,
                               bool enableLrc = false);
    static uint64_t poly(Algorithm algorithm);
    static uint64_t xorValue(Algorithm algorithm);
    static uint64_t initialValue(Algorithm algorithm);
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