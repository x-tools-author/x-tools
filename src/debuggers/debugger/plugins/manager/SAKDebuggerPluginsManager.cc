/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKDebuggerPluginsManager.hh"
#include "ui_SAKDebuggerPluginsManager.h"

SAKDebuggerPluginsManager::SAKDebuggerPluginsManager(QWidget *parent)
    :QWidget(parent)
{
    mUi->setupUi(this);
}

SAKDebuggerPluginsManager::~SAKDebuggerPluginsManager()
{
    delete mUi;
}
