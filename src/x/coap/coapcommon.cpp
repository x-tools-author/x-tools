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
#include <coap3/coap_session_internal.h>

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

static void addContextFormatItem(const QString& key, int value, QComboBox* cb)
{
    QString txt = QString::number(value) + QString("-") + key;
    cb->addItem(txt, value);
    cb->setItemData(cb->count() - 1, key, Qt::ToolTipRole);
};

void CoAPCommon::setupContextFormat(QComboBox* comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();

    // clang-format off
    addContextFormatItem("application/json", COAP_MEDIATYPE_APPLICATION_JSON, comboBox);
    addContextFormatItem("text/plain", COAP_MEDIATYPE_TEXT_PLAIN, comboBox);
    addContextFormatItem("application/octet-stream", COAP_MEDIATYPE_APPLICATION_OCTET_STREAM, comboBox);
    addContextFormatItem("application/link-format", COAP_MEDIATYPE_APPLICATION_LINK_FORMAT, comboBox);
    addContextFormatItem("application/xml", COAP_MEDIATYPE_APPLICATION_XML, comboBox);
    addContextFormatItem("application/rdf+xml", COAP_MEDIATYPE_APPLICATION_RDF_XML, comboBox);
    addContextFormatItem("application/exi", COAP_MEDIATYPE_APPLICATION_EXI, comboBox);
    addContextFormatItem("application/cbor", COAP_MEDIATYPE_APPLICATION_CBOR, comboBox);
    addContextFormatItem("application/cwt", COAP_MEDIATYPE_APPLICATION_CWT, comboBox);
    addContextFormatItem("application/coap-group+json", COAP_MEDIATYPE_APPLICATION_COAP_GROUP_JSON, comboBox);
    addContextFormatItem("application/cose; cose-type=\"cose-sign\"", COAP_MEDIATYPE_APPLICATION_COSE_SIGN, comboBox);
    addContextFormatItem("application/cose; cose-type=\"cose-sign1\"", COAP_MEDIATYPE_APPLICATION_COSE_SIGN1, comboBox);
    addContextFormatItem("application/cose; cose-type=\"cose-encrypt\"", COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT, comboBox);
    addContextFormatItem("application/cose; cose-type=\"cose-encrypt0\"", COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT0, comboBox);
    addContextFormatItem("application/cose; cose-type=\"cose-mac\"", COAP_MEDIATYPE_APPLICATION_COSE_MAC, comboBox);
    addContextFormatItem("application/cose; cose-type=\"cose-mac0\"", COAP_MEDIATYPE_APPLICATION_COSE_MAC0, comboBox);
    addContextFormatItem("application/cose-key", COAP_MEDIATYPE_APPLICATION_COSE_KEY, comboBox);
    addContextFormatItem("application/cose-key-set", COAP_MEDIATYPE_APPLICATION_COSE_KEY_SET, comboBox);
    addContextFormatItem("application/senml+json", COAP_MEDIATYPE_APPLICATION_SENML_JSON, comboBox);
    addContextFormatItem("application/sensml+json", COAP_MEDIATYPE_APPLICATION_SENSML_JSON, comboBox);
    addContextFormatItem("application/senml+cbor", COAP_MEDIATYPE_APPLICATION_SENML_CBOR, comboBox);
    addContextFormatItem("application/sensml+cbor", COAP_MEDIATYPE_APPLICATION_SENSML_CBOR, comboBox);
    addContextFormatItem("application/senml-exi", COAP_MEDIATYPE_APPLICATION_SENML_EXI, comboBox);
    addContextFormatItem("application/sensml-exi", COAP_MEDIATYPE_APPLICATION_SENSML_EXI, comboBox);
    addContextFormatItem("application/senml+xml", COAP_MEDIATYPE_APPLICATION_SENML_XML, comboBox);
    addContextFormatItem("application/sensml+xml", COAP_MEDIATYPE_APPLICATION_SENSML_XML, comboBox);
    addContextFormatItem("application/dots+cbor", COAP_MEDIATYPE_APPLICATION_DOTS_CBOR, comboBox);
    addContextFormatItem("application/ace+cbor", COAP_MEDIATYPE_APPLICATION_ACE_CBOR, comboBox);
    addContextFormatItem("application/missing-blocks+cbor-seq", COAP_MEDIATYPE_APPLICATION_MB_CBOR_SEQ, comboBox);
    addContextFormatItem("application/oscore", COAP_MEDIATYPE_APPLICATION_OSCORE, comboBox);
    // clang-format on

    int index = comboBox->findData(COAP_MEDIATYPE_APPLICATION_JSON);
    if (index != -1) {
        comboBox->setCurrentIndex(index);
    }
}

QString getCoAPAddress(const coap_address_t* addr)
{
    char addrStr[INET6_ADDRSTRLEN] = {0};
    const void* addrPtr = nullptr;

    if (addr->addr.sa.sa_family == AF_INET) {
        addrPtr = &addr->addr.sin.sin_addr;
    } else if (addr->addr.sa.sa_family == AF_INET6) {
        addrPtr = &addr->addr.sin6.sin6_addr;
    } else {
        return QString();
    }

    if (inet_ntop(addr->addr.sa.sa_family, addrPtr, addrStr, sizeof(addrStr)) == nullptr) {
        return QString();
    }

    return QString::fromUtf8(addrStr);
}

QString CoAPCommon::getSessionRemoteAddress(coap_session_t* session)
{
    return getCoAPAddress(&session->addr_info.remote);
}

quint16 CoAPCommon::getSessionRemotePort(coap_session_t* session)
{
    return coap_address_get_port(&session->addr_info.remote);
}

QString CoAPCommon::getSessionLocalAddress(coap_session_t* session)
{
    return getCoAPAddress(&session->addr_info.local);
}

quint16 CoAPCommon::getSessionLocalPort(coap_session_t* session)
{
    return coap_address_get_port(&session->addr_info.local);
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
    obj[ClientParameterKeys().option] = QString::fromLatin1(params.option.toBase64());
    return obj;
}

CoAPCommon::ClientParameters CoAPCommon::jsonObject2ClientParameters(const QJsonObject& obj)
{
    ClientParameters params;
    params.serverAddress = obj.value(ClientParameterKeys().serverAddress).toString("localhost");
    params.serverPort = static_cast<quint16>(obj.value(ClientParameterKeys().serverPort).toInt(433));
    params.protocol = obj.value(ClientParameterKeys().protocol).toInt(COAP_PROTO_UDP);
    QString optionBase64 = obj.value(ClientParameterKeys().option).toString("");
    params.option = QByteArray::fromBase64(optionBase64.toLatin1());
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
