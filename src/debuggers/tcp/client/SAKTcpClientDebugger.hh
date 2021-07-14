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

class SAKTcpClientController;
class SAKTcpClientDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKTcpClientDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
protected:
    SAKDebugPageDevice* device() override;
    SAKDebugPageController *controller() override;
private:
    SAKDebugPageDevice *mDevice;
    SAKDebugPageController *mDeviceController;
};

#endif
