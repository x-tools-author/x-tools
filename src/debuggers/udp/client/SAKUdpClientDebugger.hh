/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTDEBUGGER_HH
#define SAKUDPCLIENTDEBUGGER_HH

#include "SAKDebugger.hh"
#include "SAKCommonDataStructure.hh"

class SAKUdpClientDevice;
class SAKUdpClientAdvanced;
class SAKUdpClientController;
class SAKUdpClientDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKUdpClientDebugger(QSettings *settings,
                                     const QString settingsGroup,
                                     QSqlDatabase *sqlDatabase,
                                     QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() final;
    SAKDebuggerController *controller() final;
    void addActionToDeviceMenu(QMenu *menu) final;
    void updateUiState(bool opened) final;
private:
    SAKUdpClientDevice *mDevice;
    SAKUdpClientAdvanced *mAdvanced;
    SAKUdpClientController *mController;
};

#endif
