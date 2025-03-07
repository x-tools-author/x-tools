/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "chartstestui.h"
#include "ui_chartstestui.h"

#include "chartstest.h"
#include "common/xtools.h"

ChartsTestUi::ChartsTestUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::ChartsTestUi)
{
    ui->setupUi(this);

    setupChartsDataFormat(ui->comboBoxFormat);
}

Device *ChartsTestUi::newDevice()
{
    return new ChartsTest(this);
}

QVariantMap ChartsTestUi::save() const
{
    QVariantMap map;
    ChartsTestDataKeys keys;
    map[keys.dataFormat] = ui->comboBoxFormat->currentData();
    map[keys.channels] = ui->spinBoxChannels->value();
    return map;
}

void ChartsTestUi::load(const QVariantMap &map)
{
    if (map.isEmpty()) {
        return;
    }

    ChartsTestDataKeys keys;
    int index = ui->comboBoxFormat->findData(map.value(keys.dataFormat));
    ui->comboBoxFormat->setCurrentIndex(index < 0 ? 0 : index);
    ui->spinBoxChannels->setValue(map.value(keys.channels).toInt());
}
