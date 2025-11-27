/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xmqtt.h"
#include "ui_xmqtt.h"

#include <mongoose.h>

namespace xMqtt {

struct xMqttParameterKeys
{
    const QString showMode{"showMode"};
    const QString showClient{"showClient"};
    const QString showServer{"showServer"};

    const QString client{"client"};
    const QString server{"server"};
};

enum class ShowMode {
    Client = 0,
    Server = 1,
    Both = 2,
};

xMqtt::xMqtt(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xMqtt)
{
    ui->setupUi(this);
    ui->toolButtonClient->setCheckable(true);
    ui->toolButtonServer->setCheckable(true);
    ui->toolButtonClient->setChecked(true);
    ui->toolButtonServer->setChecked(true);
    connect(ui->toolButtonClient, &QToolButton::clicked, this, &xMqtt::onClientButtonClicked);
    connect(ui->toolButtonServer, &QToolButton::clicked, this, &xMqtt::onServerButtonClicked);

#if 1
    static bool disabledMongooseLog = false;
    if (!disabledMongooseLog) {
        mg_log_set(MG_LL_NONE); // Disable mongoose log except error
        disabledMongooseLog = true;
    }
#endif
}

xMqtt::~xMqtt()
{
    delete ui;
}

QJsonObject xMqtt::save()
{
    QJsonObject obj;
    int mode = static_cast<int>(ShowMode::Both);
    if (ui->widgetClient->isVisible() && !ui->widgetServer->isVisible()) {
        mode = static_cast<int>(ShowMode::Client);
    } else if (!ui->widgetClient->isVisible() && ui->widgetServer->isVisible()) {
        mode = static_cast<int>(ShowMode::Server);
    } else {
        mode = static_cast<int>(ShowMode::Both);
    }
    obj[xMqttParameterKeys().showMode] = mode;
    obj[xMqttParameterKeys().showClient] = ui->toolButtonClient->isChecked();
    obj[xMqttParameterKeys().showServer] = ui->toolButtonServer->isChecked();
    obj[xMqttParameterKeys().client] = ui->widgetClient->save();
    obj[xMqttParameterKeys().server] = ui->widgetServer->save();
    return obj;
}

void xMqtt::load(const QJsonObject& obj)
{
    int mode = obj.value(xMqttParameterKeys().showMode).toInt(static_cast<int>(ShowMode::Both));
    switch (static_cast<ShowMode>(mode)) {
    case ShowMode::Client:
        ui->widgetClient->setVisible(true);
        ui->widgetServer->setVisible(false);
        break;
    case ShowMode::Server:
        ui->widgetClient->setVisible(false);
        ui->widgetServer->setVisible(true);
        break;
    case ShowMode::Both:
    default:
        ui->widgetClient->setVisible(true);
        ui->widgetServer->setVisible(true);
        break;
    }
    ui->widgetClient->load(obj.value(xMqttParameterKeys().client).toObject());
    ui->widgetServer->load(obj.value(xMqttParameterKeys().server).toObject());
    ui->toolButtonClient->setChecked(obj.value(xMqttParameterKeys().showClient).toBool(true));
    ui->toolButtonServer->setChecked(obj.value(xMqttParameterKeys().showServer).toBool(true));
    onClientButtonClicked(ui->toolButtonClient->isChecked());
    onServerButtonClicked(ui->toolButtonServer->isChecked());
}

void xMqtt::onClientButtonClicked(bool checked)
{
    if (!checked && !ui->toolButtonServer->isChecked()) {
        ui->toolButtonClient->setChecked(true);
        return;
    }

    ui->widgetClient->setVisible(checked);
}

void xMqtt::onServerButtonClicked(bool checked)
{
    if (!checked && !ui->toolButtonClient->isChecked()) {
        ui->toolButtonServer->setChecked(true);
        return;
    }

    ui->widgetServer->setVisible(checked);
}

} // namespace xMqtt