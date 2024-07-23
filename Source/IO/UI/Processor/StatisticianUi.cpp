/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "StatisticianUi.h"
#include "ui_StatisticianUi.h"

#include "StatisticianUi.h"

StatisticianUi::StatisticianUi(QWidget *parent)
    : AbstractIOUi{parent}
    , ui(new Ui::StatisticianUi)
{
    ui->setupUi(this);
    updateInfo(0, 0, 0);
}

StatisticianUi::~StatisticianUi()
{
    delete ui;
}

QVariantMap StatisticianUi::save() const
{
    return {};
}

void StatisticianUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void StatisticianUi::updateInfo(int frame, int bytes, int speed)
{
    QString info = tr("%1 frames, %2 bytes, %3B/s").arg(frame).arg(bytes).arg(speed);
    ui->label->setText(info);
}
