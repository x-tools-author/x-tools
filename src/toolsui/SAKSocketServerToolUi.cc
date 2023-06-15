/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSocketServerTool.hh"
#include "SAKSocketServerToolUi.hh"
#include "ui_SAKSocketServerToolUi.h"

SAKSocketServerToolUi::SAKSocketServerToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSocketServerToolUi)
{
    ui->setupUi(this);
    connect(ui->comboBoxServerIp,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, &SAKSocketServerToolUi::onComboBoxServerIpActived);
    connect(ui->spinBoxServerPort,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &SAKSocketServerToolUi::onSpinBoxServerPortValueChanged);
    connect(ui->comboBoxClientList,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SAKSocketServerToolUi::onComboBoxClientsIndexChanged);
    connect(ui->comboBoxMessageType,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SAKSocketServerToolUi::onComboBoxMessageTypeIndexChanged);
}

SAKSocketServerToolUi::~SAKSocketServerToolUi()
{
    delete ui;
}

void SAKSocketServerToolUi::onIsWorkingChanged(bool isWorking)
{
    ui->comboBoxServerIp->setEnabled(!isWorking);
    ui->spinBoxServerPort->setEnabled(!isWorking);
    ui->checkBoxSpecifyIpAndPort->setEnabled(!isWorking);
}

void SAKSocketServerToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingsGroup)
{
    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKSocketServerTool")) {
        qCWarning(mLoggingCategory) << "Invalid SAKSocketServerTool object,"
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

    mTool = qobject_cast<SAKSocketServerTool*>(tool);
    if (!mTool) {
        qCWarning(mLoggingCategory) << "qobject_cast<>() return nullptr";
        return;
    }

    QString ip = ui->comboBoxServerIp->currentText();
    int port = ui->spinBoxServerPort->value();
    bool specified = ui->checkBoxSpecifyIpAndPort->isChecked();
    int messageType = ui->comboBoxMessageType->currentData().toInt();
    mTool->setServerIp(ip);;
    mTool->setServerPort(port);
    mTool->setSpecifyIpAndPort(specified);
    mTool->setMessageType(messageType);

    connect(mTool, &SAKSocketServerTool::bindingIpPortChanged, this, [=](){
        QString ipport = mTool->bindingIpPort();
        ui->labelBindingInfo->setText(ipport);
    });
    connect(mTool, &SAKSocketServerTool::finished, this, [=](){
        ui->labelBindingInfo->setText(tr("Closed"));
    });
    connect(mTool, &SAKSocketServerTool::clientsChanged, this, [=](){
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

    ui->comboBoxServerIp->setGroupKey(settingsGroup, "serverIp");
    ui->spinBoxServerPort->setGroupKey(settingsGroup, "port");
    ui->comboBoxMessageType->setGroupKey(settingsGroup, "messageType");
}

void SAKSocketServerToolUi::onComboBoxServerIpActived()
{
    if (mTool) {
        mTool->setServerIp(ui->comboBoxServerIp->currentText().trimmed());
    }
}

void SAKSocketServerToolUi::onSpinBoxServerPortValueChanged(int value)
{
    if (mTool) {
        mTool->setServerPort(value);
    }
}

void SAKSocketServerToolUi::onComboBoxClientsIndexChanged()
{
    int index = ui->comboBoxClientList->currentIndex();
    mTool->setClientIndex(index - 1);
}

void SAKSocketServerToolUi::onComboBoxMessageTypeIndexChanged()
{
    int messageType = ui->comboBoxMessageType->currentData().toInt();
    mTool->setMessageType(messageType);
}
