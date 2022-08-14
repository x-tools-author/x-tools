/******************************************************************************
 * Copyright 2022 wuhai(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBLECENTRALDEFBUGGER_HH
#define SAKBLECENTRALDEFBUGGER_HH

#include "SAKDebugger.hh"

class SAKBleCentralDevice;
class SAKBleCentralController;
class SAKBleCentralDebugger : public SAKDebugger
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKBleCentralDebugger(QSettings *settings,
                                      const QString settingsGroup,
                                      QSqlDatabase *sqlDatabase,
                                      QWidget *parent = Q_NULLPTR);
protected:
    SAKDebuggerDevice* device() override;
    SAKDebuggerController *controller() override;
private:
    SAKBleCentralDevice *mDevice;
    SAKBleCentralController *mController;
};

#endif
