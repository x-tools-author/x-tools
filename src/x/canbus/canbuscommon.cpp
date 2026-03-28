/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "canbuscommon.h"

#include <QCanBus>

namespace xCanBus {

void xSetupFrameTypes(QComboBox* cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    cb->addItem(QObject::tr("Data Frame"), QVariant(QCanBusFrame::DataFrame));
    cb->addItem(QObject::tr("Remote Request Frame"), QVariant(QCanBusFrame::RemoteRequestFrame));
    cb->addItem(QObject::tr("Error Frame"), QVariant(QCanBusFrame::ErrorFrame));
}

void xSetupFrameIdentifiers(QComboBox* cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    cb->addItem(QObject::tr("Standard"), 11);
    cb->addItem(QObject::tr("Extended"), 29);
}

} // namespace xCanBus