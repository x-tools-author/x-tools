/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKWebSocketServerDevice_HH
#define SAKWebSocketServerDevice_HH

#include <QThread>
#include <QTcpSocket>

#include "SAKDevice.hh"

class SAKWebSocketServerDebugPage;
/// @brief web socket服务器设备
class SAKWebSocketServerDevice : public SAKDevice
{
    Q_OBJECT
public:
    SAKWebSocketServerDevice(SAKWebSocketServerDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run() final;
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKWebSocketServerDebugPage *debugPage;
    QTcpSocket *tcpSocket;
private:
    void afterDisconnected();
};

#endif
