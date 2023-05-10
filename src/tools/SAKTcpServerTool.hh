/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDTCPSERVERTOOL_HPP
#define EDTCPSERVERTOOL_HPP

#include <QTcpServer>
#include "EDBaseTool.hpp"

class EDTcpServerTool : public EDBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QStringList clients READ clients NOTIFY clientsChanged)
    Q_PROPERTY(int clientIndex READ clientIndex WRITE setClientIndex NOTIFY clientIndexChanged)
public:
    explicit EDTcpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;
    virtual void outputBytesHandler() final;
    virtual void uninitialize() final;

private:
    QTcpServer *mTcpServer{nullptr};
    QList<QTcpSocket*> mTcpSocketList;

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

#endif // EDTCPSERVERTOOL_HPP
