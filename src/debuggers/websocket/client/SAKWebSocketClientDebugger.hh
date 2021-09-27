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

class SAKWebSocketClientDevice;
class SAKWebSocketClientController;
class SAKWebSocketClientDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKWebSocketClientDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() final;
    SAKDebuggerController *controller() final;
private:
    SAKWebSocketClientDevice *mDevice;
    SAKWebSocketClientController *mController;
};

#endif
