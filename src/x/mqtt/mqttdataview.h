/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

#include "utilities/serializable.h"

namespace Ui {
class MqttDataView;
} // namespace Ui

namespace xMQTT {

class MqttDataModel;
class MqttDataFilter;
class MqttDataView : public QWidget, public xTools::Serializable
{
    Q_OBJECT
public:
    explicit MqttDataView(QWidget *parent = nullptr);
    ~MqttDataView() override;
    QJsonObject save() override;
    void load(const QJsonObject &obj) override;

    MqttDataModel *model() const;

private:
    Ui::MqttDataView *ui;
    MqttDataModel *m_model{nullptr};
    MqttDataFilter *m_filter{nullptr};

private:
    void onRowInserted();
    void onPingPongToggled(bool checked);
    void onFilterTextChanged(const QString &text);
};

} // namespace xMQTT