/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "registeritem.h"

namespace xModbus {

RegisterItem jsonObject2RegisterItem(const QJsonObject &json)
{
    RegisterItemKeys keus;
    RegisterItem item;
    int defaultType = static_cast<int>(QModbusDataUnit::Invalid);
    item.type = static_cast<QModbusDataUnit::RegisterType>(json.value(keus.type).toInt(defaultType));
    item.name = json.value(keus.name).toString(QString("Untitled"));
    item.unit = json.value(keus.unit).toString("");
    item.description = json.value(keus.description).toString("");
    item.address = static_cast<quint16>(json.value(keus.address).toInt(1));
    item.min = json.value(keus.min).toDouble(0);
    item.max = json.value(keus.max).toDouble(65535);
    item.decimals = json.value(keus.decimals).toInt(0);
    return item;
}

QJsonObject registerItem2JsonObject(const RegisterItem &item)
{
    RegisterItemKeys keus;
    QJsonObject json;
    json.insert(keus.type, static_cast<int>(item.type));
    json.insert(keus.name, item.name);
    json.insert(keus.unit, item.unit);
    json.insert(keus.description, item.description);
    json.insert(keus.address, static_cast<int>(item.address));
    json.insert(keus.min, item.min);
    json.insert(keus.max, item.max);
    json.insert(keus.decimals, item.decimals);
    return json;
}

} // namespace xModbus