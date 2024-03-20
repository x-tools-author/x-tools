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
    , ui(new Ui::SAKSocketServerToolUi)
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

void xToolsSocketServerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKSocketServerTool")) {
        qWarning() << "Invalid SAKSocketServerTool object,"
                                       " the operation will be ignored!";
        return;
    }

    if (!tool->inherits("SAKWebSocketServerTool")) {
        ui->labelMessageType->hide();
        ui->comboBoxMessageType->hide();
    }

    if (!tool->inherits("SAKUdpServerTool")) {
        ui->labelCntext->hide();
        ui->labelBindingInfo->hide();
        ui->checkBoxSpecifyIpAndPort->hide();
    }

    mTool = qobject_cast<xToolsSocketServerTool *>(tool);
    if (!mTool) {
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
    mTool->setServerIp(ip);
    ;
    mTool->setServerPort(port);
    mTool->setSpecifyIpAndPort(specified);
    mTool->setMessageType(messageType);

    connect(mTool, &xToolsSocketServerTool::bindingIpPortChanged, this, [=]() {
        QString ipport = mTool->bindingIpPort();
        ui->labelBindingInfo->setText(ipport);
    });
    connect(mTool, &xToolsSocketServerTool::finished, this, [=]() {
        ui->labelBindingInfo->setText(tr("Closed"));
    });
    connect(mTool, &xToolsSocketServerTool::clientsChanged, this, [=]() {
        int index = ui->comboBoxClientList->currentIndex();
        QString first = ui->comboBoxClientList->itemText(0);
        QStringList clients = mTool->clients();
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
    if (mTool) {
        mTool->setServerIp(ui->comboBoxServerIp->currentText().trimmed());
    }
}

void xToolsSocketServerToolUi::onSpinBoxServerPortValueChanged(int value)
{
    if (mTool) {
        mTool->setServerPort(value);
    }
}

void xToolsSocketServerToolUi::onComboBoxClientsIndexChanged()
{
    int index = ui->comboBoxClientList->currentIndex();
    mTool->setClientIndex(index - 1);
}

void xToolsSocketServerToolUi::onComboBoxMessageTypeIndexChanged()
{
    int messageType = ui->comboBoxMessageType->currentData().toInt();
    mTool->setMessageType(messageType);
}

void xToolsSocketServerToolUi::onCheckBoxSpecifyIpAndPortClicked()
{
    bool specified = ui->checkBoxSpecifyIpAndPort->isChecked();
    mTool->setSpecifyIpAndPort(specified);
}
