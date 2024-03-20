/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketClientTransmitterToolUiEditor.h"
#include "ui_xToolsSocketClientTransmitterToolUiEditor.h"

#include "xToolsSocketClientTransmitterTool.h"

xToolsSocketClientTransmitterToolUiEditor::xToolsSocketClientTransmitterToolUiEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::xToolsSocketClientTransmitterToolUiEditor)
{
    ui->setupUi(this);
    connect(ui->pushButtonApply,
            &QPushButton::clicked,
            this,
            &xToolsSocketClientTransmitterToolUiEditor::accept);
    connect(ui->pushButtonCancel,
            &QPushButton::clicked,
            this,
            &xToolsSocketClientTransmitterToolUiEditor::reject);
}

xToolsSocketClientTransmitterToolUiEditor::~xToolsSocketClientTransmitterToolUiEditor()
{
    delete ui;
}

void xToolsSocketClientTransmitterToolUiEditor::setWebSocketContextVisible(bool v)
{
    if (v) {
        ui->labelMessageType->show();
        ui->comboBoxMessageType->show();
    } else {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }
}

QJsonObject xToolsSocketClientTransmitterToolUiEditor::parameters()
{
    bool enable = ui->checkBoxEnable->isChecked();
    QString clientIp = ui->comboBoxClientIp->currentText();
    int clientPort = ui->spinBoxClientPort->value();
    bool specifiedClientIpPort = ui->checkBoxSpecifiedClientIpAndPort->isChecked();
    QString serverIp = ui->comboBoxServerIp->currentText();
    int serverPort = ui->spinBoxServerPort->value();
    int messageType = ui->comboBoxMessageType->currentData().toInt();

    QJsonObject obj;
    xToolsSocketClientTransmitterTool::ItemContextKeys keys;
    obj.insert(keys.enable, enable);
    obj.insert(keys.clientIp, clientIp);
    obj.insert(keys.clientPort, clientPort);
    obj.insert(keys.specifiedClientIpPort, specifiedClientIpPort);
    obj.insert(keys.serverIp, serverIp);
    obj.insert(keys.serverPort, serverPort);
    obj.insert(keys.messageType, messageType);
    return obj;
}

void xToolsSocketClientTransmitterToolUiEditor::setParameters(const QJsonObject &params)
{
    xToolsSocketClientTransmitterTool::ItemContextKeys keys;
    bool enable = params.value(keys.enable).toBool();
    QString clientIp = params.value(keys.clientIp).toString();
    int clientPort = params.value(keys.clientPort).toInt();
    bool specifiedClientIpPort = params.value(keys.specifiedClientIpPort).toBool();
    QString serverIp = params.value(keys.serverIp).toString();
    int serverPort = params.value(keys.serverPort).toInt();
    int messageType = params.value(keys.messageType).toInt();

    ui->checkBoxEnable->setChecked(enable);
    ui->comboBoxClientIp->setCurrentText(clientIp);
    ui->spinBoxClientPort->setValue(clientPort);
    ui->checkBoxSpecifiedClientIpAndPort->setChecked(specifiedClientIpPort);
    ui->comboBoxServerIp->setCurrentText(serverIp);
    ui->spinBoxServerPort->setValue(serverPort);
    ui->comboBoxMessageType->setCurrentIndexFromData(messageType);
}
