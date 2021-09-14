/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebugger.hh"
#include "SAKPluginCharts.hh"
#include "SAKChartsXYSerial.hh"
#include "SAKChartsThroughputWidget.hh"

#include "ui_SAKPluginCharts.h"

SAKPluginCharts::SAKPluginCharts(QSqlDatabase *sqlDatabase,
                                 QSettings *settings,
                                 QString settingsGroup,
                                 QString tableNameSuffix,
                                 QWidget *parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKPluginCharts)
{
    mUi->setupUi(this);

    SAKChartsXYSerial *xySerialView = new SAKChartsXYSerial(sqlDatabase,
                                                            settings,
                                                            settingsGroup,
                                                            tableNameSuffix,
                                                            parent);
    mUi->tabWidget->addTab(xySerialView, tr("XYSerial"));
    connect(this, &SAKPluginCharts::bytesRead,
            xySerialView, &SAKChartsXYSerial::onBytesRead);
}

SAKPluginCharts::~SAKPluginCharts()
{
    delete mUi;
}
