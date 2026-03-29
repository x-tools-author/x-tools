/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCanBusFrame>
#include <QJsonObject>
#include <QString>

namespace xCanBus {

struct FrameItem
{
    QString name;
    bool cycle;
    int cycleInterval;
    bool response;
    QCanBusFrame frame;

    int elapsed{0}; // used for cycle sending, not saved in json
};

QJsonObject frameItemToJson(const FrameItem &item);
FrameItem frameItemFromJson(const QJsonObject &obj);

} // namespace xCanBus