/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKChartsXYSerialChartViewSettingsDialog.hh"

#include "ui_SAKChartsXYSerialChartViewSettingsDialog.h"

SAKChartsXYSerialChartViewSettingsDialog::SAKChartsXYSerialChartViewSettingsDialog(QWidget *parent)
    :QDialog (parent)
    ,mUi (new Ui::SAKChartsXYSerialChartViewSettingsDialog)
{
    mUi->setupUi(this);
}

SAKChartsXYSerialChartViewSettingsDialog::~SAKChartsXYSerialChartViewSettingsDialog()
{
    delete mUi;
}
