/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPSERVERDEVICE_HH
#define SAKUDPSERVERDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QUdpSocket>
#include <QJsonObject>

#include "SAKDebuggerDevice.hh"

class SAKUdpServerDebugger;
class SAKUdpServerController;
class SAKUdpServerDevice:public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKUdpServerDevice(QSettings *settings,
                       const QString &settingsGroup,
                       QWidget *uiParent = Q_NULLPTR,
                       QObject *parent = Q_NULLPTR);
    void setMulticastParameters(const QJsonArray &parameters);
private:
    bool initialize() final;
    ReadContextVector read() final;
    WriteContext write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QUdpSocket *mUdpServer;
    QStringList mMulticastHostList;
    QMutex mMulticastHostListMutex;
private:
    QStringList multicastParameters();
signals:
    void addClient(QString host, quint16 port);
};

#endif
