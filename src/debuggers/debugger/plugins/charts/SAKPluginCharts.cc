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
#include "SAKChartsXYSerialWidget.hh"
#include "SAKChartsThroughputWidget.hh"

#include "ui_SAKPluginCharts.h"

SAKPluginCharts::SAKPluginCharts(QWidget *parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKPluginCharts)
{
    mUi->setupUi(this);

    mUi->tabWidget->addTab(new SAKChartsXYSerialWidget(), tr("XYSerial"));
}

SAKPluginCharts::~SAKPluginCharts()
{
    delete mUi;
}
