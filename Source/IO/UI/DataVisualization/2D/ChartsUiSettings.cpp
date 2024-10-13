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
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPointF>
#include <QTimer>

namespace xTools {

ChartsUiSettings::ChartsUiSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartsUiSettings)
{
    ui->setupUi(this);

    ui->gridLayoutChartControl->addWidget(new QLabel(tr("Channel"), this), 0, 0, Qt::AlignCenter);
    ui->gridLayoutChartControl->addWidget(new QLabel(tr("Visible"), this), 0, 1, Qt::AlignCenter);
    ui->gridLayoutChartControl->addWidget(new QLabel(tr("Type"), this), 0, 2, Qt::AlignCenter);
    ui->gridLayoutChartControl->addWidget(new QLabel(tr("Color"), this), 0, 3, Qt::AlignCenter);
    ui->gridLayoutChartControl->addWidget(new QLabel(tr("Name"), this), 0, 4, Qt::AlignCenter);
    for (int i = 0; i < 16; ++i) {
        int row = i + 1;
        QString str = QString::number(row);
        ui->gridLayoutChartControl->addWidget(new QLabel(str, this), row, 0, Qt::AlignCenter);
        ui->gridLayoutChartControl->addWidget(new QCheckBox(this), row, 1, Qt::AlignCenter);
        ui->gridLayoutChartControl->addWidget(new QComboBox(this), row, 2, Qt::AlignCenter);
        ui->gridLayoutChartControl->addWidget(new QPushButton(this), row, 3, Qt::AlignCenter);
        ui->gridLayoutChartControl->addWidget(new QLineEdit(this), row, 4, Qt::AlignCenter);
    }
}

ChartsUiSettings::~ChartsUiSettings()
{
    delete ui;
}

} // namespace xTools
