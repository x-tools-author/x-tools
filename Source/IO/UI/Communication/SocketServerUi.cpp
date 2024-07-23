/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketServerUi.h"

#include "../../IO/Communication/SocketServer.h"

SocketServerUi::SocketServerUi(xIO::CommunicationType type, QWidget *parent)
    : SocketUi(type, parent)
{}

SocketServerUi::~SocketServerUi() {}

void SocketServerUi::setupDevice(Communication *device)
{
    SocketServer *server = qobject_cast<SocketServer *>(device);
    if (!server) {
        return;
    }

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
