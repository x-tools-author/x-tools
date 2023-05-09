/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDWEBSOCKETCLIENTTOOL_HPP
#define EDWEBSOCKETCLIENTTOOL_HPP

#include <QWebSocket>
#include "EDBaseTool.hpp"


class EDWebSocketClientTool : public EDBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString clientIp READ clientIp WRITE setClientIp NOTIFY clientIpChanged)
    Q_PROPERTY(int clientPort READ clientPort WRITE setClientPort NOTIFY clientPortChanged)
    Q_PROPERTY(bool specifyClientIpPort READ specifyClientIpPort WRITE setSpecifyClientIpPort NOTIFY specifyClientIpPortChanged)
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString bindingIpPort READ bindingIpPort NOTIFY bindingIpPortChanged)
    Q_PROPERTY(int messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)

public:
    explicit EDWebSocketClientTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;
    virtual void outputBytesHandler() final;
    virtual void uninitialize() final;

private:
    QWebSocket *mWebSocket{nullptr};

private:
    QString mClientIp;
    QString clientIp(){return mClientIp;}
    void setClientIp(const QString &ip){mClientIp = ip; emit clientIpChanged();}
    Q_SIGNAL void clientIpChanged();

    int mClientPort;
    int clientPort(){return mClientPort;}
    void setClientPort(int port){mClientPort = port; emit clientPortChanged();}
    Q_SIGNAL void clientPortChanged();

    bool mSpecifyClientIpPort;
    bool specifyClientIpPort(){return mSpecifyClientIpPort;}
    void setSpecifyClientIpPort(bool specified){mSpecifyClientIpPort = specified;}
    Q_SIGNAL void specifyClientIpPortChanged();

    QString mServerIp;
    QString serverIp(){return mServerIp;}
    void setServerIp(const QString &ip){mServerIp = ip; emit serverIpChanged();}
    Q_SIGNAL void serverIpChanged();

    int mServerPort;
    int serverPort(){return mServerPort;}
    void setServerPort(int port){mServerPort = port; emit serverPortChanged();}
    Q_SIGNAL void serverPortChanged();

    QString mBindingIpPort;
    QString bindingIpPort(){return mBindingIpPort;}
    Q_SIGNAL void bindingIpPortChanged();

    std::atomic_int8_t mMessageType;
    int messageType(){return mMessageType;}
    void setMessageType(int type){mMessageType = type; emit messageTypeChanged();}
    Q_SIGNAL void messageTypeChanged();
};

#endif // EDWEBSOCKETCLIENTTOOL_HPP
