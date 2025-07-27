/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "socketui.h"
#include "ui_socketui.h"

#include "common/xtools.h"
#include "device/socket.h"

SocketUi::SocketUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::SocketUi)
{
    ui->setupUi(this);
    ui->spinBoxServerPort->setValue(34455);
    setupSocketAddress(ui->comboBoxServerIp);
    setupWebSocketDataChannel(ui->comboBoxChannel);

    setupClients(QStringList());
    connect(ui->comboBoxWriteTo, xComboBoxActivated, this, [this]() {
        QString const flag = ui->comboBoxWriteTo->currentData().toString();
        emit currentClientChanged(flag);
    });
    connect(ui->toolButtonDisconnectAllClient,
            &QToolButton::clicked,
            this,
            &SocketUi::invokeDisconnectAll);
    connect(ui->comboBoxChannel, xComboBoxActivated, this, [this]() {
        if (this->m_socket) {
            this->m_socket->setDataChannel(ui->comboBoxChannel->currentIndex());
        }
    });

    setPathWidgetsVisible(false);
}

SocketUi::~SocketUi() {}

QVariantMap SocketUi::save() const
{
    SocketItem item;
    item.serverPort = ui->spinBoxServerPort->value();
    item.serverAddress = ui->comboBoxServerIp->currentText();
    item.dataChannel = static_cast<WebSocketDataChannel>(ui->comboBoxChannel->currentIndex());
    item.authentication = ui->checkBoxAuthentication->isChecked();
    item.username = ui->lineEditUser->text();
    item.password = ui->lineEditPassword->text();
    item.multicastAddress = ui->lineEditMulticastIp->text();
    item.multicastPort = ui->spinBoxMulticastPort->value();
    item.enableMulticast = ui->checkBoxEnableMulticast->isChecked();
    item.justMulticast = ui->checkBoxJustMulticast->isChecked();
    item.path = ui->lineEditPath->text();

    return saveSocketItem(item);
}

void SocketUi::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    SocketItemKeys keys;
    SocketItem item = loadSocketItem(parameters);
    ui->spinBoxServerPort->setValue(item.serverPort);
    ui->comboBoxServerIp->setCurrentText(item.serverAddress);
    ui->comboBoxChannel->setCurrentIndex(static_cast<int>(item.dataChannel));
    ui->checkBoxAuthentication->setChecked(item.authentication);
    ui->lineEditUser->setText(item.username);
    ui->lineEditPassword->setText(item.password);
    ui->lineEditMulticastIp->setText(item.multicastAddress);
    ui->spinBoxMulticastPort->setValue(item.multicastPort);
    ui->checkBoxEnableMulticast->setChecked(item.enableMulticast);
    ui->checkBoxJustMulticast->setChecked(item.justMulticast);
    ui->lineEditPath->setText(item.path);
}

void SocketUi::setServerWidgetsVisible(bool visible)
{
    ui->labelServerIp->setVisible(visible);
    ui->labelServerPort->setVisible(visible);
    ui->comboBoxServerIp->setVisible(visible);
    ui->spinBoxServerPort->setVisible(visible);
}

void SocketUi::setChannelWidgetsVisible(bool visible)
{
    ui->labelChannel->setVisible(visible);
    ui->comboBoxChannel->setVisible(visible);
}

void SocketUi::setAuthenticationWidgetsVisible(bool visible)
{
    ui->checkBoxAuthentication->setVisible(visible);
    ui->labelUser->setVisible(visible);
    ui->labelPassword->setVisible(visible);
    ui->lineEditUser->setVisible(visible);
    ui->lineEditPassword->setVisible(visible);
}

void SocketUi::setWriteToWidgetsVisible(bool visible)
{
    ui->labelWriteTo->setVisible(visible);
    ui->comboBoxWriteTo->setVisible(visible);
    ui->toolButtonDisconnectAllClient->setVisible(visible);
}

void SocketUi::setMulticastWidgetsVisible(bool visible)
{
    ui->labelMulticastIp->setVisible(visible);
    ui->lineEditMulticastIp->setVisible(visible);
    ui->labelMulticastPort->setVisible(visible);
    ui->spinBoxMulticastPort->setVisible(visible);
    ui->checkBoxEnableMulticast->setVisible(visible);
    ui->checkBoxJustMulticast->setVisible(visible);
}

void SocketUi::setPathWidgetsVisible(bool visible)
{
    ui->labelPath->setVisible(visible);
    ui->lineEditPath->setVisible(visible);
}

void SocketUi::setServerWidgetsEnabled(bool enabled)
{
    ui->labelServerIp->setEnabled(enabled);
    ui->labelServerPort->setEnabled(enabled);
    ui->comboBoxServerIp->setEnabled(enabled);
    ui->spinBoxServerPort->setEnabled(enabled);
}

void SocketUi::setChannelWidgetsEnabled(bool enabled)
{
    ui->labelChannel->setEnabled(enabled);
    ui->comboBoxChannel->setEnabled(enabled);

    ui->labelPath->setEnabled(enabled);
    ui->lineEditPath->setEnabled(enabled);
}

void SocketUi::setAuthenticationWidgetsEnabled(bool enabled)
{
    ui->checkBoxAuthentication->setEnabled(enabled);
    ui->labelUser->setEnabled(enabled);
    ui->labelPassword->setEnabled(enabled);
    ui->lineEditUser->setEnabled(enabled);
    ui->lineEditPassword->setEnabled(enabled);
}

void SocketUi::setWriteToWidgetsEnabled(bool enabled)
{
    ui->labelWriteTo->setEnabled(enabled);
    ui->comboBoxWriteTo->setEnabled(enabled);
    ui->toolButtonDisconnectAllClient->setEnabled(enabled);
}

void SocketUi::setMulticastWidgetsEnabled(bool enabled)
{
    ui->labelMulticastIp->setEnabled(enabled);
    ui->lineEditMulticastIp->setEnabled(enabled);
    ui->labelMulticastPort->setEnabled(enabled);
    ui->spinBoxMulticastPort->setEnabled(enabled);
    ui->checkBoxEnableMulticast->setEnabled(enabled);
    ui->checkBoxJustMulticast->setEnabled(enabled);
}

void SocketUi::setPathWidgetsEnabled(bool enabled)
{
    ui->labelPath->setEnabled(enabled);
    ui->lineEditPath->setEnabled(enabled);
}

void SocketUi::setupClients(const QStringList &clients)
{
    QString current = ui->comboBoxWriteTo->currentData().toString();
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
