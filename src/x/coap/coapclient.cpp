/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapclient.h"

#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include <coap3/coap.h>
#include <coap3/coap_debug.h>
#include <coap3/coap_session.h>
#include <coap3/coap_session_internal.h>

#include "coapcommon.h"

namespace xCoAP {

class CoAPClientPrivate : public QObject
{
public:
    CoAPClientPrivate(CoAPClient *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPClientPrivate() override {}

public:
    QJsonObject m_parameters;
    QMutex m_parametersMutex;

public:
    static coap_response_t messageHandler(coap_session_t *session,
                                          const coap_pdu_t *sent,
                                          const coap_pdu_t *received,
                                          const coap_mid_t id)
    {
        if (!session || !sent || !received || !session->app) {
            return COAP_RESPONSE_OK;
        }

        quint16 port = CoAPCommon::getSessionRemotePort(session);
        QString address = CoAPCommon::getSessionRemoteAddress(session);
        QString server = QString("%1:%2").arg(address).arg(port);
        port = CoAPCommon::getSessionLocalPort(session);
        address = CoAPCommon::getSessionLocalAddress(session);
        QString client = QString("%1:%2").arg(address).arg(port);

        std::shared_ptr<CoAPMsgItem> requestPdu = std::make_shared<CoAPMsgItem>();
        requestPdu->isRx = false;
        requestPdu->messageId = id;
        requestPdu->version = (sent->hdr_size & 0xC0);
        requestPdu->clientHost = client;
        requestPdu->serverHost = server;
        requestPdu->type = sent->type;

        std::shared_ptr<CoAPMsgItem> responsePdu = std::make_shared<CoAPMsgItem>();
        responsePdu->isRx = true;
        responsePdu->messageId = id;
        responsePdu->version = (received->hdr_size & 0xC0);
        responsePdu->clientHost = client;
        responsePdu->serverHost = server;

        //------------------------------------------------------------------------------------------
        // Get payload
        const uint8_t *data;
        size_t len;
        size_t offset;
        size_t total;

        QByteArray payload;
        if (coap_get_data_large(sent, &len, &data, &offset, &total)) {
            payload.append(reinterpret_cast<const char *>(data), static_cast<int>(len));
        }
        requestPdu->payload = payload;

        payload.clear();
        if (coap_get_data_large(received, &len, &data, &offset, &total)) {
            payload.append(reinterpret_cast<const char *>(data), static_cast<int>(len));
        }
        responsePdu->payload = payload;

        //------------------------------------------------------------------------------------------
        CoAPClient *clientInstance = static_cast<CoAPClient *>(session->app);
        emit clientInstance->messageReceived(std::move(requestPdu), std::move(responsePdu));
        return COAP_RESPONSE_OK;
    }
    void eventLoopTimeout(QTimer *timer, coap_context_t *ctx)
    {
        int result = coap_io_process(ctx, 100);
        if (result < 0) {
            qCWarning(xCoAPClientLog) << "coap_io_process returned error" << result;
            q->exit();
            return;
        }

        timer->start();
    }
    void sendMessage(const QByteArray &payload,
                     const QByteArray &token,
                     const QByteArray &resource,
                     const QByteArray &option,
                     int code,
                     coap_session_t *session)
    {
        coap_pdu_code_t cookedCode = static_cast<coap_pdu_code_t>(code);
        coap_pdu_t *request = coap_new_pdu(COAP_MESSAGE_CON, cookedCode, session);
        if (!request) {
            qCWarning(xCoAPClientLog) << "Failed to create CoAP request PDU";
            return;
        }

        // Payload...
        if (!payload.isEmpty()) {
            int ret = coap_add_data(request,
                                    static_cast<size_t>(payload.size()),
                                    reinterpret_cast<const uint8_t *>(payload.constData()));
            if (ret == 0) {
                qCWarning(xCoAPClientLog) << "Failed to add payload to CoAP request";
                return;
            }
        }

        // Token...
        if (token.isEmpty()) {
            if (!coap_add_token(request,
                                static_cast<size_t>(token.size()),
                                reinterpret_cast<const uint8_t *>(token.constData()))) {
                qCWarning(xCoAPClientLog) << "Failed to add token to CoAP request";
                return;
            }
        }

        // Resource path...
        if (resource.isEmpty()) {
            qCWarning(xCoAPClientLog) << "Resource path is empty";
            return;
        }
        coap_add_option(request,
                        COAP_OPTION_URI_PATH,
                        resource.size(),
                        reinterpret_cast<const uint8_t *>(resource.constData()));
        if (coap_send(session, request) == COAP_INVALID_MID) {
            qCWarning(xCoAPClientLog) << "Failed to send CoAP request";
            return;
        }
#if 0
        // Options...
        if (!option.isEmpty()) {
            coap_add_option(request,
                            COAP_OPTION_ACCEPT,
                            option.size(),
                            reinterpret_cast<const uint8_t *>(option.constData()));
        }
#endif
    }

private:
    CoAPClient *q{nullptr};
};

CoAPClient::CoAPClient(QObject *parent)
    : QThread(parent)
{
    d = new CoAPClientPrivate(this);
}

CoAPClient::~CoAPClient() {}

void CoAPClient::startClient(const QJsonObject &parameters)
{
    stopClient();
    d->m_parametersMutex.lock();
    d->m_parameters = parameters;
    d->m_parametersMutex.unlock();
    start();
}

void CoAPClient::stopClient()
{
    if (isRunning()) {
        requestInterruption();
        exit();
        wait();
    }
}

void CoAPClient::run()
{
    qCDebug(xCoAPClientLog) << "CoAP client thread started:" << QThread::currentThread();
    d->m_parametersMutex.lock();
    CoAPCommon::ClientParameters params = CoAPCommon::jsonObject2ClientParameters(d->m_parameters);
    d->m_parametersMutex.unlock();

    if (!CoAPCommon::isValidProtocol(params.protocol)) {
        qCCritical(xCoAPClientLog) << "Invalid CoAP protocol:" << params.protocol;
        return;
    }

    coap_context_t *ctx = coap_new_context(nullptr);
    if (!ctx) {
        qCCritical(xCoAPClientLog) << "Failed to create CoAP context";
        return;
    }

    coap_register_response_handler(ctx, &CoAPClientPrivate::messageHandler);

    coap_address_t dst;
    coap_address_init(&dst);
    dst.addr.sin.sin_family = AF_INET;
    dst.addr.sin.sin_port = htons(params.serverPort);
    void *addrPtr = reinterpret_cast<void *>(&dst.addr.sin.sin_addr);
    if (inet_pton(AF_INET, params.serverAddress.toStdString().c_str(), addrPtr) == -1) {
        qCCritical(xCoAPClientLog) << "Invalid CoAP server address:" << params.serverAddress;
        coap_free_context(ctx);
        return;
    }

    qCInfo(xCoAPClientLog,
           "Connecting to CoAP server %s:%d via protocol %d",
           params.serverAddress.toStdString().c_str(),
           params.serverPort,
           params.protocol);

    coap_proto_t proto = static_cast<coap_proto_t>(params.protocol);
    coap_session_t *session = coap_new_client_session(ctx, nullptr, &dst, proto);
    if (!session) {
        qCCritical(xCoAPClientLog) << "Failed to create CoAP client session";
        coap_free_context(ctx);
        return;
    }
    session->app = this;

    // Send message to CoAP server...
    QTimer *timer = new QTimer();
    connect(this,
            &CoAPClient::invokeSendMessage,
            timer,
            [=](const QByteArray &payload,
                const QByteArray &token,
                const QByteArray &resource,
                const QByteArray &option,
                int code) { d->sendMessage(payload, token, resource, option, code, session); });

    // Event loop...
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, timer, [=]() { d->eventLoopTimeout(timer, ctx); });
    timer->start();
    exec();
    coap_session_release(session);
    coap_free_context(ctx);
    qCDebug(xCoAPClientLog) << "CoAP client thread finished:" << QThread::currentThread();
}

} // namespace xCoAP
