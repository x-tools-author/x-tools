/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

#include "utilities/serializable.h"

namespace Ui {
class xCanBus;
}

namespace xCanBus {

class CanBusDevice;
class xCanBus : public QWidget, public xTools::Serializable
{
    Q_OBJECT
public:
    explicit xCanBus(QWidget* parent = nullptr);
    ~xCanBus();

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;

protected:
    bool event(QEvent* event) override;

private:
    Ui::xCanBus* ui;
    int m_leftWidth{168};
    CanBusDevice* m_device{nullptr};

private:
    void onDisconnect();
    void onConnect();

    void onPluginChanged(const QString& pluginName);
};

} // namespace xCanBus