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

struct coap_session_t;
#define xCoAPLog QLoggingCategory("xTools.xCoAP")
#define xCoAPClientLog QLoggingCategory("xTools.xCoAP.Client")
#define xCoAPServerLog QLoggingCategory("xTools.xCoAP.Server")

namespace xCoAP {

struct CoAPMsgItem
{
    bool isRx;
    int messageId;
    QString clientHost;
    QString serverHost;
    int version;
    int type;
    int tokenLength;
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

    static QString getSessionRemoteAddress(coap_session_t* session);
    static quint16 getSessionRemotePort(coap_session_t* session);
    static QString getSessionLocalAddress(coap_session_t* session);
    static quint16 getSessionLocalPort(coap_session_t* session);

public:
    static bool isValidProtocol(int protocol);

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
};

} // namespace xCoAP
