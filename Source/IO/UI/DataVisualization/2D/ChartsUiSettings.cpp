/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ChartsUiSettings.h"
#include "ui_ChartsUiSettings.h"

#include <QChartView>
#include <QCheckBox>
#include <QPointF>
#include <QTimer>

#include "IO/IO/DataVisualization/2D/Charts.h"

namespace xTools {

ChartsUiSettings::ChartsUiSettings(QWidget *parent)
    : QPushButton(parent)
    , ui(new Ui::ChartsUiSettings)
{
    ui->setupUi(this);
}

ChartsUiSettings::~ChartsUiSettings()
{
    delete ui;
}

} // namespace xTools
