/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKDebugPageDevice.hh"

class SAKUdpServerDebugPage;
class SAKUdpServerDeviceController;
class SAKUdpServerDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKUdpServerDevice(SAKUdpServerDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run() final;
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKUdpServerDebugPage *debugPage;
    QUdpSocket *udpServer;
};

#endif
