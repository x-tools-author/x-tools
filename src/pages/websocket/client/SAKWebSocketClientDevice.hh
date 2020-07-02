/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKWEBSOCKETCLIENTDEVICE_HH
#define SAKWEBSOCKETCLIENTDEVICE_HH

#include <QAbstractSocket>
#include "SAKDevice.hh"

class SAKWebSocketClientDebugPage;
/// @brief web socket 客户端
class SAKWebSocketClientDevice:public SAKDevice
{
    Q_OBJECT
public:
    SAKWebSocketClientDevice(SAKWebSocketClientDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    void run() final;
private:
    SAKWebSocketClientDebugPage *debugPage;
private slots:
    void errorSlot(QAbstractSocket::SocketError error);
};

#endif
