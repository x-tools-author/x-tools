/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPSERVERTOOL_H
#define SAKUDPSERVERTOOL_H

#include <QUdpSocket>
#include "SAKCommunicationTool.hh"

class SAKUdpServerTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QStringList clients READ clients NOTIFY clientsChanged)
    Q_PROPERTY(int clientIndex READ clientIndex WRITE setClientIndex NOTIFY clientIndexChanged)
public:
    explicit SAKUdpServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize() final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void readBytes() final;
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

#endif // SAKUDPSERVERTOOL_H
