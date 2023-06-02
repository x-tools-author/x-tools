/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSocketClientTool.hh"
#include "SAKSocketClientToolUi.hh"
#include "ui_SAKSocketClientToolUi.h"

SAKSocketClientToolUi::SAKSocketClientToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSocketClientToolUi)
{
    ui->setupUi(this);

    connect(ui->comboBoxClientAddress,
            QOverload<int>::of(&QComboBox::activated),
            this, &SAKSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged);
    connect(ui->spinBoxClientPort,
            QOverload<int>::of(&SAKSpinBox::valueChanged),
            this, &SAKSocketClientToolUi::onSpinBoxClientPortValueChanged);
    connect(ui->comboBoxServerAddress,
            QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &SAKSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged);
    connect(ui->spinBoxServerPort,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SAKSocketClientToolUi::onSpinBoxServerPortValueChanged);
    connect(ui->checkBoxSpecifyIpAndPort, &QCheckBox::clicked,
            this, &SAKSocketClientToolUi::onCheckBoxSpecifyIpAndPortClicked);
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

void SAKSocketClientToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingsGroup)
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

    mTool = qobject_cast<SAKSocketClientTool*>(tool);
    if (!mTool) {
        qCWarning(mLoggingCategory) << "qobject_cast<>() return nullptr";
        return;
    }

    mTool->setClientIp(ui->comboBoxClientAddress->currentText().trimmed());
    mTool->setClientPort(ui->spinBoxClientPort->value());
    mTool->setServerIp(ui->comboBoxServerAddress->currentText().trimmed());
    mTool->setServerPort(ui->spinBoxServerPort->value());
    mTool->setSpecifyClientIpPort(ui->checkBoxSpecifyIpAndPort->isChecked());

    connect(mTool, &SAKSocketClientTool::bindingIpPortChanged, this, [=](){
        QString ipport = mTool->bindingIpPort();
        ui->labelContext->setText(ipport);
    });
    connect(mTool, &SAKSocketClientTool::finished, this, [=](){
        ui->labelContext->setText(tr("Closed"));
    });

    ui->comboBoxClientAddress->setGroupKey(settingsGroup, "clientAddress");
    ui->spinBoxClientPort->setGroupKey(settingsGroup, "clientGroup");
    ui->checkBoxSpecifyIpAndPort->setGroupKey(settingsGroup, "specifyIpAndPort");
    ui->comboBoxServerAddress->setGroupKey(settingsGroup, "serverAddress");
    ui->spinBoxServerPort->setGroupKey(settingsGroup, "serverPort");
    ui->comboBoxMessageType->setGroupKey(settingsGroup, "messageType");
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
