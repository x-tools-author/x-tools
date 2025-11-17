/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "localserverui.h"
#include "ui_localserverui.h"

#include "common/xtools.h"
#include "localserver.h"
#include "utilities/compatibility.h"

LocalServerUi::LocalServerUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::LocalServerUi)
{
    ui->setupUi(this);
    ui->comboBoxClients->addItem(tr("All"), 0);
    connect(ui->comboBoxClients, xComboBoxActivated, this, &LocalServerUi::onTargetChanged);
}

LocalServerUi::~LocalServerUi() {}

QVariantMap LocalServerUi::save() const
{
    QVariantMap map = DeviceUi::save();
    LocalServerParametersKeys keys;
    map[keys.serverName] = ui->lineEditServerName->text();
    return map;
}

void LocalServerUi::load(const QVariantMap &parameters)
{
    DeviceUi::load(parameters);
    LocalServerParametersKeys keys;
    QString defaultServerName = QString("xtools.local.socket");
    QString serverName = parameters.value(keys.serverName, defaultServerName).toString();
    if (serverName.isEmpty()) {
        serverName = defaultServerName;
    }
    ui->lineEditServerName->setText(serverName);
}

void LocalServerUi::setUiEnabled(bool enabled)
{
    ui->lineEditServerName->setEnabled(enabled);
}

Device *LocalServerUi::newDevice()
{
    m_server = new LocalServer(this);
    connect(m_server, &LocalServer::socketConnected, this, &LocalServerUi::onSocketConnected);
    connect(m_server, &LocalServer::socketDisconnected, this, &LocalServerUi::onSocketDisconnected);
    return m_server;
}

void LocalServerUi::onSocketConnected(QLocalSocket *socket, const QString &socketName)
{
    ui->comboBoxClients->addItem(socketName, QVariant::fromValue(socket));
}

void LocalServerUi::onSocketDisconnected(QLocalSocket *socket)
{
    int index = ui->comboBoxClients->findData(QVariant::fromValue(socket));
    if (index != -1) {
        ui->comboBoxClients->removeItem(index);
    }
}

void LocalServerUi::onTargetChanged()
{
    QLocalSocket *socket = ui->comboBoxClients->currentData().value<QLocalSocket *>();
    m_server->setTarget(socket);
}