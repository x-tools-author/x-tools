/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
