/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapcommon.h"

#include <coap3/coap.h>

#include "common/xtools.h"

namespace xCoAP {

CoAPCommon::CoAPCommon(QObject* parent)
    : QObject(parent)
{}

CoAPCommon::~CoAPCommon() {}

void CoAPCommon::setupSocketAddress(QComboBox* comboBox)
{
    xSetupSocketAddress(comboBox);
}

void CoAPCommon::setupSocketPort(QSpinBox* spinBox)
{
    xSetupSocketPort(spinBox);

    if (spinBox) {
        spinBox->setValue(433);
    }
}

void CoAPCommon::setupSocketProtocol(QComboBox* comboBox)
{
    if (!comboBox) {
        return;
    }

    /*
        COAP_PROTO_NONE = 0,
        COAP_PROTO_UDP,
        COAP_PROTO_DTLS,
        COAP_PROTO_TCP,
        COAP_PROTO_TLS,
        COAP_PROTO_WS,
        COAP_PROTO_WSS,
        COAP_PROTO_LAST
    */

    comboBox->clear();
    comboBox->addItem("UDP", static_cast<int>(COAP_PROTO_UDP));
    comboBox->addItem("DTLS", static_cast<int>(COAP_PROTO_DTLS));
    comboBox->addItem("TCP", static_cast<int>(COAP_PROTO_TCP));
    comboBox->addItem("TLS", static_cast<int>(COAP_PROTO_TLS));
    comboBox->addItem("WS", static_cast<int>(COAP_PROTO_WS));
    comboBox->addItem("WSS", static_cast<int>(COAP_PROTO_WSS));
}

bool CoAPCommon::isValidProtocol(int protocol)
{
    switch (protocol) {
    case COAP_PROTO_UDP:
    case COAP_PROTO_DTLS:
    case COAP_PROTO_TCP:
    case COAP_PROTO_TLS:
    case COAP_PROTO_WS:
    case COAP_PROTO_WSS:
        return true;
    default:
        return false;
    }
}

QJsonObject CoAPCommon::clientParameters2JsonObject(const ClientParameters& params)
{
    QJsonObject obj;
    obj[ClientParameterKeys().serverAddress] = params.serverAddress;
    obj[ClientParameterKeys().serverPort] = static_cast<int>(params.serverPort);
    obj[ClientParameterKeys().protocol] = params.protocol;
    return obj;
}

CoAPCommon::ClientParameters CoAPCommon::jsonObject2ClientParameters(const QJsonObject& obj)
{
    ClientParameters params;
    params.serverAddress = obj.value(ClientParameterKeys().serverAddress).toString("localhost");
    params.serverPort = static_cast<quint16>(obj.value(ClientParameterKeys().serverPort).toInt(433));
    params.protocol = obj.value(ClientParameterKeys().protocol)
                          .toInt(static_cast<int>(COAP_PROTO_UDP));
    return params;
}

QJsonObject CoAPCommon::serverParameters2JsonObject(const ServerParameters& params)
{
    QJsonObject obj;
    ServerParameterKeys keys;
    obj[keys.serverAddress] = params.serverAddress;
    obj[keys.serverPort] = static_cast<int>(params.serverPort);
    obj[keys.protocol] = params.protocol;
    return obj;
}

CoAPCommon::ServerParameters CoAPCommon::jsonObject2ServerParameters(const QJsonObject& obj)
{
    ServerParameters params;
    ServerParameterKeys keys;
    params.serverAddress = obj.value(keys.serverAddress).toString("localhost");
    params.serverPort = static_cast<quint16>(obj.value(keys.serverPort).toInt(433));
    params.protocol = obj.value(keys.protocol).toInt(static_cast<int>(COAP_PROTO_UDP));
    return params;
}

} // namespace xCoAP
