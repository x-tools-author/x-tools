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
#include "SAKCommunicationTool.hh"
#include "SAKSocketServerToolUi.hh"
#include "ui_SAKSocketServerToolUi.h"

SAKSocketServerToolUi::SAKSocketServerToolUi(QWidget *parent)
    : SAKCommunicationToolUi{parent}
    , ui(new Ui::SAKSocketServerToolUi)
{
    ui->setupUi(this);
    connect(ui->comboBoxServerIp, &QComboBox::activated,
            this, &SAKSocketServerToolUi::onComboBoxServerIpActived);
    connect(ui->spinBoxServerPort, &QSpinBox::valueChanged,
            this, &SAKSocketServerToolUi::onSpinBoxServerPortValueChanged);
}

SAKSocketServerToolUi::~SAKSocketServerToolUi()
{
    delete ui;
}

void SAKSocketServerToolUi::setupCommunicationTool(SAKCommunicationTool *tool)
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
        ui->labelContext->setText(ipport);
    });
    connect(mTool, &SAKSocketServerTool::finished, this, [=](){
        ui->labelContext->setText(tr("Closed"));
    });
    connect(mTool, &SAKSocketServerTool::clientsChanged, this, [=](){
        QStringList clients = mTool->clients();
        ui->comboBoxClientList->clear();
        ui->comboBoxClientList->addItems(clients);
    });
}

void SAKSocketServerToolUi::updateUiState(bool isWorking)
{
    ui->comboBoxServerIp->setEnabled(!isWorking);
    ui->spinBoxServerPort->setEnabled(!isWorking);
    ui->checkBoxSpecifyIpAndPort->setEnabled(!isWorking);
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
