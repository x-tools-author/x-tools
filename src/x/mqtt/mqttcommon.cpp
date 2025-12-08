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
#if 0
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
#endif
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

void setupMessageFormatComboBox(QComboBox *comboBox)
{
    comboBox->clear();
    comboBox->addItem(QString("UTF-8"), static_cast<int>(MessageFormat::UTF8));
    comboBox->addItem(QString("Hex"), static_cast<int>(MessageFormat::Hex));
    comboBox->setCurrentIndex(0);
}

QByteArray cookedMessage(const QString &message, MessageFormat format)
{
    if (format == MessageFormat::UTF8) {
        return message.toUtf8();
    } else if (format == MessageFormat::Hex) {
        return QByteArray::fromHex(message.toUtf8());
    }

    return QByteArray();
}

QString uncookedMessage(const QByteArray &message, MessageFormat format)
{
    if (format == MessageFormat::UTF8) {
        return QString::fromUtf8(message);
    } else if (format == MessageFormat::Hex) {
        return QString::fromUtf8(message.toHex().toUpper());
    }

    return QString();
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

QString mqttCmd2String(uint8_t cmd)
{
    if (cmd == MQTT_CMD_PINGREQ) {
        return QStringLiteral("PINGREQ");
    } else if (cmd == MQTT_CMD_PINGRESP) {
        return QStringLiteral("PINGRESP");
    } else if (cmd == MQTT_CMD_CONNECT) {
        return QStringLiteral("CONNECT");
    } else if (cmd == MQTT_CMD_CONNACK) {
        return QStringLiteral("CONNACK");
    } else if (cmd == MQTT_CMD_PUBLISH) {
        return QStringLiteral("PUBLISH");
    } else if (cmd == MQTT_CMD_PUBACK) {
        return QStringLiteral("PUBACK");
    } else if (cmd == MQTT_CMD_PUBREC) {
        return QStringLiteral("PUBREC");
    } else if (cmd == MQTT_CMD_PUBREL) {
        return QStringLiteral("PUBREL");
    } else if (cmd == MQTT_CMD_PUBCOMP) {
        return QStringLiteral("PUBCOMP");
    } else if (cmd == MQTT_CMD_SUBSCRIBE) {
        return QStringLiteral("SUBSCRIBE");
    } else if (cmd == MQTT_CMD_SUBACK) {
        return QStringLiteral("SUBACK");
    } else if (cmd == MQTT_CMD_UNSUBSCRIBE) {
        return QStringLiteral("UNSUBSCRIBE");
    } else if (cmd == MQTT_CMD_UNSUBACK) {
        return QStringLiteral("UNSUBACK");
    } else if (cmd == MQTT_CMD_DISCONNECT) {
        return QStringLiteral("DISCONNECT");
    } else {
        return QStringLiteral("UNKNOWN");
    }
}

} // namespace xMQTT