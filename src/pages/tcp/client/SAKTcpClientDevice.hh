/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPCLIENTDEVICE_HH
#define SAKTCPCLIENTDEVICE_HH

#include <QThread>
#include <QTcpSocket>

#include "SAKDebugPageDevice.hh"

class SAKTcpClientDebugPage;
class SAKTcpClientDeviceController;
class SAKTcpClientDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKTcpClientDevice(SAKTcpClientDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
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
    SAKTcpClientDebugPage *mDebugPage;
    QTcpSocket *mTcpSocket;
    SAKTcpClientDeviceController *mDeviceController;
signals:
    void clientInfoChange(QString info);
};

#endif
