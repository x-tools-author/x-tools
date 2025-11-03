/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "modbusregister.h"

namespace xModbus {

RegisterItem jsonObject2RegisterItem(const QJsonObject &json)
{
    RegisterItemKeys keys;
    RegisterItem item;
    int defaultType = static_cast<int>(QModbusDataUnit::Invalid);
    item.type = static_cast<QModbusDataUnit::RegisterType>(json.value(keys.type).toInt(defaultType));
    item.name = json.value(keys.name).toString(QString("Untitled"));
    item.unit = json.value(keys.unit).toString("");
    item.description = json.value(keys.description).toString("");
    item.address = static_cast<quint16>(json.value(keys.address).toInt(1));
    item.min = json.value(keys.min).toDouble(0);
    item.max = json.value(keys.max).toDouble(65535);
    item.decimals = json.value(keys.decimals).toInt(0);
    item.value = static_cast<qint16>(json.value(keys.value).toInt(0));
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
    json.insert(keus.value, item.value);
    return json;
}

} // namespace xModbus