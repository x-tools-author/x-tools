/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKUdpTransmitterTool.hh"
#include "SAKUdpTransmitterToolUiEditor.hh"
#include "ui_SAKUdpTransmitterToolUiEditor.h"

SAKUdpTransmitterToolUiEditor::SAKUdpTransmitterToolUiEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKUdpTransmitterToolUiEditor)
{
    ui->setupUi(this);
    connect(ui->pushButtonApply, &QPushButton::clicked,
            this, &SAKUdpTransmitterToolUiEditor::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked,
            this, &SAKUdpTransmitterToolUiEditor::reject);
}

SAKUdpTransmitterToolUiEditor::~SAKUdpTransmitterToolUiEditor()
{
    delete ui;
}

QJsonObject SAKUdpTransmitterToolUiEditor::parameters()
{
    bool enable = ui->checkBoxEnable->isChecked();
    QString clientIp = ui->comboBoxClientIp->currentText();
    int clientPort = ui->spinBoxClientPort->value();
    bool specifiedClientIpPort =
        ui->checkBoxSpecifiedClientIpAndPort->isChecked();
    QString serverIp = ui->comboBoxServerIp->currentText();
    int serverPort = ui->spinBoxServerPort->value();

    QJsonObject obj;
    SAKUdpTransmitterTool::ItemContextKeys keys;
    obj.insert(keys.enable, enable);
    obj.insert(keys.clientIp, clientIp);
    obj.insert(keys.clientPort, clientPort);
    obj.insert(keys.specifiedClientIpPort, specifiedClientIpPort);
    obj.insert(keys.serverIp, serverIp);
    obj.insert(keys.serverPort, serverPort);
    return obj;
}

void SAKUdpTransmitterToolUiEditor::setParameters(const QJsonObject &params)
{
    SAKUdpTransmitterTool::ItemContextKeys keys;
    bool enable = params.value(keys.enable).toBool();
    QString clientIp = params.value(keys.clientIp).toString();
    int clientPort = params.value(keys.clientPort).toInt();
    bool specifiedClientIpPort =
        params.value(keys.specifiedClientIpPort).toBool();
    QString serverIp = params.value(keys.serverIp).toString();
    int serverPort = params.value(keys.serverPort).toInt();

    ui->checkBoxEnable->setChecked(enable);
    ui->comboBoxClientIp->setCurrentText(clientIp);
    ui->spinBoxClientPort->setValue(clientPort);
    ui->checkBoxSpecifiedClientIpAndPort->setChecked(specifiedClientIpPort);
    ui->comboBoxServerIp->setCurrentText(serverIp);
    ui->spinBoxServerPort->setValue(serverPort);
}
