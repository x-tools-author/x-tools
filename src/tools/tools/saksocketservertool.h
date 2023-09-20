/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKSOCKETSERVERTOOL_H
#define SAKSOCKETSERVERTOOL_H

#include "sakcommunicationtool.h"

class SAKSocketServerTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(bool specifyIpAndPort READ specifyIpAndPort WRITE setSpecifyIpAndPort NOTIFY specifyIpAndPortChanged)
    Q_PROPERTY(QStringList clients READ clients NOTIFY clientsChanged)
    Q_PROPERTY(int clientIndex READ clientIndex WRITE setClientIndex NOTIFY clientIndexChanged)
    Q_PROPERTY(QString bindingIpPort READ bindingIpPort NOTIFY bindingIpPortChanged)
    // Just for web socket server.
    Q_PROPERTY(int messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)

public:
    explicit SAKSocketServerTool(const char *log, QObject *parent = nullptr);

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
    QString mServerIp;
    int mServerPort;
    bool mSpecifyIpAndPort;
    QStringList mClients;
    int mClientIndex;
    QString mBindingIpPort;
    std::atomic_int8_t mMessageType;

signals:
    void serverIpChanged();
    void serverPortChanged();
    void specifyIpAndPortChanged();
    void clientsChanged();
    void clientIndexChanged();
    void bindingIpPortChanged();
    void messageTypeChanged();
};

#endif // SAKSOCKETSERVERTOOL_H
