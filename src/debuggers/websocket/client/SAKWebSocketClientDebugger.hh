/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETCLIENTDEBUGGER_HH
#define SAKWEBSOCKETCLIENTDEBUGGER_HH

#include "SAKDebugger.hh"

class SAKDebugPageController;
class SAKWebSocketClientDevice;
class SAKWebSocketClientController;
class SAKWebSocketClientDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKWebSocketClientDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
protected:
    SAKDebugPageDevice* device() override;
    SAKDebugPageController *controller() override;
private:
    SAKDebugPageDevice *mDevice;
    SAKDebugPageController *mDeviceController;
};

#endif
