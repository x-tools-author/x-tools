/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKTransponderSerialPort.hh"
#include "SAKDebuggerPluginTransponders.hh"
#include "ui_SAKDebuggerPluginTransponders.h"

SAKDebuggerPluginTransponders::SAKDebuggerPluginTransponders(QSqlDatabase *sqlDatabase,
                                                             QSettings *settings,
                                                             QString settingsGroup,
                                                             QWidget *parent)
    :QWidget(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
    ,mUi (new Ui::SAKDebuggerPluginTransponders)
{
    mUi->setupUi(this);
    mUi->tabWidget->clear();

    auto transponderSerialPort = new SAKTransponderSerialPort(sqlDatabase,
                                                              settings,
                                                              settingsGroup,
                                                              "TransponderSerialPort");
    mUi->tabWidget->addTab(transponderSerialPort, tr("SerialPort"));
}

SAKDebuggerPluginTransponders::~SAKDebuggerPluginTransponders()
{
    delete mUi;
}

void SAKDebuggerPluginTransponders::onBytesRead(QByteArray bytes)
{
    Q_UNUSED(bytes);
}
