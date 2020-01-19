/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>

#include "SAKGlobal.hh"
#include "SAKTcpServerDeviceController.hh"
#include "ui_SAKTcpServerDeviceController.h"
SAKTcpServerDeviceController::SAKTcpServerDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKTcpServerDeviceController)
{
    ui->setupUi(this);

    serverHostComboBox = ui->serverhostComboBox;
    serverPortLineEdit = ui->serverPortLineEdit;
    clientHostComboBox = ui->clientHostComboBox;

    refresh();
}

SAKTcpServerDeviceController::~SAKTcpServerDeviceController()
{
    delete ui;
}

QString SAKTcpServerDeviceController::serverHost()
{
    return serverHostComboBox->currentText();
}

quint16 SAKTcpServerDeviceController::serverPort()
{
    return static_cast<quint16>(serverPortLineEdit->text().toInt());
}

QString SAKTcpServerDeviceController::currentClientHost()
{
    QStringList host = clientHostComboBox->currentText().split(":");

    return host.first();
}

quint16 SAKTcpServerDeviceController::currentClientPort()
{
    QString port = clientHostComboBox->currentText().split(":").last();
    return static_cast<quint16>(port.toInt());
}

void SAKTcpServerDeviceController::refresh()
{
    SAKGlobal::initIpComboBox(serverHostComboBox);
}

void SAKTcpServerDeviceController::setUiEnable(bool enable)
{
    serverHostComboBox->setEnabled(enable);
    serverPortLineEdit->setEnabled(enable);
}

void SAKTcpServerDeviceController::addClient(QString host, quint16 port)
{
    QString item = host.append(":");
    item.append(QString::number(port));

    for(int i = 0; i < clientHostComboBox->count(); i++){
        if (clientHostComboBox->itemText(i).compare(item) == 0){
            return;
        }
    }

    clientHostComboBox->addItem(item);
}
