/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QMessageBox>

#include "saksocketclienttool.h"
#include "saksocketclienttoolui.h"
#include "ui_saksocketclienttoolui.h"

SAKSocketClientToolUi::SAKSocketClientToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSocketClientToolUi)
{
    ui->setupUi(this);

    connect(ui->comboBoxClientAddress,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this,
            &SAKSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged);
    connect(ui->spinBoxClientPort,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &SAKSocketClientToolUi::onSpinBoxClientPortValueChanged);
    connect(ui->comboBoxServerAddress,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentTextChanged),
            this,
            &SAKSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged);
    connect(ui->spinBoxServerPort,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &SAKSocketClientToolUi::onSpinBoxServerPortValueChanged);
    connect(ui->checkBoxSpecifyIpAndPort,
            &QCheckBox::clicked,
            this,
            &SAKSocketClientToolUi::onCheckBoxSpecifyIpAndPortClicked);
}

SAKSocketClientToolUi::~SAKSocketClientToolUi()
{
    delete ui;
}

void SAKSocketClientToolUi::onIsWorkingChanged(bool isWorking)
{
    ui->comboBoxClientAddress->setEnabled(!isWorking);
    ui->spinBoxClientPort->setEnabled(!isWorking);
    ui->checkBoxSpecifyIpAndPort->setEnabled(!isWorking);
}

void SAKSocketClientToolUi::onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKSocketClientTool")) {
        qWarning(mLoggingCategory) << "Invalid SAKSocketClientTool object,"
                                      " the operation will be ignored!";
        return;
    }

    if (!tool->inherits("SAKWebSocketClientTool")) {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }

    mTool = qobject_cast<SAKSocketClientTool *>(tool);
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

    connect(mTool, &SAKSocketClientTool::bindingIpPortChanged, this, [=]() {
        QString ipport = mTool->bindingIpPort();
        ui->labelContext->setText(ipport);
    });
    connect(mTool, &SAKSocketClientTool::finished, this, [=]() {
        ui->labelContext->setText(tr("Closed"));
    });
}

void SAKSocketClientToolUi::onComboBoxClientAddressActived()
{
    if (mTool) {
        QString ip = ui->comboBoxClientAddress->currentText().trimmed();
        mTool->setClientIp(ip);
    }
}

void SAKSocketClientToolUi::onSpinBoxClientPortValueChanged(int value)
{
    if (mTool) {
        mTool->setClientPort(value);
    }
}

void SAKSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged()
{
    if (mTool) {
        QString ip = ui->comboBoxServerAddress->currentText().trimmed();
        mTool->setServerIp(ip);
    }
}

void SAKSocketClientToolUi::onSpinBoxServerPortValueChanged(int value)
{
    if (mTool) {
        mTool->setServerPort(value);
    }
}

void SAKSocketClientToolUi::onCheckBoxSpecifyIpAndPortClicked()
{
    if (mTool) {
        bool checked = ui->checkBoxSpecifyIpAndPort->isChecked();
        mTool->setSpecifyClientIpPort(checked);
    }
}
