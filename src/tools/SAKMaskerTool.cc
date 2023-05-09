/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#include "EDMaskerTool.hpp"

EDMaskerTool::EDMaskerTool(QObject *parent)
    : EDBaseTool{"ED.MaskTool", parent}
{

}

void EDMaskerTool::setMaskCode(qint8 maskCode)
{
    mMask = maskCode;
}

bool EDMaskerTool::initialize(QString &errStr)
{
    Q_UNUSED(errStr);
    return true;
}

void EDMaskerTool::inputBytesHandler(const QByteArray &bytes)
{
    if (!enable()) {
        outputBytes(bytes);
    } else {
        QByteArray cookedBytes;
        for (int i = 0; i < bytes.length(); i++) {
            quint8 value = quint8(bytes.at(i));
            value ^= mMask;
            cookedBytes.append(reinterpret_cast<char*>(&value), 1);
        }

        outputBytes(cookedBytes);
    }
}
