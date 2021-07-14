/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPSERVERDEBUGGER_HH
#define SAKTCPSERVERDEBUGGER_HH

#include "SAKDebugPage.hh"

class SAKTcpServerDevice;
class SAKTcpServerController;
class SAKTcpServerDebugger : public SAKDebugPage
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKTcpServerDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
};

#endif
