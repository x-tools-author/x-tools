/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>
#include <thread>

#include <QByteArray>
#include <QString>

#include <mongoose.h>

#include "mqttcommon.h"
#include "mqttserver.h"

extern "C" {
struct SubscriptionContext
{
    struct SubscriptionContext *next;
    struct mg_connection *c;
    struct mg_str topic;
    uint8_t qos;
};
}

namespace xMQTT {

class MqttServerPrivate
{
public:
    MqttServerPrivate(MqttServer *q) {}
    ~MqttServerPrivate() {}

public:
    MqttServer *q{nullptr};
    QString m_ip{"127.0.0.1"};
    quint16 m_port{1883};
    struct SubscriptionContext *m_subs = nullptr;
    std::atomic_bool m_interrupted{false};
    mg_connection *m_conn{nullptr};

public:
    size_t nextTopic(struct mg_mqtt_message *msg, struct mg_str *topic, uint8_t *qos, size_t pos)
    {
        unsigned char *buf = (unsigned char *) msg->dgram.buf + pos;
        size_t new_pos;
        if (pos >= msg->dgram.len)
            return 0;

        topic->len = (size_t) (((unsigned) buf[0]) << 8 | buf[1]);
        topic->buf = (char *) buf + 2;
        new_pos = pos + 2 + topic->len + (qos == NULL ? 0 : 1);
        if ((size_t) new_pos > msg->dgram.len)
            return 0;
        if (qos != NULL)
            *qos = buf[2 + topic->len];
        return new_pos;
    }

    size_t nextSub(struct mg_mqtt_message *msg, struct mg_str *topic, uint8_t *qos, size_t pos)
    {
        uint8_t tmp;
        return nextTopic(msg, topic, qos == NULL ? &tmp : qos, pos);
    }

    size_t nextUnsub(struct mg_mqtt_message *msg, struct mg_str *topic, size_t pos)
    {
        return nextTopic(msg, topic, NULL, pos);
    }

