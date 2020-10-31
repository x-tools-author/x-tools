/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusDebugPage.hh"
#include "ui_SAKModbusDebugPage.h"

SAKModbusDebugPage::SAKModbusDebugPage(int type, QString name, QSettings *settings, QSplashScreen *splashScreen, QSqlDatabase *sqlDatabase, QWidget *parent)
    :QWidget(parent)
    ,mType(type)
    ,mName(name)
    ,mSettings(settings)
    ,mSplashScreen(splashScreen)
    ,mSqlDatabase(sqlDatabase)
    ,ui(new Ui::SAKModbusDebugPage)
{
    ui->setupUi(this);
}

SAKModbusDebugPage::~SAKModbusDebugPage()
{
    delete ui;
}

void SAKModbusDebugPage::on_deviceTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
}
