/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTDEVICE_HH
#define SAKUDPCLIENTDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QUdpSocket>

#include "SAKDebuggerDevice.hh"
#include "SAKCommonDataStructure.hh"

class SAKUdpClientDebugger;
class SAKUdpClientController;
class SAKUdpClientDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKUdpClientDevice(QSettings *settings,
                       const QString &settingsGroup,
                       QWidget *uiParent = Q_NULLPTR,
                       QObject *parent = Q_NULLPTR);
    ~SAKUdpClientDevice();

    struct UdpSocketParameters{
        bool enableUnicast;
        bool enableMulticast;
        bool enableBroadcast;
        quint16 broadcastPort;

        struct MulticastInfo{
            QString address;
            quint16 port;
        };
        QList<MulticastInfo> multicastInfoList;
    };

    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;

    void setParametersCtx(
            SAKCommonDataStructure::SAKStructUdpClientParametersContext ctx
            );

    void setUnicastEnable(bool enable);
    void setBroadcastEnable(bool enable);
    void setBroadcastPort(quint16 port);
    bool joinMulticastGroup(QString address, quint16 port, QString &errorString);
    void removeMulticastInfo(QString address, quint16 port);
    void setMulticastEnable(bool enable);
private:
    QMutex mParametersContextMutex;
    UdpSocketParameters mParametersContext;
    SAKUdpClientDebugger *mDebugPage;
    QUdpSocket *mUdpSocket;
    SAKUdpClientController *mDeviceController;
private:
    const UdpSocketParameters udpSocketParameters();
signals:
    void clientInfoChanged(QString info);
};
Q_DECLARE_METATYPE(SAKUdpClientDevice::UdpSocketParameters::MulticastInfo)
#endif
