/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDUDPSERVERTOOL_HPP
#define EDUDPSERVERTOOL_HPP

#include <QUdpSocket>
#include "EDBaseTool.hpp"

class EDUdpServerTool : public EDBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QStringList clients READ clients NOTIFY clientsChanged)
    Q_PROPERTY(int clientIndex READ clientIndex WRITE setClientIndex NOTIFY clientIndexChanged)
public:
    explicit EDUdpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;
    virtual void outputBytesHandler() final;
    virtual void uninitialize() final;

private:
    QUdpSocket *mUdpSocket{nullptr};

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
};

#endif // EDUDPSERVERTOOL_HPP
