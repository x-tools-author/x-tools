/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETCLIENTDEVICE_HH
#define SAKWEBSOCKETCLIENTDEVICE_HH

#include <QAbstractSocket>
#include "SAKDebugPageDevice.hh"

class SAKWebSocketClientDebugPage;
/// @brief web socket 客户端
class SAKWebSocketClientDevice:public SAKDebugPageDevice
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
