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

bool MqttClient::isOpened() const
{
    return d->m_opened;
}

void MqttClient::publish(const QString &topic, const QByteArray &message)
{
    if (d->m_conn) {
        d->publish(d->m_conn, topic, message);
    }
}

void MqttClient::subscribe(const QString &topic)
{
    if (d->m_conn) {
        d->subscribe(d->m_conn, topic);
    }
}

void MqttClient::unsubscribe(const QString &topic)
{
    if (d->m_conn) {
        d->unsubscribe(d->m_conn, topic);
    }
}

void MqttClient::run()
{
    d->m_opened = false;
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mgr.userdata = this;
    d->m_conn = mg_mqtt_connect(&mgr,
                                d->url().toUtf8().constData(),
                                nullptr,
                                &MqttClientPrivate::eventHandler,
                                this);
    if (d->m_conn) {
        d->m_conn->fn_data = this;
        mg_timer_add(&mgr,
                     3000,
                     MG_TIMER_REPEAT | MG_TIMER_RUN_NOW,
                     &MqttClientPrivate::timerCallback,
                     &mgr);
    } else {
        emit logMessage(QString("Failed to connect to %1").arg(d->url()), true);
        return;
    }

    while (!isInterruptionRequested()) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
}

} // namespace xMQTT