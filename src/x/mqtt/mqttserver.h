/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include <QThread>

#include "mqttcommon.h"

namespace xMQTT {

struct MqttMessage;
class MqttServerPrivate;
class MqttServer : public QThread
{
    Q_OBJECT
public:
    explicit MqttServer(QObject *parent = nullptr);
    ~MqttServer() override;

    void startServer(const QString &ip, quint16 port);
    void stopServer();

signals:
    void logMessage(const QString &msg, bool isError = false);
    void mqttMessageRx(std::shared_ptr<MqttMessage> message);
    void clientSubscribed(const QString &ip, quint16 port, const QString &topic);
    void clientUnsubscribed(const QString &ip, quint16 port, const QString &topic);
    void serverStarted();
    void clientConnected(const QString &ip, quint16 port);
    void clientDisconnected(const QString &ip, quint16 port);

protected:
    void run() override;

private:
    friend class MqttServerPrivate;
    MqttServerPrivate *d{nullptr};
};

} // namespace xMQTT