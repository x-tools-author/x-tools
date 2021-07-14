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
class SAKUdpServerDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKUdpServerDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
};
#endif
