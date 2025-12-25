/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpmulticastui.h"
#include "ui_udpmulticastui.h"

#include <QNetworkInterface>

#include "udpmulticast.h"

UdpMulticastUi::UdpMulticastUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::UdpMulticastUi)
{
    ui->setupUi(this);

    // Get all available network interfaces
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : const_cast<QList<QNetworkInterface> &>(interfaces)) {
        if (!interface.isValid()) {
            continue;
        }

        if (interface.flags().testFlag(QNetworkInterface::CanMulticast)) {
            ui->comboBoxInterface->addItem(interface.humanReadableName(), interface.name());
        }
    }
}

UdpMulticastUi::~UdpMulticastUi()
{
    delete ui;
}

QVariantMap UdpMulticastUi::save() const
{
    UdpMulticastParameters params;
    params.address = ui->lineEditMulticastIp->text();
    params.port = ui->spinBoxMulticastPort->value();
    params.name = ui->comboBoxInterface->currentData().toString();
    params.loopback = ui->checkBoxLoopback->isChecked();
    return udpMulticastParameters2variantMap(params);
}

void UdpMulticastUi::load(const QVariantMap &parameters)
{
    UdpMulticastParameters params = variantMap2UdpMulticastParameters(parameters);

    ui->lineEditMulticastIp->setText(params.address);
    ui->spinBoxMulticastPort->setValue(params.port);
    ui->checkBoxLoopback->setChecked(params.loopback);

    int index = ui->comboBoxInterface->findData(params.name);
    ui->comboBoxInterface->setCurrentIndex(index == -1 ? 0 : index);
}

Device *UdpMulticastUi::newDevice()
{
    UdpMulticast *device = new UdpMulticast(this);
    return device;
}