    static void eventHandler(struct mg_connection *c, int ev, void *ev_data)
    {
        MqttServer *server = static_cast<MqttServer *>(c->fn_data);
        MqttServerPrivate *d = server->d;

        if (ev == MG_EV_MQTT_CMD) {
            struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
            if (!mm) {
                return;
            }

            std::shared_ptr<MqttMessage> msg = std::make_shared<MqttMessage>();
            msg->id = mm->id;
            msg->cmd = mm->cmd;
            msg->qos = mm->qos;
            msg->ack = mm->ack;
            msg->props_start = mm->props_start;
            msg->props_size = mm->props_size;
            msg->topic = QString::fromUtf8(mm->topic.buf, static_cast<int>(mm->topic.len));
            msg->data = QByteArray(mm->data.buf, static_cast<int>(mm->data.len));
            msg->dgram = QByteArray(mm->dgram.buf, static_cast<int>(mm->dgram.len));
            emit server->mqttMessageRx(msg);

            switch (mm->cmd) {
            case MQTT_CMD_CONNECT: {
                // Client connects
                if (mm->dgram.len < 9) {
                    //mg_error(c, "Malformed MQTT frame");
                    emit server->logMessage(QObject::tr("Malformed MQTT frame"), false);
                } else if (mm->dgram.buf[8] != 4) {
                    //mg_error(c, "Unsupported MQTT version %d", mm->dgram.buf[8]);
                    QString msg = QObject::tr("Unsupported MQTT version %1").arg(mm->dgram.buf[8]);
                    emit server->logMessage(msg, false);
                } else {
                    uint8_t response[] = {0, 0};
                    mg_mqtt_send_header(c, MQTT_CMD_CONNACK, 0, sizeof(response));
                    mg_send(c, response, sizeof(response));
                }
                break;
            }
            case MQTT_CMD_SUBSCRIBE: {
                // Client subscribes
                size_t pos = 4; // Initial topic offset, where ID ends
                uint8_t qos, resp[256];
                struct mg_str topic;
                int num_topics = 0;
                while ((pos = d->nextSub(mm, &topic, &qos, pos)) > 0) {
                    SubscriptionContext *sub = (SubscriptionContext *) calloc(1, sizeof(*sub));
                    sub->c = c;
                    sub->topic = mg_strdup(topic);
                    sub->qos = qos;
                    LIST_ADD_HEAD(struct SubscriptionContext, &d->m_subs, sub);
                    QString topicStr = QString::fromUtf8(topic.buf, int(topic.len));
                    QString remIp = mgAddressToIpV4(&c->rem);
                    QString remPort = QString::number(mg_ntohs(c->rem.port));
                    QString msg = QString("Client(%1:%2) subscribed to topic: %3")
                                      .arg(remIp)
                                      .arg(remPort)
                                      .arg(topicStr);
                    emit server->logMessage(msg, false);
                    emit server->clientSubscribed(remIp, mg_ntohs(c->rem.port), topicStr);
                    // MG_INFO(("SUB %p [%.*s]", c->fd, (int) sub->topic.len, sub->topic.buf));
                    // Change '+' to '*' for topic matching using mg_match
                    for (size_t i = 0; i < sub->topic.len; i++) {
                        if (sub->topic.buf[i] == '+') {
                            ((char *) sub->topic.buf)[i] = '*';
                        }
                    }
                    resp[num_topics++] = qos;
                }
                mg_mqtt_send_header(c, MQTT_CMD_SUBACK, 0, num_topics + 2);
                uint16_t id = mg_htons(mm->id);
                mg_send(c, &id, 2);
                mg_send(c, resp, num_topics);
                break;
            }
            case MQTT_CMD_PUBLISH: {
                // Client published message. Push to all subscribed channels
                MG_INFO(("PUB %p [%.*s] -> [%.*s]",
                         c->fd,
                         (int) mm->data.len,
                         mm->data.buf,
                         (int) mm->topic.len,
                         mm->topic.buf));
                QByteArray payload = QByteArray(mm->data.buf, static_cast<int>(mm->data.len));
                QString topicStr = QString::fromUtf8(mm->topic.buf, int(mm->topic.len));
                QString msg = QString("Message received on topic %1: %2")
                                  .arg(topicStr)
                                  .arg(QString::fromUtf8(payload.toHex()));
                emit server->logMessage(msg, false);
                for (struct SubscriptionContext *sub = d->m_subs; sub != NULL; sub = sub->next) {
                    if (mg_match(mm->topic, sub->topic, NULL)) {
                        struct mg_mqtt_opts pub_opts;
                        memset(&pub_opts, 0, sizeof(pub_opts));
                        pub_opts.topic = mm->topic;
                        pub_opts.message = mm->data;
                        pub_opts.qos = 1, pub_opts.retain = false;
                        mg_mqtt_pub(sub->c, &pub_opts);
                    }
                }
                break;
            }
            case MQTT_CMD_PINGREQ: {
                // The server must send a PINGRESP packet in response to a PINGREQ packet [MQTT-3.12.4-1]
                mg_mqtt_send_header(c, MQTT_CMD_PINGRESP, 0, 0);
                break;
            }
            }
        } else if (ev == MG_EV_ACCEPT) {
            // c->is_hexdumping = 1;
            QString remIp = mgAddressToIpV4(&c->rem);
            QString msg = QString("Client connected: %1:%2").arg(remIp).arg(mg_ntohs(c->rem.port));
            emit server->logMessage(msg, false);
            emit server->clientConnected(remIp, mg_ntohs(c->rem.port));
        } else if (ev == MG_EV_CLOSE) {
            // Client disconnects. Remove from the subscription list
            for (struct SubscriptionContext *next, *sub = d->m_subs; sub != NULL; sub = next) {
                next = sub->next;
                if (c != sub->c) {
                    continue;
                }
                MG_INFO(("UNSUB %p [%.*s]", c->fd, (int) sub->topic.len, sub->topic.buf));
                mg_free(sub->topic.buf);
                LIST_DELETE(struct SubscriptionContext, &d->m_subs, sub);
                free(sub);
            }

            if (c != d->m_conn) {
                QString remIp = mgAddressToIpV4(&c->rem);
                emit server->clientDisconnected(remIp, mg_ntohs(c->rem.port));

                QString msg = QString("Client disconnected: %1:%2").arg(remIp).arg(c->rem.port);
                emit server->logMessage(msg, false);
            }
        }
    }
};

} // namespace xMQTT