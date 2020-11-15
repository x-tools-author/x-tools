/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETCLIENTDEBUGPAGE_HH
#define SAKWEBSOCKETCLIENTDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKDebugPageController;
class SAKWebSocketClientDevice;
class SAKWebSocketClientDeviceController;
class SAKWebSocketClientDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKWebSocketClientDebugPage(int type, QString name, QWidget *parent = Q_NULLPTR);
};

#endif
