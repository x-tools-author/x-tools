/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

namespace xMQTT {

class MqttDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MqttDataModel(QObject *parent = nullptr);
    ~MqttDataModel() override;
};

} // namespace xMQTT