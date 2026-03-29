/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistcommon.h"

#include <QDataStream>
#include <QIODevice>

namespace xCanBus {

struct FrameItemKeys
{
    const QString name{"name"};
    const QString cycle{"cycle"};
    const QString cycleInterval{"cycleInterval"};
    const QString response{"response"};
    const QString responseId{"responseId"};
    const QString frame{"frame"};
};

QJsonObject frameItemToJson(const FrameItem &item)
{
    FrameItemKeys keys;
    QJsonObject obj;
    obj[keys.name] = item.name;
    obj[keys.cycle] = item.cycle;
    obj[keys.cycleInterval] = item.cycleInterval;
    obj[keys.response] = item.response;

    QByteArray frameData;
    QDataStream stream(&frameData, QIODevice::WriteOnly);
    stream << item.frame;
    obj[keys.frame] = QString(frameData.toHex());

    return obj;
}

FrameItem frameItemFromJson(const QJsonObject &obj)
{
    FrameItemKeys keys;
    FrameItem item;
    item.name = obj[keys.name].toString(QObject::tr("Untitled"));
    item.cycle = obj[keys.cycle].toBool(false);
    item.cycleInterval = obj[keys.cycleInterval].toInt(1000);
    item.response = obj[keys.response].toBool(false);

    QByteArray frameData = QByteArray::fromHex(obj[keys.frame].toString().toUtf8());
    QDataStream stream(&frameData, QIODevice::ReadOnly);
    stream >> item.frame;

    return item;
}

} // namespace xCanBus