/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QObject>

#include <mongoose.h>

#include "mqttclient.h"
#include "mqttcommon.h"

#if 0
size_t get_props_size(struct mg_mqtt_prop *props, size_t count);
bool mg_send_u16(struct mg_connection *c, uint16_t value);
void mg_send_mqtt_properties(struct mg_connection *c, struct mg_mqtt_prop *props, size_t nprops);
#endif

namespace xMQTT {

class MqttClient;
class MqttClientPrivate
{
public:
    MqttClientPrivate(MqttClient *qPtr)
        : q(qPtr)
    {}
    ~MqttClientPrivate() {}

public:
    MqttClient *q{nullptr};
    int m_qos{static_cast<int>(QoS::ExactlyOnce)};
    int m_version{4};
    QString m_ip;
    quint16 m_port{1883};
    mg_connection *m_conn{nullptr};
    bool m_opened{false};

public:
    QString url() const { return QString("mqtt://%1:%2").arg(m_ip).arg(m_port); }

    static void subscribe(struct mg_connection *c, const QString &topic)
    {
        MqttClient *client = static_cast<MqttClient *>(c->fn_data);
        MqttClientPrivate *d = client->d;

        struct mg_mqtt_opts opts = {};
        memset(&opts, 0, sizeof(opts));
        opts.topic = mg_str(topic.toUtf8().constData());
        opts.qos = d->m_qos;
        mg_mqtt_sub(c, &opts);
        QString msg = QString("Subscribed to %1").arg(QString(topic));
        emit client->logMessage(msg, false);
    }

    static void mg_mqtt_unsub(struct mg_connection *c, const struct mg_mqtt_opts *opts)
    {
#if 0
        // https://github.com/cesanta/mongoose/issues/2695
        uint8_t qos_ = opts->qos & 3;
        size_t plen = c->is_mqtt5 ? get_props_size(opts->props, opts->num_props) : 0;
        size_t len = 2 + opts->topic.len + 2 + 1 + plen;

        mg_mqtt_send_header(c, MQTT_CMD_UNSUBSCRIBE, 2, (uint32_t) len);
        if (++c->mgr->mqtt_id == 0) {
            ++c->mgr->mqtt_id;
        }

        mg_send_u16(c, mg_htons(c->mgr->mqtt_id));
        if (c->is_mqtt5) {
            mg_send_mqtt_properties(c, opts->props, opts->num_props);
        }

        mg_send_u16(c, mg_htons((uint16_t) opts->topic.len));
        mg_send(c, opts->topic.buf, opts->topic.len);
        mg_send(c, &qos_, sizeof(qos_));
#endif
    }

    static void unsubscribe(struct mg_connection *c, const QString &topic)
    {
        MqttClient *client = static_cast<MqttClient *>(c->fn_data);
        MqttClientPrivate *d = client->d;

        struct mg_mqtt_opts opts = {};
        memset(&opts, 0, sizeof(opts));
        opts.topic = mg_str(topic.toUtf8().constData());
        mg_mqtt_unsub(c, &opts);
        QString msg = QString("Unsubscribed from %1").arg(QString(topic));
        emit client->logMessage(msg, false);
    }

    static void publish(struct mg_connection *c, const QString &topic, const QByteArray &message)
    {
        MqttClient *client = static_cast<MqttClient *>(c->fn_data);
        MqttClientPrivate *d = client->d;

        struct mg_mqtt_opts opts = {};
        memset(&opts, 0, sizeof(opts));
        opts.topic = mg_str(topic.toUtf8().constData());
        opts.message = mg_str(message.constData());
        opts.qos = d->m_qos;
        mg_mqtt_pub(c, &opts);
        QString msg = QString("Published to %1:%2").arg(QString(topic)).arg(QString(message));
        emit client->logMessage(msg, false);
    }

    static void eventHandler(struct mg_connection *c, int ev, void *ev_data)
    {
        MqttClient *client = static_cast<MqttClient *>(c->fn_data);
        MqttClientPrivate *d = client->d;

        if (ev == MG_EV_OPEN) {
            // MG_INFO(("%lu CREATED", c->id));
            // c->is_hexdumping = 1;
        } else if (ev == MG_EV_CONNECT) {
            if (c->is_tls) {
                struct mg_tls_opts opts;
                opts.ca = mg_unpacked("/certs/ca.pem");
                opts.name = mg_url_host(d->url().toUtf8().constData());
                mg_tls_init(c, &opts);
            }
        } else if (ev == MG_EV_ERROR) {
            // On error, log error message
            // MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
            QString errMsg = QString("Connection error: %1").arg(QString((char *) ev_data));
            emit client->logMessage(errMsg, false);
        } else if (ev == MG_EV_MQTT_OPEN) {
            // MQTT connect is successful
            //MG_INFO(("%lu CONNECTED to %s", c->id, d->url().toUtf8().constData()));
            //subscribe(c, s_sub_topic);
            d->m_opened = true;
            emit client->logMessage(QString("Connected to %1").arg(d->url()), false);
        } else if (ev == MG_EV_MQTT_MSG) {
            // When we get echo response, print it
            char response[100];
            struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
            mg_snprintf(response,
                        sizeof(response),
                        "Got %.*s -> %.*s",
                        mm->topic.len,
                        mm->topic.buf,
                        mm->data.len,
                        mm->data.buf);
            //publish(c, s_pub_topic, response);
        } else if (ev == MG_EV_MQTT_CMD) {
            struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
            if (mm->cmd == MQTT_CMD_PINGREQ) {
                mg_mqtt_pong(c);
            } else if (mm->cmd == MQTT_CMD_SUBACK) {
                QString msg = QString("Subscribed to %1")
                                  .arg(mm->ack == 0 ? "successfully" : "with failure");
                emit client->logMessage(msg, false);
                qInfo() << "Subscription acknowledged";
            }
        } else if (ev == MG_EV_CLOSE) {
            // MG_INFO(("%lu CLOSED", c->id));
            //s_mqtt_conn = NULL; // Mark that we're closed
            d->m_conn = nullptr;
            d->m_opened = false;
            emit client->logMessage(QString("Disconnected from %1").arg(d->url()), false);
        }
    }

    static void timerCallback(void *arg)
    {
        struct mg_mgr *mgr = (struct mg_mgr *) arg;
        MqttClient *client = static_cast<MqttClient *>(mgr->userdata);
        MqttClientPrivate *d = client->d;

        struct mg_mqtt_opts opts;
        opts.clean = true;
        opts.qos = d->m_qos;
        opts.topic = mg_str("hello/from/mongoose");
        opts.keepalive = 5;
        opts.version = d->m_version;
        opts.message = mg_str("bye");
        if (d->m_conn == nullptr) {
            d->m_conn = mg_mqtt_connect(mgr,
                                        d->url().toUtf8().constData(),
                                        &opts,
                                        &MqttClientPrivate::eventHandler,
                                        client);
            if (d->m_conn) {
                d->m_conn->fn_data = client;
            }
        } else {
            mg_mqtt_ping(d->m_conn);
        }
    }
};

} // namespace xMQTT
