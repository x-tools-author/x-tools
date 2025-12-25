/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "udpbroadcastui.h"
#include "ui_udpbroadcastui.h"

#include <QNetworkAddressEntry>
#include <QNetworkInterface>

#include "udpbroadcast.h"

UdpBroadcastUi::UdpBroadcastUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::UdpBroadcastUi)
{
    ui->setupUi(this);

    ui->comboBoxInterface->addItem(tr("All Interfaces"), QString("255.255.255.255"));
    // Get all available network interfaces QList<QNetworkInterface> interfaces
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : const_cast<QList<QNetworkInterface> &>(interfaces)) {
        if (!interface.isValid()) {
            continue;
        }

        if (interface.flags().testFlag(QNetworkInterface::CanBroadcast)) {
            auto entries = interface.addressEntries();
            for (const QNetworkAddressEntry &entry :
                 const_cast<QList<QNetworkAddressEntry> &>(entries)) {
                if (entry.broadcast().isNull()) {
                    continue;
                }

                QString address = entry.broadcast().toString();
                ui->comboBoxInterface->addItem(interface.humanReadableName(), address);
            }
        }
    }
}

UdpBroadcastUi::~UdpBroadcastUi()
{
    delete ui;
}

QVariantMap UdpBroadcastUi::save() const
{
    UdpBroadcastParameters params;
    params.address = ui->comboBoxInterface->currentData().toString();
    params.port = ui->spinBoxPort->value();
    return udpBroadcastParameters2variantMap(params);
}

void UdpBroadcastUi::load(const QVariantMap &parameters)
{
    UdpBroadcastParameters params = variantMap2UdpBroadcastParameters(parameters);

    int index = ui->comboBoxInterface->findData(params.address);
    ui->comboBoxInterface->setCurrentIndex(index == -1 ? 0 : index);
    ui->spinBoxPort->setValue(params.port);
}

Device *UdpBroadcastUi::newDevice()
{
    UdpBroadcast *device = new UdpBroadcast(this);
    return device;
}
