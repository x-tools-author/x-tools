/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttserver.h"
#include "mqttserver_p.h"

#include <mongoose.h>

#include "mqttserver_p.h"

namespace xMQTT {

MqttServer::MqttServer(QObject *parent)
    : QThread(parent)
{
    d = new MqttServerPrivate(this);
}

MqttServer::~MqttServer()
{
    stopServer();
    delete d;
}

void MqttServer::startServer(const QString &ip, quint16 port)
{
    d->m_ip = ip;
    d->m_port = port;
    stopServer();
    start();
}

void MqttServer::stopServer()
{
    requestInterruption();
    exit();
    wait();
}

void MqttServer::run()
{
    const QString tmp = QString("mqtt://%1:%2").arg(d->m_ip).arg(d->m_port);
    const QByteArray url = tmp.toUtf8();
    struct mg_mgr mgr;
    mgr.userdata = this;
    mg_mgr_init(&mgr);
    emit logMessage(QStringLiteral("Server starting on %1").arg(QString::fromUtf8(url)));

    d->m_conn = mg_mqtt_listen(&mgr, url.constData(), &MqttServerPrivate::eventHandler, this);
    if (d->m_conn == nullptr) {
        QString msg = QString("Server can not listen on %1").arg(QString::fromUtf8(url));
        emit logMessage(msg, true);
        return;
    }

    while (!isInterruptionRequested()) {
        mg_mgr_poll(&mgr, 1000); // Event loop, 1s timeout
    }
    mg_mgr_free(&mgr);
    emit logMessage(QStringLiteral("Server stopped"));
}

} // namespace xMQTT