/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketClientToolUi.h"
#include "ui_xToolsSocketClientToolUi.h"

#include <QMessageBox>

#include "xToolsSocketClientTool.h"

xToolsSocketClientToolUi::xToolsSocketClientToolUi(QWidget *parent)
    : xToolsCommunicationToolUi{parent}
    , ui(new Ui::xToolsSocketClientToolUi)
{
    ui->setupUi(this);

    connect(ui->comboBoxClientAddress,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this,
            &xToolsSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged);
    connect(ui->spinBoxClientPort,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsSocketClientToolUi::onSpinBoxClientPortValueChanged);
    connect(ui->comboBoxServerAddress,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentTextChanged),
            this,
            &xToolsSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged);
    connect(ui->spinBoxServerPort,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsSocketClientToolUi::onSpinBoxServerPortValueChanged);
    connect(ui->checkBoxSpecifyIpAndPort,
            &QCheckBox::clicked,
            this,
            &xToolsSocketClientToolUi::onCheckBoxSpecifyIpAndPortClicked);
}

xToolsSocketClientToolUi::~xToolsSocketClientToolUi()
{
    delete ui;
}

void xToolsSocketClientToolUi::onIsWorkingChanged(bool isWorking)
{
    ui->comboBoxClientAddress->setEnabled(!isWorking);
    ui->spinBoxClientPort->setEnabled(!isWorking);
    ui->checkBoxSpecifyIpAndPort->setEnabled(!isWorking);
}

void xToolsSocketClientToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("xToolsSocketClientTool")) {
        qWarning() << "Invalid xToolsSocketClientTool object, the operation will be ignored!";
        return;
    }

    if (!tool->inherits("xToolsWebSocketClientTool")) {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }

    mTool = qobject_cast<xToolsSocketClientTool *>(tool);
    if (!mTool) {
        qWarning() << "qobject_cast<>() return nullptr";
        return;
    }

    ui->comboBoxClientAddress->setGroupKey(settingsGroup, "clientAddress");
    ui->spinBoxClientPort->setGroupKey(settingsGroup, "clientPort");
    ui->checkBoxSpecifyIpAndPort->setGroupKey(settingsGroup, "specifyIpAndPort");
    ui->comboBoxServerAddress->setGroupKey(settingsGroup, "serverAddress");
    ui->spinBoxServerPort->setGroupKey(settingsGroup, "serverPort");
    ui->comboBoxMessageType->setGroupKey(settingsGroup, "messageType");

    mTool->setClientIp(ui->comboBoxClientAddress->currentText().trimmed());
    mTool->setClientPort(ui->spinBoxClientPort->value());
    mTool->setServerIp(ui->comboBoxServerAddress->currentText().trimmed());
    mTool->setServerPort(ui->spinBoxServerPort->value());
    mTool->setSpecifyClientIpPort(ui->checkBoxSpecifyIpAndPort->isChecked());

    connect(mTool, &xToolsSocketClientTool::bindingIpPortChanged, this, [=]() {
        QString ipport = mTool->bindingIpPort();
        ui->labelContext->setText(ipport);
    });
    connect(mTool, &xToolsSocketClientTool::finished, this, [=]() {
        ui->labelContext->setText(tr("Closed"));
    });
}

void xToolsSocketClientToolUi::onComboBoxClientAddressActived()
{
    if (mTool) {
        QString ip = ui->comboBoxClientAddress->currentText().trimmed();
        mTool->setClientIp(ip);
    }
}

void xToolsSocketClientToolUi::onSpinBoxClientPortValueChanged(int value)
{
    if (mTool) {
        mTool->setClientPort(value);
    }
}

void xToolsSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged()
{
    if (mTool) {
        QString ip = ui->comboBoxServerAddress->currentText().trimmed();
        mTool->setServerIp(ip);
    }
}

void xToolsSocketClientToolUi::onSpinBoxServerPortValueChanged(int value)
{
    if (mTool) {
        mTool->setServerPort(value);
    }
}

void xToolsSocketClientToolUi::onCheckBoxSpecifyIpAndPortClicked()
{
    if (mTool) {
        bool checked = ui->checkBoxSpecifyIpAndPort->isChecked();
        mTool->setSpecifyClientIpPort(checked);
    }
}
