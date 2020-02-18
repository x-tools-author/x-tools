/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qter188/QtSwissArmyKnife
 *     https://gitee.com/qter188/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is qter188@outlook.com.
 */
#include <QMetaEnum>

#include "SAKCRCInterface.hh"


SAKCRCInterface::SAKCRCInterface(QObject *parent)
    :QObject (parent)
{

}

QStringList SAKCRCInterface::supportedParameterModels()
{
    modelStrings.clear();
    QMetaEnum models = QMetaEnum::fromType<CRCModel>();

    const char *ch = nullptr;
    for (int i = 0; i < models.keyCount(); i++) {
        ch = models.valueToKey(i);
        if (ch){
            modelStrings.append(QString(ch));
        }
    }

    return modelStrings;
}

QString SAKCRCInterface::getPolyFormula(SAKCRCInterface::CRCModel model)
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

uint32_t SAKCRCInterface::getInitValue(SAKCRCInterface::CRCModel model)
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

uint32_t SAKCRCInterface::getPoly(SAKCRCInterface::CRCModel model)
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

uint32_t SAKCRCInterface::getXorValue(SAKCRCInterface::CRCModel model)
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

bool SAKCRCInterface::getInputReversal(SAKCRCInterface::CRCModel model)
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

bool SAKCRCInterface::getOutputReversal(SAKCRCInterface::CRCModel model)
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

int SAKCRCInterface::getBitsWidth(SAKCRCInterface::CRCModel model)
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
