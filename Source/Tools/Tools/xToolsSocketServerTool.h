/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsCommunicationTool.h"

class xToolsSocketServerTool : public xToolsCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(bool specifyIpAndPort READ specifyIpAndPort WRITE setSpecifyIpAndPort NOTIFY
                   specifyIpAndPortChanged)
    Q_PROPERTY(QStringList clients READ clients NOTIFY clientsChanged)
    Q_PROPERTY(int clientIndex READ clientIndex WRITE setClientIndex NOTIFY clientIndexChanged)
    Q_PROPERTY(QString bindingIpPort READ bindingIpPort NOTIFY bindingIpPortChanged)
    // Just for web socket server.
    Q_PROPERTY(int messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)

public:
    explicit xToolsSocketServerTool(QObject *parent = nullptr);

    QString serverIp();
    void setServerIp(const QString &ip);
    int serverPort();
    void setServerPort(int port);
    bool specifyIpAndPort();
    void setSpecifyIpAndPort(bool specified);
    QStringList clients();
    int clientIndex();
    void setClientIndex(int index);
    QString bindingIpPort();
    int messageType();
    void setMessageType(int type);

protected:
    QString m_serverIp;
    int m_serverPort;
    bool m_specifyIpAndPort{true};
    QStringList m_clients;
    int m_clientIndex;
    QString m_bindingIpPort;
    qint8 m_messageType;

signals:
    void serverIpChanged();
    void serverPortChanged();
    void specifyIpAndPortChanged();
    void clientsChanged();
    void clientIndexChanged();
    void bindingIpPortChanged();
    void messageTypeChanged();
};

