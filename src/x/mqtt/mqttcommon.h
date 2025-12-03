/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include <QComboBox>
#include <QMetaType>
#include <QSpinBox>

struct mg_addr;
namespace xMQTT {

enum class QoS { AtMostOnce = 0, AtLeastOnce = 1, ExactlyOnce = 2 };
void setupQosComboBox(QComboBox *comboBox);
void setupSocketAddress(QComboBox *cb);
void setupPortSpinBox(QSpinBox *spinBox, quint16 defaultPort = 1883);
enum class MqttVersion { V3_1_1 = 4, V5_0 = 5 };
void setupVersionComboBox(QComboBox *comboBox);

enum class MessageFormat { UTF8 = 0, Hex = 1 };
void setupMessageFormatComboBox(QComboBox *comboBox);
QByteArray cookedMessage(const QString &message, MessageFormat format);
QString uncookedMessage(const QByteArray &message, MessageFormat format);

QString mgAddressToIpV4(const struct mg_addr *addr);
bool ipV4ToMgAddress(const QString &ip, struct mg_addr *addr);

// -------------------------------------------------------------------------------------------------
struct MqttMessage
{
    bool isRx;
    uint8_t cmd;      // MQTT command, one of MQTT_CMD_*
    QByteArray dgram; // Whole MQTT packet, including headers
    QString serverAddress;
    quint16 serverPort{0};
    QString clientAddress;
    quint16 clientPort{0};
};

QString mqttCmd2String(uint8_t cmd);

} // namespace xMQTT

Q_DECLARE_METATYPE(xMQTT::MqttMessage)
Q_DECLARE_METATYPE(std::shared_ptr<xMQTT::MqttMessage>)
