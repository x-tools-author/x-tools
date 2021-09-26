/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPCLIENTDEBUGGER_HH
#define SAKTCPCLIENTDEBUGGER_HH

#include "SAKDebugger.hh"

class SAKTcpClientDevice;
class SAKTcpClientController;
class SAKTcpClientDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKTcpClientDebugger(QSettings *settings,
                                     const QString settingsGroup,
                                     QSqlDatabase *sqlDatabase,
                                     QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() override;
    SAKDebuggerController *controller() override;
private:
    SAKTcpClientDevice *mDevice;
    SAKTcpClientController *mController;
};

#endif
