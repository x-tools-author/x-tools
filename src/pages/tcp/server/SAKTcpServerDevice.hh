/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTCPSERVERDEVICE_HH
#define SAKTCPSERVERDEVICE_HH

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "SAKDevice.hh"

class SAKTcpServerDebugPage;
class SAKTcpServerDeviceController;
class SAKTcpServerDevice:public SAKDevice
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
