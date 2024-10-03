/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SocketUi.h"
#include "ui_SocketUi.h"

#include "../../IO/Communication/Socket.h"
#include "../../xIO.h"

namespace xTools {

SocketUi::SocketUi(xIO::CommunicationType type, QWidget *parent)
    : CommunicationUi(type, parent)
    , ui(new Ui::SocketUi)
{
    ui->setupUi(this);
    ui->spinBoxClientPort->setValue(55443);
    ui->spinBoxServerPort->setValue(34455);
    xIO::setupSocketAddress(ui->comboBoxClientIp);
    xIO::setupSocketAddress(ui->comboBoxServerIp);
    xIO::setupWebSocketDataChannel(ui->comboBoxChannel);

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
            &SocketUi::invokeDisconnectAll);

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    connect(ui->comboBoxChannel, &QComboBox::activated, this, [this]() {
#else
    connect(ui->comboBoxChannel, QOverload<int>::of(&QComboBox::activated), this, [this]() {
#endif
        if (this->m_socket) {
            this->m_socket->setDataChannel(ui->comboBoxChannel->currentIndex());
        }
    });
}

SocketUi::~SocketUi() {}

QVariantMap SocketUi::save() const
{
    QVariantMap parameters;
    parameters.insert("clientPort", ui->spinBoxClientPort->value());
    parameters.insert("clientAddress", ui->comboBoxClientIp->currentText());
    parameters.insert("serverPort", ui->spinBoxServerPort->value());
    parameters.insert("serverAddress", ui->comboBoxServerIp->currentText());
    parameters.insert("channel", ui->comboBoxChannel->currentIndex());
    parameters.insert("authentication", ui->checkBoxAuthentication->isChecked());
    parameters.insert("username", ui->lineEditUser->text());
    parameters.insert("password", ui->lineEditPassword->text());

    return parameters;
}

void SocketUi::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    ui->spinBoxClientPort->setValue(parameters.value("clientPort").toUInt());
    ui->comboBoxClientIp->setCurrentText(parameters.value("clientAddress").toString());
    ui->spinBoxServerPort->setValue(parameters.value("serverPort").toUInt());
    ui->comboBoxServerIp->setCurrentText(parameters.value("serverAddress").toString());
    ui->comboBoxChannel->setCurrentIndex(parameters.value("channel").toInt());
    ui->checkBoxAuthentication->setChecked(parameters.value("authentication").toBool());
    ui->lineEditUser->setText(parameters.value("username").toString());
    ui->lineEditPassword->setText(parameters.value("password").toString());
}

void SocketUi::setupIO(AbstractIO *io)
{
    m_socket = dynamic_cast<Socket *>(io);
}

void SocketUi::setClientWidgetsVisible(bool visible)
{
    ui->labelClientIp->setVisible(visible);
    ui->labelClientPort->setVisible(visible);
    ui->comboBoxClientIp->setVisible(visible);
    ui->spinBoxClientPort->setVisible(visible);
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

void SocketUi::setClientWidgetsEnabled(bool enabled)
{
    ui->labelClientIp->setEnabled(enabled);
    ui->labelClientPort->setEnabled(enabled);
    ui->comboBoxClientIp->setEnabled(enabled);
    ui->spinBoxClientPort->setEnabled(enabled);
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

void SocketUi::setupClients(const QStringList &clients)
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
} // namespace xTools
