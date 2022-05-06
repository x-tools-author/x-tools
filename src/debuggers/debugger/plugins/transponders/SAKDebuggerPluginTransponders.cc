/****************************************************************************************
 * Copyright 2018-2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKUdpTransponders.hh"
#include "SAKTcpTransponders.hh"
#include "SAKWebSocketTransponders.hh"
#ifdef SAK_IMPORT_MODULE_SERIALPORT
#include "SAKSerialPortTransponders.hh"
#endif
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
#ifdef SAK_IMPORT_MODULE_SERIALPORT
    auto *serialPort = new SAKSerialPortTransponders(sqlDatabase,
                                                     settings,
                                                     settingsGroup,
                                                     tableNameSuffix + "SerialPort");
    connect(this, &SAKDebuggerPluginTransponders::bytesRead,
            serialPort, &SAKSerialPortTransponders::onBytesRead);
    connect(serialPort, &SAKSerialPortTransponders::invokeWriteCookedBytes,
            this, &SAKDebuggerPluginTransponders::invokeWriteCookedBytes);
    serialPort->setContentsMargins(6, 6, 6, 6);
    mUi->tabWidget->addTab(serialPort, tr("SerialPort"));
#endif

    auto udpTransponder = new SAKUdpTransponders(sqlDatabase,
                                                 settings,
                                                 settingsGroup,
                                                 tableNameSuffix + "UdpClient");
    connect(this, &SAKDebuggerPluginTransponders::bytesRead,
            udpTransponder, &SAKUdpTransponders::onBytesRead);
    connect(udpTransponder, &SAKUdpTransponders::invokeWriteCookedBytes,
            this, &SAKDebuggerPluginTransponders::invokeWriteCookedBytes);
    udpTransponder->setContentsMargins(6, 6, 6, 6);
    mUi->tabWidget->addTab(udpTransponder, tr("UdpClient"));


    auto tcpTransponder = new SAKTcpTransponders(sqlDatabase,
                                                 settings,
                                                 settingsGroup,
                                                 tableNameSuffix + "TcpClient");
    connect(this, &SAKDebuggerPluginTransponders::bytesRead,
            tcpTransponder, &SAKTcpTransponders::onBytesRead);
    connect(tcpTransponder, &SAKTcpTransponders::invokeWriteCookedBytes,
            this, &SAKDebuggerPluginTransponders::invokeWriteCookedBytes);
    tcpTransponder->setContentsMargins(6, 6, 6, 6);
    mUi->tabWidget->addTab(tcpTransponder, tr("TcpClient"));


    auto wsTransponder = new SAKWebSocketTransponders(sqlDatabase,
                                                 settings,
                                                 settingsGroup,
                                                 tableNameSuffix + "WebSocketClient");
    connect(this, &SAKDebuggerPluginTransponders::bytesRead,
            wsTransponder, &SAKWebSocketTransponders::onBytesRead);
    connect(wsTransponder, &SAKWebSocketTransponders::invokeWriteCookedBytes,
            this, &SAKDebuggerPluginTransponders::invokeWriteCookedBytes);
    wsTransponder->setContentsMargins(6, 6, 6, 6);
    mUi->tabWidget->addTab(wsTransponder, tr("WebSocketClient"));


    QString pageIndexSettingsKey = settingsGroup.append("/transponders/pageIndex");
    int pageIndex = settings->value(pageIndexSettingsKey).toInt();
    mUi->tabWidget->setCurrentIndex(pageIndex);
    connect(mUi->tabWidget, &QTabWidget::currentChanged, this, [=](int index){
        settings->setValue(pageIndexSettingsKey, index);
    });
}

SAKDebuggerPluginTransponders::~SAKDebuggerPluginTransponders()
{
    delete mUi;
}
