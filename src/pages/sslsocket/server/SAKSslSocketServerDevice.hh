/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSSLSOCKETSERVERDEVICE_HH
#define SAKSSLSOCKETSERVERDEVICE_HH

#include <QThread>
#include <QTcpServer>
#include <QSslSocket>

#include "SAKDebugPageDevice.hh"

class SAKSslSocketTcpServer;
class SAKSslSocketServerDebugPage;
class SAKSslSocketServerDeviceController;
class SAKSslSocketServerDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKSslSocketServerDevice(SAKSslSocketServerDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
private:
    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    bool checkSomething(QString &errorString) final;
    void close() final;
    void free() final;
signals:
    void addClient(QString host, quint16 port, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
private:
    SAKSslSocketServerDebugPage *mDebugPage;
    SAKSslSocketTcpServer *mTcpServer;
    SAKSslSocketServerDeviceController *mDeviceController;
};

#endif
