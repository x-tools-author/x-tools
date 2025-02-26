/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hidui.h"
#include "ui_hidui.h"

#include <QHid>
#include <QHidInfo>

namespace xTools {

HidUi::HidUi(CommunicationType type, QWidget *parent)
    : CommunicationUi(type, parent)
    , ui(new Ui::HidUi)
{
    ui->setupUi(this);

    refresh();

    setupBaudRate(ui->comboBoxBaudRate);
    setupDataBits(ui->comboBoxDataBits);
    setupParity(ui->comboBoxParity);
    setupStopBits(ui->comboBoxStopBits);
    setupFlowControl(ui->comboBoxFlowControl);
}

QVariantMap HidUi::save() const
{
    QVariantMap map;
    map["portName"] = ui->comboBoxPortName->currentText();
    map["baudRate"] = ui->comboBoxBaudRate->currentText().toInt();
    map["dataBits"] = ui->comboBoxDataBits->currentData().toInt();
    map["parity"] = ui->comboBoxParity->currentData().toInt();
    map["stopBits"] = ui->comboBoxStopBits->currentData().toInt();
    map["flowControl"] = ui->comboBoxFlowControl->currentData().toInt();
    return map;
}

void HidUi::load(const QVariantMap &map)
{
    if (map.isEmpty()) {
        return;
    }

    QString portName = map.value("portName").toString();
    int baudRate = map.value("baudRate").toInt();
    int dataBits = map.value("dataBits").toInt();
    int parity = map.value("parity").toInt();
    int stopBits = map.value("stopBits").toInt();
    int flowControl = map.value("flowControl").toInt();

    ui->comboBoxPortName->setCurrentText(portName);
    ui->comboBoxBaudRate->setCurrentText(QString::number(baudRate));
    ui->comboBoxDataBits->setCurrentIndex(ui->comboBoxDataBits->findData(dataBits));
    ui->comboBoxParity->setCurrentIndex(ui->comboBoxParity->findData(parity));
    ui->comboBoxStopBits->setCurrentIndex(ui->comboBoxStopBits->findData(stopBits));
    ui->comboBoxFlowControl->setCurrentIndex(ui->comboBoxFlowControl->findData(flowControl));
}

void HidUi::refresh()
{
    setupPortName(ui->comboBoxPortName);
}

} // namespace xTools
