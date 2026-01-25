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
        const uint8_t *data;
        size_t len;
        size_t offset;
        size_t total;

        (void) session;
        (void) sent;
        (void) id;
        if (coap_get_data_large(received, &len, &data, &offset, &total)) {
            printf("1111%*.*s", (int) len, (int) len, (const char *) data);
            if (len + offset == total) {
                printf("\n");
            }
        }
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

void CoAPClient::getMessage(const QString &path) {}

void CoAPClient::postMessage(const QByteArray &payload, const QString &path)
{
    Q_UNUSED(payload);
    Q_UNUSED(path);
}

void CoAPClient::putMessage(const QByteArray &payload, const QString &path)
{
    Q_UNUSED(payload);
    Q_UNUSED(path);
}

void CoAPClient::deleteMessage(const QString &path)
{
    Q_UNUSED(path);
}

void CoAPClient::fetchMessage(const QString &path)
{
    Q_UNUSED(path);
}

void CoAPClient::patchMessage(const QByteArray &payload, const QString &path)
{
    Q_UNUSED(payload);
    Q_UNUSED(path);
}

void CoAPClient::iPatchMessage(const QByteArray &payload, const QString &path)
{
    Q_UNUSED(payload);
    Q_UNUSED(path);
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

    coap_pdu_t *request = coap_new_pdu(COAP_MESSAGE_CON, COAP_REQUEST_CODE_GET, session);
    if (!request) {
        qCCritical(xCoAPClientLog) << "Failed to create CoAP request PDU";
        coap_session_release(session);
        coap_free_context(ctx);
        return;
    }

    // Token...
    static uint8_t s_token = 0;
    uint8_t token_data[1];
    token_data[0] = ++s_token;
    if (!coap_add_token(request, sizeof(token_data), token_data)) {
        qCWarning(xCoAPClientLog) << "Failed to add token to CoAP request";
        coap_session_release(session);
        coap_free_context(ctx);
        return;
    }

    // URI Path...
    const coap_str_const_t *path = coap_make_str_const("hello");
    coap_add_option(request, COAP_OPTION_URI_PATH, path->length, path->s);
    if (coap_send(session, request) == COAP_INVALID_MID) {
        qCWarning(xCoAPClientLog) << "Failed to send CoAP request";
        coap_session_release(session);
        coap_free_context(ctx);
        return;
    }

    // Event loop...
    QTimer *timer = new QTimer();
    timer->setInterval(10);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, [=]() { d->eventLoopTimeout(timer, ctx); });
    timer->start();
    exec();
    coap_session_release(session);
    coap_free_context(ctx);
    qCDebug(xCoAPClientLog) << "CoAP client thread finished:" << QThread::currentThread();
}

} // namespace xCoAP
