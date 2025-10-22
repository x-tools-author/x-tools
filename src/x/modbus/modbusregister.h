/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QModbusDataUnit>

namespace xModbus {

struct RegisterItemKeys
{
    const QString type{"type"};
    const QString name{"name"};
    const QString unit{"unit"};
    const QString description{"description"};
    const QString address{"address"};
    const QString min{"min"};
    const QString max{"max"};
    const QString decimals{"decimals"};
};

struct RegisterItem
{
    QModbusDataUnit::RegisterType type; // 寄存器类型
    QString name;                       // 名称
    QString unit;                       // 单位
    QString description;                // 描述（说明）
    quint16 address;                    // 地址
    double min;                         // 最小值
    double max;                         // 最大值
    int decimals;                       // 小数位
};

RegisterItem jsonObject2RegisterItem(const QJsonObject &json);
QJsonObject registerItem2JsonObject(const RegisterItem &item);

} // namespace xModbus