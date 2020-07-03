/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSCTPCLIENTDEVICE_HH
#define SAKSCTPCLIENTDEVICE_HH

#include <QThread>
#include <QSctpSocket>

#include "SAKDevice.hh"

class SAKSctpClientDebugPage;
class SAKSctpClientDevice:public SAKDevice
{
    Q_OBJECT
public:
    SAKSctpClientDevice(SAKSctpClientDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run() final;
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKSctpClientDebugPage *debugPage;
    QSctpSocket *sctpSocket;
private:
    void afterDisconnected();
};

#endif
