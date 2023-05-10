/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDWEBSOCKETSERVERTOOL_HPP
#define EDWEBSOCKETSERVERTOOL_HPP

#include <QWebSocketServer>
#include "EDBaseTool.hpp"

class EDWebSocketServerTool : public EDBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QStringList clients READ clients NOTIFY clientsChanged)
    Q_PROPERTY(int clientIndex READ clientIndex WRITE setClientIndex NOTIFY clientIndexChanged)
    Q_PROPERTY(int messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)

public:
    explicit EDWebSocketServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;
    virtual void outputBytesHandler() final;
    virtual void uninitialize() final;

private:
    QWebSocketServer *mWebSocketServer{nullptr};
    QList<QWebSocket*> mWebSocketList;

private:
    QString mServerIp;
    QString serverIp(){return mServerIp;}
    void setServerIp(const QString &ip){mServerIp = ip; emit serverIpChanged();}
    Q_SIGNAL void serverIpChanged();

    int mServerPort;
    int serverPort(){return mServerPort;}
    void setServerPort(int port){mServerPort = port; emit serverPortChanged();}
    Q_SIGNAL void serverPortChanged();

    QStringList mClients;
    QStringList clients(){return mClients;}
    Q_SIGNAL void clientsChanged();

    int mClientIndex;
    int clientIndex(){return mClientIndex;}
    void setClientIndex(int index){mClientIndex = index; emit clientIndexChanged();}
    Q_SIGNAL void clientIndexChanged();

    std::atomic_int8_t mMessageType;
    int messageType(){return mMessageType;}
    void setMessageType(int type){mMessageType = type; emit messageTypeChanged();}
    Q_SIGNAL void messageTypeChanged();
};

#endif // EDWEBSOCKETSERVERTOOL_HPP
