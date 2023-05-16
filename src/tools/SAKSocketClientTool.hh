/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTOOL_HH
#define SAKSOCKETCLIENTTOOL_HH

#include "SAKCommunicationTool.hh"

class SAKSocketClientTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString clientIp READ clientIp WRITE setClientIp NOTIFY clientIpChanged)
    Q_PROPERTY(int clientPort READ clientPort WRITE setClientPort NOTIFY clientPortChanged)
    Q_PROPERTY(bool specifyClientIpPort READ specifyClientIpPort WRITE setSpecifyClientIpPort NOTIFY specifyClientIpPortChanged)
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString bindingIpPort READ bindingIpPort NOTIFY bindingIpPortChanged)
    // Just for web socket client.
    Q_PROPERTY(int messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)
public:
    explicit SAKSocketClientTool(const char *lg, QObject *parent = nullptr);

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
    QString mClientIp;
    int mClientPort;
    bool mSpecifyClientIpPort;
    QString mServerIp;
    int mServerPort;
    QString mBindingIpPort;
    std::atomic_int8_t mMessageType;

signals:
    void clientIpChanged();
    void clientPortChanged();
    void specifyClientIpPortChanged();
    void serverIpChanged();
    void serverPortChanged();
    void bindingIpPortChanged();
    void messageTypeChanged();
};

#endif // SAKSOCKETCLIENTTOOL_HH
