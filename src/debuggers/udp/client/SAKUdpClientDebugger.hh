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

class SAKUdpClientDevice;
class SAKUdpClientController;
class SAKUdpClientDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKUdpClientDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
};

#endif
