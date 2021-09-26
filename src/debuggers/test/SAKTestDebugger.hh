/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKTESTDEBUGGER_HH
#define SAKTESTDEBUGGER_HH

#include "SAKDebugger.hh"
#include "SAKTestDebuggerDevice.hh"
#include "SAKTestDebuggerController.hh"

class SAKTestDebuggerDevice;
class SAKTestDebuggerController;
class SAKTestDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKTestDebugger(QSettings *settings,
                                const QString settingsGroup,
                                QSqlDatabase *sqlDatabase,
                                QWidget *parent = Q_NULLPTR);
    SAKDebuggerDevice* device() final;
    SAKDebuggerController *controller() final;
private:
    SAKTestDebuggerDevice *mDevice;
    SAKTestDebuggerController *mController;
private:
    void generateReadData();
    void generateWriteData();
};

#endif
