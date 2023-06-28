/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

#include "SAKIpComboBox.hh"

SAKIpComboBox::SAKIpComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    auto addresses = QNetworkInterface::allAddresses();
    for (auto &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            addItem(address.toString());
        }
    }
}
