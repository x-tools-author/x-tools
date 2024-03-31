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

#include <QDebug>
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
    connect(ui->checkBoxAuthentication,
            &QCheckBox::clicked,
            this,
            &xToolsSocketClientToolUi::onAuthenticationCheckBoxClicked);
    connect(ui->lineEditUserName,
            &QLineEdit::textChanged,
            this,
            &xToolsSocketClientToolUi::onUserNameLineEditTextChanged);

    connect(ui->lineEditPassword,
            &QLineEdit::textChanged,
            this,
            &xToolsSocketClientToolUi::onPasswordLineEditTextChanged);
}

xToolsSocketClientToolUi::~xToolsSocketClientToolUi()
{
    delete ui;
}

void xToolsSocketClientToolUi::onIsWorkingChanged(bool isWorking)
{
    ui->comboBoxClientAddress->setEnabled(!isWorking);
    ui->spinBoxClientPort->setEnabled(!isWorking);
}

void xToolsSocketClientToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                       const QString &settingsGroup)
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
        ui->widgetAuthentication->hide();
    }

    m_tool = qobject_cast<xToolsSocketClientTool *>(tool);
    if (!m_tool) {
        Q_ASSERT_X(m_tool, __FUNCTION__, "Invalid xToolsSocketClientTool object!");
        return;
    }

    ui->comboBoxClientAddress->setGroupKey(settingsGroup, "clientAddress");
    ui->spinBoxClientPort->setGroupKey(settingsGroup, "clientPort");
    ui->comboBoxServerAddress->setGroupKey(settingsGroup, "serverAddress");
    ui->spinBoxServerPort->setGroupKey(settingsGroup, "serverPort");
    ui->comboBoxMessageType->setGroupKey(settingsGroup, "messageType");
    ui->checkBoxAuthentication->setGroupKey(settingsGroup, "authentication");
    ui->lineEditUserName->setGroupKey(settingsGroup, "username");
    ui->lineEditPassword->setGroupKey(settingsGroup, "password");

    m_tool->setClientIp(ui->comboBoxClientAddress->currentText().trimmed());
    m_tool->setClientPort(ui->spinBoxClientPort->value());
    m_tool->setServerIp(ui->comboBoxServerAddress->currentText().trimmed());
    m_tool->setServerPort(ui->spinBoxServerPort->value());
}

void xToolsSocketClientToolUi::onComboBoxClientAddressActivated()
{
    if (m_tool) {
        QString ip = ui->comboBoxClientAddress->currentText().trimmed();
        m_tool->setClientIp(ip);
    }
}

void xToolsSocketClientToolUi::onSpinBoxClientPortValueChanged(int value)
{
    if (m_tool) {
        m_tool->setClientPort(value);
    }
}

void xToolsSocketClientToolUi::onComboBoxServerAddressCurrentTextChanged()
{
    if (m_tool) {
        QString ip = ui->comboBoxServerAddress->currentText().trimmed();
        m_tool->setServerIp(ip);
    }
}

void xToolsSocketClientToolUi::onSpinBoxServerPortValueChanged(int value)
{
    if (m_tool) {
        m_tool->setServerPort(value);
    }
}

void xToolsSocketClientToolUi::onAuthenticationCheckBoxClicked()
{
    if (m_tool) {
        bool checked = ui->checkBoxAuthentication->isChecked();
        m_tool->setAuthentication(checked);
    }
}

void xToolsSocketClientToolUi::onUserNameLineEditTextChanged(const QString &text)
{
    if (m_tool) {
        m_tool->setUserName(text);
    }
}

void xToolsSocketClientToolUi::onPasswordLineEditTextChanged(const QString &text)
{
    if (m_tool) {
        m_tool->setPassword(text);
    }
}
