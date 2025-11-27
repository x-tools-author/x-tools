/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttcommon.h"

#include <mongoose.h>

#include <QAbstractSocket>
#include <QNetworkInterface>

#include "utilities/compatibility.h"

namespace xMQTT {

void setupQosComboBox(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->addItem(QObject::tr("At most once (0)"), static_cast<int>(QoS::AtMostOnce));
    comboBox->addItem(QObject::tr("At least once (1)"), static_cast<int>(QoS::AtLeastOnce));
    comboBox->addItem(QObject::tr("Exactly once (2)"), static_cast<int>(QoS::ExactlyOnce));
    comboBox->setCurrentIndex(2);
}

void setupSocketAddress(QComboBox *cb)
{
    if (!cb) {
        return;
    }

    QStringList ipv4List, ipv6List;
    auto addresses = QNetworkInterface::allAddresses();
    for (auto &address : addresses) {
        auto str = address.toString();

        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ipv4List.append(str);
        } else if (address.protocol() == QAbstractSocket::IPv6Protocol) {
            ipv6List.append(str);
        }
    }

    cb->clear();
    cb->addItems(ipv4List);
#if 0
        cb->addItems(ipv6List);
#endif
}

void setupPortSpinBox(QSpinBox *spinBox, quint16 defaultPort)
{
    if (!spinBox) {
        return;
    }
    spinBox->setRange(1, 65535);
    spinBox->setValue(defaultPort);
}

void setupVersionComboBox(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->addItem(QString("MQTT 3.1.1"), 4);
    comboBox->addItem(QString("MQTT 5.0"), 5);
    comboBox->setCurrentIndex(0);
}

QString mgAddressToIpV4(const struct mg_addr *addr)
{
    if (addr == nullptr) {
        return QString("0.0.0.0");
    }

    char ipv4[4] = {0};
    memcpy(ipv4, addr->ip, 4);
    QString ip;
    for (int i = 0; i < 4; i++) {
        ip += QString::number(static_cast<uint8_t>(ipv4[i]));
        if (i < 3) {
            ip += ".";
        }
    }

    return ip;
}

bool ipV4ToMgAddress(const QString &ip, struct mg_addr *addr)
{
    if (addr == nullptr) {
        return false;
    }

    // split the ip address by '.'
    std::vector<std::string> tokens;
    QStringList parts = ip.split('.', xSkipEmptyParts);
    if (parts.size() != 4) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        addr->ip[i] = static_cast<uint8_t>(parts[i].toInt());
    }

    return true;
}

} // namespace xMQTT