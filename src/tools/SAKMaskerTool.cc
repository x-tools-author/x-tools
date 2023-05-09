/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKMaskerTool.hh"

SAKMaskerTool::SAKMaskerTool(QObject *parent)
    : SAKBaseTool{"ED.MaskTool", parent}
{

}

void SAKMaskerTool::setMaskCode(qint8 maskCode)
{
    mMask = maskCode;
}

void SAKMaskerTool::inputBytes(const QByteArray &bytes,
                               const QVariant &context)
{
    emit bytesInputted(bytes, context);

    if (!enable()) {
        emit bytesOutputted(bytes, context);
    } else {
        QByteArray cookedBytes;
        for (int i = 0; i < bytes.length(); i++) {
            quint8 value = quint8(bytes.at(i));
            value ^= mMask;
            cookedBytes.append(reinterpret_cast<char*>(&value), 1);
        }

        emit bytesOutputted(bytes, context);
    }
}
