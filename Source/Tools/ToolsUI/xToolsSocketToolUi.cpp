/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketToolUi.h"
#include "ui_xToolsSocketToolUi.h"

#include "xToolsSocketTool.h"

xToolsSocketToolUi::xToolsSocketToolUi(QWidget *parent)
    : xToolsCommunicationToolUi(parent)
    , ui(new Ui::xToolsSocketToolUi)
{
    ui->setupUi(this);
    ui->spinBoxClientPort->setValue(55443);
    ui->spinBoxServerPort->setValue(34455);
#if 0
    setupIp(ui->comboBoxClientIp);
    setupIp(ui->comboBoxServerIp);
    setupWebSocketDataChannel(ui->comboBoxChannel);
#endif

    setupClients(QStringList());
#if QT_VERSION>= QT_VERSION_CHECK(6,5,0)
    connect(ui->comboBoxWriteTo, &QComboBox::activated, this, [this]() {
#else
    connect(ui->comboBoxWriteTo, QOverload<int>::of(&QComboBox::activated), this, [this]() {
#endif
        QString const flag = ui->comboBoxWriteTo->currentData().toString();
        emit currentClientChanged(flag);
    });
    connect(ui->toolButtonDisconnectAllClient,
            &QToolButton::clicked,
            this,
            &xToolsSocketToolUi::invokeDisconnectAll);
}

xToolsSocketToolUi::~xToolsSocketToolUi() {}

QVariantMap xToolsSocketToolUi::save() const
{
    xToolsSocketTool::ParameterKeysContext keys;
    QVariantMap parameters;
    parameters.insert(keys.clientPort, ui->spinBoxClientPort->value());
    parameters.insert(keys.clientAddress, ui->comboBoxClientIp->currentText());
    parameters.insert(keys.serverPort, ui->spinBoxServerPort->value());
    parameters.insert(keys.serverAddress, ui->comboBoxServerIp->currentText());
    parameters.insert(keys.channel, ui->comboBoxChannel->currentIndex());
    parameters.insert(keys.authentication, ui->checkBoxAuthentication->isChecked());
    parameters.insert(keys.username, ui->lineEditUser->text());
    parameters.insert(keys.password, ui->lineEditPassword->text());

    return parameters;
}

void xToolsSocketToolUi::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    xToolsSocketTool::ParameterKeysContext keys;
    ui->spinBoxClientPort->setValue(parameters.value(keys.clientPort).toUInt());
    ui->comboBoxClientIp->setCurrentText(parameters.value(keys.clientAddress).toString());
    ui->spinBoxServerPort->setValue(parameters.value(keys.serverPort).toUInt());
    ui->comboBoxServerIp->setCurrentText(parameters.value(keys.serverAddress).toString());
    ui->comboBoxChannel->setCurrentIndex(parameters.value(keys.channel).toInt());
    ui->checkBoxAuthentication->setChecked(parameters.value(keys.authentication).toBool());
    ui->lineEditUser->setText(parameters.value(keys.username).toString());
    ui->lineEditPassword->setText(parameters.value(keys.password).toString());
}

void xToolsSocketToolUi::setClientWidgetsVisible(bool visible)
{
    ui->labelClientIp->setVisible(visible);
    ui->labelClientPort->setVisible(visible);
    ui->comboBoxClientIp->setVisible(visible);
    ui->spinBoxClientPort->setVisible(visible);
}

void xToolsSocketToolUi::setServerWidgetsVisible(bool visible)
{
    ui->labelServerIp->setVisible(visible);
    ui->labelServerPort->setVisible(visible);
    ui->comboBoxServerIp->setVisible(visible);
    ui->spinBoxServerPort->setVisible(visible);
}

void xToolsSocketToolUi::setChannelWidgetsVisible(bool visible)
{
    ui->labelChannel->setVisible(visible);
    ui->comboBoxChannel->setVisible(visible);
}

void xToolsSocketToolUi::setAuthenticationWidgetsVisible(bool visible)
{
    ui->checkBoxAuthentication->setVisible(visible);
    ui->labelUser->setVisible(visible);
    ui->labelPassword->setVisible(visible);
    ui->lineEditUser->setVisible(visible);
    ui->lineEditPassword->setVisible(visible);
}

void xToolsSocketToolUi::setWriteToWidgetsVisible(bool visible)
{
    ui->labelWriteTo->setVisible(visible);
    ui->comboBoxWriteTo->setVisible(visible);
    ui->toolButtonDisconnectAllClient->setVisible(visible);
}

void xToolsSocketToolUi::setClientWidgetsEnabled(bool enabled)
{
    ui->labelClientIp->setEnabled(enabled);
    ui->labelClientPort->setEnabled(enabled);
    ui->comboBoxClientIp->setEnabled(enabled);
    ui->spinBoxClientPort->setEnabled(enabled);
}

void xToolsSocketToolUi::setServerWidgetsEnabled(bool enabled)
{
    ui->labelServerIp->setEnabled(enabled);
    ui->labelServerPort->setEnabled(enabled);
    ui->comboBoxServerIp->setEnabled(enabled);
    ui->spinBoxServerPort->setEnabled(enabled);
}

void xToolsSocketToolUi::setChannelWidgetsEnabled(bool enabled)
{
    ui->labelChannel->setEnabled(enabled);
    ui->comboBoxChannel->setEnabled(enabled);
}

void xToolsSocketToolUi::setAuthenticationWidgetsEnabled(bool enabled)
{
    ui->checkBoxAuthentication->setEnabled(enabled);
    ui->labelUser->setEnabled(enabled);
    ui->labelPassword->setEnabled(enabled);
    ui->lineEditUser->setEnabled(enabled);
    ui->lineEditPassword->setEnabled(enabled);
}

void xToolsSocketToolUi::setWriteToWidgetsEnabled(bool enabled)
{
    ui->labelWriteTo->setEnabled(enabled);
    ui->comboBoxWriteTo->setEnabled(enabled);
    ui->toolButtonDisconnectAllClient->setEnabled(enabled);
}

void xToolsSocketToolUi::setupClients(const QStringList &clients)
{
    QString current = ui->comboBoxClientIp->currentData().toString();
    ui->comboBoxWriteTo->clear();
    ui->comboBoxWriteTo->addItem(tr("All clients"), QString(""));

    for (const QString &client : clients) {
        ui->comboBoxWriteTo->addItem(client, client);
    }

    int index = ui->comboBoxWriteTo->findData(current);
    if (index == -1) {
        ui->comboBoxWriteTo->setCurrentIndex(0);
    } else {
        ui->comboBoxWriteTo->setCurrentIndex(index);
    }
}
