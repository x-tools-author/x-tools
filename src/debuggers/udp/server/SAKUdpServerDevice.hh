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

#include <QThread>
#include <QUdpSocket>

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
private:
    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;
private:
    QUdpSocket *mUdpServer;
signals:
    void addClient(QString host, quint16 port);
};

#endif
