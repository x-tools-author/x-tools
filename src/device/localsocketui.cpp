/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "localsocketui.h"
#include "ui_localsocketui.h"

#include "localsocket.h"

LocalSocketUi::LocalSocketUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::LocalSocketUi)
{
    ui->setupUi(this);
}

LocalSocketUi::~LocalSocketUi() {}

QVariantMap LocalSocketUi::save() const
{
    QVariantMap map = DeviceUi::save();
    LocalSocketParametersKeys keys;
    map[keys.serverName] = ui->lineEditServerName->text();
    return map;
}

void LocalSocketUi::load(const QVariantMap &parameters)
{
    DeviceUi::load(parameters);
    LocalSocketParametersKeys keys;
    QString defaultServerName = QString("xtools.local.socket");
    QString serverName = parameters.value(keys.serverName, defaultServerName).toString();
    if (serverName.isEmpty()) {
        serverName = defaultServerName;
    }
    ui->lineEditServerName->setText(serverName);
}

Device *LocalSocketUi::newDevice()
{
    return new LocalSocket(this);
}