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
class SAKTcpClientDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKTcpClientDevice(SAKTcpClientDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run() final;
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKTcpClientDebugPage *debugPage;
    QTcpSocket *tcpSocket;
private:
    void afterDisconnected();
};

#endif
