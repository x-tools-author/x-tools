/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QObject>
#include <QSpinBox>

struct coap_pdu_t;
struct coap_session_t;
struct coap_resource_t;
#define xCoAPLog QLoggingCategory("xTools.xCoAP")
#define xCoAPClientLog QLoggingCategory("xTools.xCoAP.Client")
#define xCoAPServerLog QLoggingCategory("xTools.xCoAP.Server")

#define CO_AP_INVALID_CONTEXT_FORMAT -1

namespace xCoAP {

struct CoAPMsgItem
{
    bool isRx;
    int messageId;
    QString clientHost;
    QString serverHost;
    int version;
    int type;
    int code;
    QByteArray token;
    QByteArray options;
    QByteArray payload;
};

class CoAPCommon : public QObject
{
    Q_OBJECT
public:
    explicit CoAPCommon(QObject* parent = nullptr);
    ~CoAPCommon();

    static void setupSocketAddress(QComboBox* comboBox);
    static void setupSocketPort(QSpinBox* comboBox);
    static void setupSocketProtocol(QComboBox* comboBox);
    static void setupContextFormat(QComboBox* cb);

public:
    static QString getContextFormatSuffix(int format);
    static QString getContextFormatString(int format);
    static QString getCoAPRemoteAddress(coap_session_t* session);
    static quint16 getCoAPRemotePort(coap_session_t* session);
    static QString getCoAPLocalAddress(coap_session_t* session);
    static quint16 getCoAPLocalPort(coap_session_t* session);
    static QByteArray getCoAPPayload(const coap_pdu_t* pdu);
    static QString getCoAPResource(coap_resource_t* resource);
    static uint32_t getCoAPPayloadFormat(const coap_pdu_t* pdu);

public:
    static bool isValidProtocol(int protocol);
    static bool clearAllItems();
    static bool deleteSelectedItems();

public:
    struct ClientParameterKeys
    {
        const QString serverAddress{"serverAddress"};
        const QString serverPort{"serverPort"};
        const QString protocol{"protocol"};
        const QString option{"option"};
    };
    struct ClientParameters
    {
        QString serverAddress;
        quint16 serverPort;
        int protocol;
        QByteArray option;
    };
    static QJsonObject clientParameters2JsonObject(const ClientParameters& params);
    static ClientParameters jsonObject2ClientParameters(const QJsonObject& obj);

public:
    struct ServerParameterKeys
    {
        const QString serverAddress{"serverAddress"};
        const QString serverPort{"serverPort"};
        const QString protocol{"protocol"};
    };
    struct ServerParameters
    {
        QString serverAddress;
        quint16 serverPort;
        int protocol;
    };
    static QJsonObject serverParameters2JsonObject(const ServerParameters& params);
    static ServerParameters jsonObject2ServerParameters(const QJsonObject& obj);

public:
    struct PayloadContextKeys
    {
        const QString description{"description"};
        const QString format{"format"};
        const QString data{"data"};
    };
    struct PayloadContext
    {
        QString description;
        int format;
        QByteArray data;
    };
    static PayloadContext defaultPayloadContext();
    static QJsonObject payloadContext2JsonObject(const PayloadContext& context);
    static PayloadContext jsonObject2PayloadContext(const QJsonObject& obj);

public:
    struct ResourceItem
    {
        QString description;
        QString uriPath;
        int contextFormat;
        QByteArray payload;
    };
    struct ResourceItemKeys
    {
        const QString description{"description"};
        const QString uriPath{"uriPath"};
        const QString contextFormat{"contextFormat"};
        const QString payload{"payload"};
    };
    static QJsonObject resourceItem2JsonObject(const ResourceItem& item);
    static ResourceItem jsonObject2ResourceItem(const QJsonObject& obj);
};

} // namespace xCoAP
