/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPSERVERDEVICE_HH
#define SAKTCPSERVERDEVICE_HH

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "SAKDebugPageDevice.hh"

class SAKTcpServerDebugPage;
class SAKTcpServerDeviceController;
class SAKTcpServerDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKTcpServerDevice(SAKTcpServerDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run();    
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKTcpServerDebugPage *debugPage;
    QTcpServer *tcpServer;
private:
    void innerReadBytes(QTcpSocket *socket, SAKTcpServerDeviceController *deviceController);
    void innerWriteBytes(QTcpSocket *socket, QByteArray bytes, SAKTcpServerDeviceController *deviceController);
};

#endif
