/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QThread>

namespace xMQTT {

class MqttClientPrivate;
class MqttClient : public QThread
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MqttClient)
public:
    explicit MqttClient(QObject *parent = nullptr);
    ~MqttClient() override;

    void startClient(const QString &host, quint16 port, int qos, int version);
    void stopClient();
    bool isOpened() const;
    void publish(const QString &topic, const QByteArray &message);
    void subscribe(const QString &topic);

signals:
    void logMessage(const QString &msg, bool isError = false);

protected:
    void run() override;

private:
    MqttClientPrivate *d{nullptr};
    friend class MqttClientPrivate;
    void outputLogMessage(const QString &msg, bool isError = false);
};

} // namespace xMQTT