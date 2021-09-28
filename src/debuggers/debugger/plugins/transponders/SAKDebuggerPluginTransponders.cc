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
                                                             QString tableNameSuffix,
                                                             QWidget *parent)
    :QWidget(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
    ,mTableNameSuffix(tableNameSuffix)
    ,mUi (new Ui::SAKDebuggerPluginTransponders)
{
    mUi->setupUi(this);
    mUi->tabWidget->clear();

    auto *serialPort = new SAKTransponderSerialPort(sqlDatabase,
                                                    settings,
                                                    settingsGroup,
                                                    tableNameSuffix + "SerialPort");
    connect(this, &SAKDebuggerPluginTransponders::bytesRead,
            serialPort, &SAKTransponderSerialPort::onBytesRead);
    connect(serialPort, &SAKTransponderSerialPort::invokeWriteCookedBytes,
            this, &SAKDebuggerPluginTransponders::invokeWriteCookedBytes);
    serialPort->setContentsMargins(6, 6, 6, 6);
    mUi->tabWidget->addTab(serialPort, tr("SerialPort"));
}

SAKDebuggerPluginTransponders::~SAKDebuggerPluginTransponders()
{
    delete mUi;
}
