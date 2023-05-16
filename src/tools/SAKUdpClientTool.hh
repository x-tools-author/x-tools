/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPCLIENTTOOL_H
#define SAKUDPCLIENTTOOL_H

#include <QUdpSocket>
#include "SAKCommunicationTool.hh"

class SAKUdpClientTool : public SAKCommunicationTool
{
    Q_OBJECT
    Q_PROPERTY(QString clientIp READ clientIp WRITE setClientIp NOTIFY clientIpChanged)
    Q_PROPERTY(int clientPort READ clientPort WRITE setClientPort NOTIFY clientPortChanged)
    Q_PROPERTY(bool specifyClientIpPort READ specifyClientIpPort WRITE setSpecifyClientIpPort NOTIFY specifyClientIpPortChanged)
    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(QString bindingIpPort READ bindingIpPort NOTIFY bindingIpPortChanged)

public:
    explicit SAKUdpClientTool(QObject *parent = nullptr);

protected:
    virtual bool initialize() final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void uninitialize() final;
    void readBytes();

private:
    QUdpSocket *mUdpSocket{nullptr};

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
};

#endif // SAKUDPCLIENTTOOL_H
