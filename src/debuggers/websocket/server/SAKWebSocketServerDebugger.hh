/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETSERVERDEBUGGER_HH
#define SAKWEBSOCKETSERVERDEBUGGER_HH

#include "SAKDebugger.hh"

class SAKDebugPageController;
class SAKWebSocketServerDevice;
class SAKWebSocketServerController;
/// @brief Web socket server debugging page
class SAKWebSocketServerDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKWebSocketServerDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
protected:
    SAKDebugPageDevice* device() override;
    SAKDebugPageController *controller() override;
private:
    SAKDebugPageDevice *mDevice;
    SAKDebugPageController *mDeviceController;
};

#endif
