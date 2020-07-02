/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSCTPSERVERDEVICE_HH
#define SAKSCTPSERVERDEVICE_HH

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "SAKDevice.hh"

class SAKSctpServerDebugPage;
class SAKSctpServerDeviceController;
class SAKSctpServerDevice:public SAKDevice
{
    Q_OBJECT
public:
    SAKSctpServerDevice(SAKSctpServerDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run();    
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKSctpServerDebugPage *debugPage;
    QTcpServer *tcpServer;
private:
    void innerReadBytes(QTcpSocket *socket, SAKSctpServerDeviceController *deviceController);
    void innerWriteBytes(QTcpSocket *socket, QByteArray bytes, SAKSctpServerDeviceController *deviceController);
};

#endif
