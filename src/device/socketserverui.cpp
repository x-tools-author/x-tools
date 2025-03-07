/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "socketserverui.h"

#include "device/socketserver.h"

SocketServerUi::SocketServerUi(QWidget *parent)
    : SocketUi(parent)
{}

SocketServerUi::~SocketServerUi() {}

void SocketServerUi::setupServer(SocketServer *server)
{
    connect(server, &SocketServer::clientsChanged, this, [=]() { setupClients(server->clients()); });
    connect(this, &SocketServerUi::invokeDisconnectAll, server, &SocketServer::disconnectAllClients);
    connect(this, &SocketServerUi::currentClientChanged, server, [=](const QString &flag) {
        server->setCurrentClientFlag(flag);
    });
}

void SocketServerUi::setUiEnabled(bool enabled)
{
    setServerWidgetsEnabled(enabled);
}
