/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKDebugger.hh"
#include "SAKDebuggerController.hh"

SAKDebuggerController::SAKDebuggerController(QSettings *settings,
                                             const QString &settingsGroup,
                                             QWidget *parent)
    :QWidget(parent)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
{

}

SAKDebuggerController::~SAKDebuggerController()
{

}
