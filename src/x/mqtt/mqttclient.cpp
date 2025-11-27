/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttclient.h"

#include <mongoose.h>

#include "mqttclient_p.h"

namespace xMQTT {

MqttClient::MqttClient(QObject *parent)
    : QThread(parent)
{
    d = new MqttClientPrivate(this);
}

MqttClient::~MqttClient()
{
    stopClient();
    delete d;
}

void MqttClient::startClient(const QString &host, quint16 port, int qos, int version)
{
    d->m_ip = host;
    d->m_port = port;
    d->m_qos = qos;
    d->m_version = version;
    stopClient();
    start();
}

void MqttClient::stopClient()
{
    requestInterruption();
    quit();
    wait();
}

void MqttClient::run()
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mgr.userdata = this;
    mg_timer_add(&mgr,
                 3000,
                 MG_TIMER_REPEAT | MG_TIMER_RUN_NOW,
                 &MqttClientPrivate::timerCallback,
                 &mgr);

    while (!isInterruptionRequested()) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

void MqttClient::outputLogMessage(const QString &msg, bool isError)
{
    emit logMessage(msg, isError);
}

} // namespace xMQTT