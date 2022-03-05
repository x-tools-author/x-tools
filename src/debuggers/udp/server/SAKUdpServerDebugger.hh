/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPSERVERDEBUGGER_HH
#define SAKUDPSERVERDEBUGGER_HH

#include "SAKDebugger.hh"

class SAKUdpServerDevice;
class SAKUdpServerController;
class SAKUdpServerMulticast;
class SAKUdpServerDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKUdpServerDebugger(QSettings *settings,
                                     const QString settingsGroup,
                                     QSqlDatabase *sqlDatabase,
                                     QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() final;
    SAKDebuggerController *controller() final;
    void addActionToDeviceMenu(QMenu *menu) final;
    void updateUiState(bool opened) final;
private:
    SAKUdpServerDevice *mDevice;
    SAKUdpServerController *mController;
    SAKUdpServerMulticast *mMulticast;
};
#endif
