/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSSLSOCKETCLIENTDEVICE_HH
#define SAKSSLSOCKETCLIENTDEVICE_HH

#include <QThread>
#include <QSslSocket>

#include "SAKDebugPageDevice.hh"

class SAKSslSocketClientDebugPage;
class SAKSslSocketClientDeviceController;
class SAKSslSocketClientDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKSslSocketClientDevice(SAKSslSocketClientDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
private:
    bool initializing(QString &errorString) final;
    bool open(QString &errorString) final;
    QByteArray read() final;
    QByteArray write(QByteArray bytes) final;
    bool checkSomething(QString &errorString) final;
    void close() final;
    void free() final;
private:
    QString mLocalHost;
    quint16 mLocalPort;
    bool mSpecifyClientAddressAndPort;
    QString mServerHost;
    quint16 mServerPort;
    SAKSslSocketClientDebugPage *mDebugPage;
    QSslSocket *mSslSocket;
    SAKSslSocketClientDeviceController *mDeviceController;
signals:
    void clientInfoChange(QString info);
};

#endif
