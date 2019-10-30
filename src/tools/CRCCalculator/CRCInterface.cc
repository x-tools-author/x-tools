/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */

#include <QMetaEnum>

#include "CRCInterface.hh"

CRCInterface::CRCInterface(QObject *parent)
    :QObject (parent)
{

}

uint32_t CRCInterface::getInitValue(CRCInterface::CRCModel model)
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

uint32_t CRCInterface::getPoly(CRCInterface::CRCModel model)
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

uint32_t CRCInterface::getXorValue(CRCInterface::CRCModel model)
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
    case CRC_16_MAXIM:
    case CRC_16_MODBUS:
    case CRC_16_CCITT:
    case CRC_16_CCITT_FALSE:
    case CRC_16_XMODEM:
        value = 0x0000;
        break;
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

bool CRCInterface::getInputReversal(CRCInterface::CRCModel model)
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

bool CRCInterface::getOutputReversal(CRCInterface::CRCModel model)
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

QStringList CRCInterface::getSupportParameterModels()
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

QString CRCInterface::getPolyFormula(CRCInterface::CRCModel model)
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

int CRCInterface::getBitsWidth(CRCInterface::CRCModel model)
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
