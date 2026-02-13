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

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

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
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_JSON), COAP_MEDIATYPE_APPLICATION_JSON, comboBox);
    // TODO: Add more context formats
#if 0
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_TEXT_PLAIN), COAP_MEDIATYPE_TEXT_PLAIN, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_OCTET_STREAM), COAP_MEDIATYPE_APPLICATION_OCTET_STREAM, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_LINK_FORMAT), COAP_MEDIATYPE_APPLICATION_LINK_FORMAT, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_XML), COAP_MEDIATYPE_APPLICATION_XML, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_EXI), COAP_MEDIATYPE_APPLICATION_EXI, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_CBOR), COAP_MEDIATYPE_APPLICATION_CBOR, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_CWT), COAP_MEDIATYPE_APPLICATION_CWT, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COAP_GROUP_JSON), COAP_MEDIATYPE_APPLICATION_COAP_GROUP_JSON, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_SIGN), COAP_MEDIATYPE_APPLICATION_COSE_SIGN, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_SIGN1), COAP_MEDIATYPE_APPLICATION_COSE_SIGN1, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT), COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT0), COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT0, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_MAC), COAP_MEDIATYPE_APPLICATION_COSE_MAC, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_MAC0), COAP_MEDIATYPE_APPLICATION_COSE_MAC0, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_KEY), COAP_MEDIATYPE_APPLICATION_COSE_KEY, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_COSE_KEY_SET), COAP_MEDIATYPE_APPLICATION_COSE_KEY_SET, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENML_JSON), COAP_MEDIATYPE_APPLICATION_SENML_JSON, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENSML_JSON), COAP_MEDIATYPE_APPLICATION_SENSML_JSON, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENML_CBOR), COAP_MEDIATYPE_APPLICATION_SENML_CBOR, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENSML_CBOR), COAP_MEDIATYPE_APPLICATION_SENSML_CBOR, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENML_EXI), COAP_MEDIATYPE_APPLICATION_SENML_EXI, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENSML_EXI), COAP_MEDIATYPE_APPLICATION_SENSML_EXI, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENML_XML), COAP_MEDIATYPE_APPLICATION_SENML_XML, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_SENSML_XML), COAP_MEDIATYPE_APPLICATION_SENSML_XML, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_DOTS_CBOR), COAP_MEDIATYPE_APPLICATION_DOTS_CBOR, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_ACE_CBOR), COAP_MEDIATYPE_APPLICATION_ACE_CBOR, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_MB_CBOR_SEQ), COAP_MEDIATYPE_APPLICATION_MB_CBOR_SEQ, comboBox);
    addContextFormatItem(getContextFormatString(COAP_MEDIATYPE_APPLICATION_OSCORE), COAP_MEDIATYPE_APPLICATION_OSCORE, comboBox);
#endif
    // clang-format on

    int index = comboBox->findData(COAP_MEDIATYPE_APPLICATION_JSON);
    if (index != -1) {
        comboBox->setCurrentIndex(index);
    }
}

