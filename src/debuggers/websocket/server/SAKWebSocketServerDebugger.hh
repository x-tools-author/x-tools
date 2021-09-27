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

class SAKWebSocketServerDevice;
class SAKWebSocketServerController;
class SAKWebSocketServerDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKWebSocketServerDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() final;
    SAKDebuggerController *controller() final;
private:
    SAKWebSocketServerDevice *mDevice;
    SAKWebSocketServerController *mController;
};

#endif
