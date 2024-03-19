/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSOCKETCLIENTTOOL_H
#define SAKSOCKETCLIENTTOOL_H

#include "sakcommunicationtool.h"

class SAKSocketClientTool : public SAKCommunicationTool
{
    Q_OBJECT
    // clang-format off
    Q_PROPERTY(QString clientIp READ clientIp WRITE setClientIp NOTIFY clientIpChanged)
    Q_PROPERTY(int clientPort READ clientPort WRITE setClientPort NOTIFY clientPortChanged)
    Q_PROPERTY(bool specifyClientIpPort READ specifyClientIpPort WRITE setSpecifyClientIpPort NOTIFY specifyClientIpPortChanged)
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString bindingIpPort READ bindingIpPort NOTIFY bindingIpPortChanged)
    // Just for web socket client.
    Q_PROPERTY(int messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)
    // clang-format on
public:
    explicit SAKSocketClientTool(QObject *parent = nullptr);

public:
    QString clientIp();
    void setClientIp(const QString &ip);
    int clientPort();
    void setClientPort(int port);
    bool specifyClientIpPort();
    void setSpecifyClientIpPort(bool specified);
    QString serverIp();
    void setServerIp(const QString &ip);
    int serverPort();
    void setServerPort(int port);
    QString bindingIpPort();
    int messageType();
    void setMessageType(int type);

protected:
    QString m_clientIp;
    int m_clientPort;
    bool m_specifyClientIpPort;
    QString m_serverIp;
    int m_serverPort;
    QString m_bindingIpPort;
    std::atomic_int8_t m_messageType;

signals:
    void clientIpChanged();
    void clientPortChanged();
    void specifyClientIpPortChanged();
    void serverIpChanged();
    void serverPortChanged();
    void bindingIpPortChanged();
    void messageTypeChanged();
};

#endif // SAKSOCKETCLIENTTOOL_H
