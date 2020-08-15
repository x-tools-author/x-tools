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
    SAKUdpServerDevice(SAKUdpServerDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
private:
    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    bool checkSomething(QString &errorString) final;
    void close() final;
    void free() final;
private:
    SAKUdpServerDebugPage *mDebugPage;
    QUdpSocket *mUdpServer;
    SAKUdpServerDeviceController *mDeviceController;
signals:
    void addClient(QString host, quint16 port);
};

#endif
