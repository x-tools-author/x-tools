/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketServerToolUi.h"
#include "ui_xToolsSocketServerToolUi.h"

#include "xToolsSocketServerTool.h"
#include <QMessageBox>

xToolsSocketServerToolUi::xToolsSocketServerToolUi(QWidget *parent)
    : xToolsCommunicationToolUi{parent}
    , ui(new Ui::xToolsSocketServerToolUi)
{
    ui->setupUi(this);
    connect(ui->comboBoxServerIp,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this,
            &xToolsSocketServerToolUi::onComboBoxServerIpActived);
    connect(ui->spinBoxServerPort,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsSocketServerToolUi::onSpinBoxServerPortValueChanged);
    connect(ui->comboBoxClientList,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSocketServerToolUi::onComboBoxClientsIndexChanged);
    connect(ui->comboBoxMessageType,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsSocketServerToolUi::onComboBoxMessageTypeIndexChanged);
    connect(ui->checkBoxSpecifyIpAndPort,
            &QCheckBox::clicked,
            this,
            &xToolsSocketServerToolUi::onCheckBoxSpecifyIpAndPortClicked);
    ui->pushButtonClear->hide();
}

xToolsSocketServerToolUi::~xToolsSocketServerToolUi()
{
    delete ui;
}

void xToolsSocketServerToolUi::onIsWorkingChanged(bool isWorking)
{
    ui->comboBoxServerIp->setEnabled(!isWorking);
    ui->spinBoxServerPort->setEnabled(!isWorking);
    ui->checkBoxSpecifyIpAndPort->setEnabled(!isWorking);
}

void xToolsSocketServerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool,
                                                       const QString &settingsGroup)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("xToolsSocketServerTool")) {
        qWarning() << "Invalid xToolsSocketServerTool object, the operation will be ignored!";
        return;
    }

    if (!tool->inherits("xToolsWebSocketServerTool")) {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }

    if (!tool->inherits("xToolsUdpServerTool")) {
        ui->labelCntext->hide();
        ui->labelBindingInfo->hide();
        ui->checkBoxSpecifyIpAndPort->hide();
    }

    m_tool = qobject_cast<xToolsSocketServerTool *>(tool);
    if (!m_tool) {
        qWarning() << "qobject_cast<>() return nullptr";
        return;
    }

    ui->comboBoxServerIp->setGroupKey(settingsGroup, "serverIp");
    ui->spinBoxServerPort->setGroupKey(settingsGroup, "port");
    ui->comboBoxMessageType->setGroupKey(settingsGroup, "messageType");
    ui->checkBoxSpecifyIpAndPort->setGroupKey(settingsGroup, "specifiedIpAndPort");

    QString ip = ui->comboBoxServerIp->currentText();
    int port = ui->spinBoxServerPort->value();
    bool specified = ui->checkBoxSpecifyIpAndPort->isChecked();
    int messageType = ui->comboBoxMessageType->currentData().toInt();
    m_tool->setServerIp(ip);
    ;
    m_tool->setServerPort(port);
    m_tool->setSpecifyIpAndPort(specified);
    m_tool->setMessageType(messageType);

    connect(m_tool, &xToolsSocketServerTool::bindingIpPortChanged, this, [=]() {
        QString ipport = m_tool->bindingIpPort();
        ui->labelBindingInfo->setText(ipport);
    });
    connect(m_tool, &xToolsSocketServerTool::finished, this, [=]() {
        ui->labelBindingInfo->setText(tr("Closed"));
    });
    connect(m_tool, &xToolsSocketServerTool::clientsChanged, this, [=]() {
        int index = ui->comboBoxClientList->currentIndex();
        QString first = ui->comboBoxClientList->itemText(0);
        QStringList clients = m_tool->clients();
        int count = ui->comboBoxClientList->count();

        ui->comboBoxClientList->clear();
        ui->comboBoxClientList->addItem(first);
        ui->comboBoxClientList->addItems(clients);
        if ((index >= 0) && (index < count)) {
            ui->comboBoxClientList->setCurrentIndex(index);
        }
    });
}

void xToolsSocketServerToolUi::onComboBoxServerIpActived()
{
    if (m_tool) {
        m_tool->setServerIp(ui->comboBoxServerIp->currentText().trimmed());
    }
}

void xToolsSocketServerToolUi::onSpinBoxServerPortValueChanged(int value)
{
    if (m_tool) {
        m_tool->setServerPort(value);
    }
}

void xToolsSocketServerToolUi::onComboBoxClientsIndexChanged()
{
    int index = ui->comboBoxClientList->currentIndex();
    m_tool->setClientIndex(index - 1);
}

void xToolsSocketServerToolUi::onComboBoxMessageTypeIndexChanged()
{
    int messageType = ui->comboBoxMessageType->currentData().toInt();
    m_tool->setMessageType(messageType);
}

void xToolsSocketServerToolUi::onCheckBoxSpecifyIpAndPortClicked()
{
    bool specified = ui->checkBoxSpecifyIpAndPort->isChecked();
    m_tool->setSpecifyIpAndPort(specified);
}