QString CoAPCommon::getContextFormatSuffix(int format)
{
    switch (format) {
    case COAP_MEDIATYPE_APPLICATION_JSON:
        return QStringLiteral("json");
    case COAP_MEDIATYPE_TEXT_PLAIN:
        return QStringLiteral("txt");
    case COAP_MEDIATYPE_APPLICATION_OCTET_STREAM:
        return QStringLiteral("bin");
    case COAP_MEDIATYPE_APPLICATION_LINK_FORMAT:
        return QStringLiteral("link");
    case COAP_MEDIATYPE_APPLICATION_XML:
        return QStringLiteral("xml");
    case COAP_MEDIATYPE_APPLICATION_EXI:
        return QStringLiteral("exi");
    case COAP_MEDIATYPE_APPLICATION_CBOR:
        return QStringLiteral("cbor");
    case COAP_MEDIATYPE_APPLICATION_CWT:
        return QStringLiteral("cwt");
    case COAP_MEDIATYPE_APPLICATION_COAP_GROUP_JSON:
        return QStringLiteral("group.json");
    case COAP_MEDIATYPE_APPLICATION_COSE_SIGN:
        return QStringLiteral("cose-sign");
    case COAP_MEDIATYPE_APPLICATION_COSE_SIGN1:
        return QStringLiteral("cose-sign1");
    case COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT:
        return QStringLiteral("cose-encrypt");
    case COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT0:
        return QStringLiteral("cose-encrypt0");
    case COAP_MEDIATYPE_APPLICATION_COSE_MAC:
        return QStringLiteral("cose-mac");
    case COAP_MEDIATYPE_APPLICATION_COSE_MAC0:
        return QStringLiteral("cose-mac0");
    case COAP_MEDIATYPE_APPLICATION_COSE_KEY:
        return QStringLiteral("cose-key");
    case COAP_MEDIATYPE_APPLICATION_COSE_KEY_SET:
        return QStringLiteral("cose-key-set");
    case COAP_MEDIATYPE_APPLICATION_SENML_JSON:
        return QStringLiteral("senml.json");
    case COAP_MEDIATYPE_APPLICATION_SENSML_JSON:
        return QStringLiteral("sensml.json");
    case COAP_MEDIATYPE_APPLICATION_SENML_CBOR:
        return QStringLiteral("senml.cbor");
    case COAP_MEDIATYPE_APPLICATION_SENSML_CBOR:
        return QStringLiteral("sensml.cbor");
    case COAP_MEDIATYPE_APPLICATION_SENML_EXI:
        return QStringLiteral("senml.exi");
    case COAP_MEDIATYPE_APPLICATION_SENSML_EXI:
        return QStringLiteral("sensml.exi");
    case COAP_MEDIATYPE_APPLICATION_SENML_XML:
        return QStringLiteral("senml.xml");
    case COAP_MEDIATYPE_APPLICATION_SENSML_XML:
        return QStringLiteral("sensml.xml");
    case COAP_MEDIATYPE_APPLICATION_DOTS_CBOR:
        return QStringLiteral("dots.cbor");
    case COAP_MEDIATYPE_APPLICATION_ACE_CBOR:
        return QStringLiteral("ace.cbor");
    case COAP_MEDIATYPE_APPLICATION_MB_CBOR_SEQ:
        return QStringLiteral("mb.cbor-seq");
    case COAP_MEDIATYPE_APPLICATION_OSCORE:
        return QStringLiteral("oscore");
    default:
        return QString();
    }
}

QString CoAPCommon::getContextFormatString(int format)
{
    static QMap<int, QString> formatMap;
    if (formatMap.isEmpty()) {
        // clang-format off
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_JSON, "application/json");
        formatMap.insert(COAP_MEDIATYPE_TEXT_PLAIN, "text/plain");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_OCTET_STREAM, "application/octet-stream");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_LINK_FORMAT, "application/link-format");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_XML, "application/xml");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_EXI, "application/exi");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_CBOR, "application/cbor");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_CWT, "application/cwt");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COAP_GROUP_JSON, "application/coap-group+json");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_SIGN, "application/cose; cose-type=\"cose-sign\"");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_SIGN1, "application/cose; cose-type=\"cose-sign1\"");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT, "application/cose; cose-type=\"cose-encrypt\"");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_ENCRYPT0,"application/cose; cose-type=\"cose-encrypt0\"");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_MAC, "application/cose; cose-type=\"cose-mac\"");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_MAC0, "application/cose; cose-type=\"cose-mac0\"");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_KEY, "application/cose-key");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_COSE_KEY_SET, "application/cose-key-set");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENML_JSON, "application/senml+json");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENSML_JSON, "application/sensml+json");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENML_CBOR, "application/senml+cbor");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENSML_CBOR, "application/sensml+cbor");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENML_EXI, "application/senml-exi");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENSML_EXI, "application/sensml-exi");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENML_XML, "application/senml+xml");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_SENSML_XML, "application/sensml+xml");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_DOTS_CBOR, "application/dots+cbor");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_ACE_CBOR, "application/ace+cbor");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_MB_CBOR_SEQ, "application/missing-blocks+cbor-seq");
        formatMap.insert(COAP_MEDIATYPE_APPLICATION_OSCORE, "application/oscore");
        // clang-format on
    }

    return formatMap.value(format, QString());
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

QString CoAPCommon::getCoAPRemoteAddress(coap_session_t* session)
{
    return getCoAPAddress(&session->addr_info.remote);
}

quint16 CoAPCommon::getCoAPRemotePort(coap_session_t* session)
{
    return coap_address_get_port(&session->addr_info.remote);
}

QString CoAPCommon::getCoAPLocalAddress(coap_session_t* session)
{
    return getCoAPAddress(&session->addr_info.local);
}

quint16 CoAPCommon::getCoAPLocalPort(coap_session_t* session)
{
    return coap_address_get_port(&session->addr_info.local);
}

QByteArray CoAPCommon::getCoAPPayload(const coap_pdu_t* pdu)
{
    const uint8_t* data;
    size_t len;
    size_t offset;
    size_t total;
    QByteArray payload;
    if (coap_get_data_large(pdu, &len, &data, &offset, &total)) {
        payload = QByteArray(reinterpret_cast<const char*>(data), static_cast<int>(len));
    }

    return payload;
}

QString CoAPCommon::getCoAPResource(coap_resource_t* resource)
{
    coap_str_const_t* uriPath = coap_resource_get_uri_path(resource);
    std::string uriPathStr(reinterpret_cast<const char*>(uriPath->s), uriPath->length);
    QString uriPathQStr = QString::fromStdString(uriPathStr);
    return uriPathQStr;
}

uint32_t CoAPCommon::getCoAPPayloadFormat(const coap_pdu_t* pdu)
{
    coap_opt_iterator_t opt_iter;
    coap_opt_t* opt = coap_check_option(pdu, COAP_OPTION_CONTENT_FORMAT, &opt_iter);
    if (opt) {
        const uint8_t* buf = coap_opt_value(opt);
        size_t len = coap_opt_length(opt);
        uint32_t contextFormat = coap_decode_var_bytes(buf, len);
        return contextFormat;
    }

    return CO_AP_INVALID_CONTEXT_FORMAT;
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

bool CoAPCommon::clearAllItems()
{
    int ret = QMessageBox::warning(
        nullptr,
        QObject::tr("Clear All Items"),
        QObject::tr("Are you sure to clear all items? This action cannot be undone."),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    return ret == QMessageBox::Yes;
}

bool CoAPCommon::deleteSelectedItems()
{
    int ret = QMessageBox::warning(
        nullptr,
        QObject::tr("Delete Selected Items"),
        QObject::tr("Are you sure to delete selected items? This action cannot be undone."),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    return ret == QMessageBox::Yes;
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

CoAPCommon::PayloadContext CoAPCommon::defaultPayloadContext()
{
    QJsonObject payload;
    payload.insert("project", "xTools");
    payload.insert("subProject", "xCoAP");
    payload.insert("author", "x-tools-author");
    payload.insert("email", "x-tools@outlook.com");
    QJsonDocument doc(payload);
    QByteArray data = doc.toJson(QJsonDocument::Indented);
    PayloadContext context;
    context.description = QString("Default Payload");
    context.format = COAP_MEDIATYPE_APPLICATION_JSON;
    context.data = data;
    return context;
}

CoAPCommon::PayloadContext CoAPCommon::jsonObject2PayloadContext(const QJsonObject& obj)
{
    PayloadContext context;
    PayloadContextKeys keys;
    context.description = obj.value(keys.description).toString("");
    context.format = obj.value(keys.format).toInt(COAP_MEDIATYPE_APPLICATION_JSON);
    QString dataBase64 = obj.value(keys.data).toString("");
    context.data = QByteArray::fromBase64(dataBase64.toLatin1());
    return context;
}

QJsonObject CoAPCommon::payloadContext2JsonObject(const PayloadContext& context)
{
    QJsonObject obj;
    PayloadContextKeys keys;
    obj[keys.description] = context.description;
    obj[keys.format] = context.format;
    obj[keys.data] = QString::fromLatin1(context.data.toBase64());
    return obj;
}

QJsonObject CoAPCommon::resourceItem2JsonObject(const ResourceItem& item)
{
    QJsonObject obj;
    ResourceItemKeys keys;
    obj[keys.description] = item.description;
    obj[keys.uriPath] = item.uriPath;
    obj[keys.contextFormat] = item.contextFormat;
    obj[keys.payload] = QString::fromLatin1(item.payload.toHex());
    return obj;
}

CoAPCommon::ResourceItem CoAPCommon::jsonObject2ResourceItem(const QJsonObject& obj)
{
    ResourceItem item;
    ResourceItemKeys keys;
    item.description = obj.value(keys.description).toString("xtools/xcoap/info");
    item.uriPath = obj.value(keys.uriPath).toString(QString("hello"));
    item.contextFormat = obj.value(keys.contextFormat).toInt(COAP_MEDIATYPE_APPLICATION_JSON);
    QString hex = obj.value(keys.payload).toString("");
    item.payload = QByteArray::fromHex(hex.toLatin1());
    if (item.payload.isEmpty()) {
        item.payload = defaultPayloadContext().data;
    }
    return item;
}

} // namespace xCoAP
