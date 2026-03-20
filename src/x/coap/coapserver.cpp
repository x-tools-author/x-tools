/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapserver.h"

#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QThread>
#include <QTimer>

#include <coap3/coap.h>
#include <coap3/coap_session.h>
#include <coap3/coap_session_internal.h>

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#include "coapcommon.h"
#include "coapglobal.h"
#include "coapresourcemodel.h"

namespace xCoAP {

class CoAPServerPrivate : public QObject
{
public:
    CoAPServerPrivate(CoAPServer* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPServerPrivate() override {}

public:
    QJsonObject m_parameters;
    QMutex m_parametersMutex;
    QMutex m_uriPathsMutex;
    CoAPResourceModel* m_resModel{nullptr};

public:
    static void getHandler(coap_resource_t* resource,
                           coap_session_t* session,
                           const coap_pdu_t* request,
                           const coap_string_t* query,
                           coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "GET request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received GET request from cached file:" << uriPath;

        CoAPResourceModel* model = q->d->m_resModel;
        QStringList uriPaths = model->uriPaths();
        QByteArray payload = model->getPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
        coap_add_data(response, payload.size(), data);
        outputMessage(resource, session, request, query, response);
    }
    static void postHandler(coap_resource_t* resource,
                            coap_session_t* session,
                            const coap_pdu_t* request,
                            const coap_string_t* query,
                            coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "POST request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received POST request for resource:" << uriPath;
        CoAPResourceModel* model = q->d->m_resModel;
        QByteArray payload = model->getPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        size_t len = payload.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_add_data(response, len, data);
        outputMessage(resource, session, request, query, response);
    }
    static void putHandler(coap_resource_t* resource,
                           coap_session_t* session,
                           const coap_pdu_t* request,
                           const coap_string_t* query,
                           coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "PUT request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received PUT request for resource:" << uriPath;

        CoAPResourceModel* model = q->d->m_resModel;
        QByteArray payload = model->getPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        size_t len = payload.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_add_data(response, len, data);
        outputMessage(resource, session, request, query, response);
    }
    static void deleteHandler(coap_resource_t* resource,
                              coap_session_t* session,
                              const coap_pdu_t* request,
                              const coap_string_t* query,
                              coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "DELETE request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received DELETE request for resource:" << uriPath;

        CoAPResourceModel* model = q->d->m_resModel;
        QByteArray payload = model->getPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        size_t len = payload.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_add_data(response, len, data);
        outputMessage(resource, session, request, query, response);
    }
    static void patchHandler(coap_resource_t* resource,
                             coap_session_t* session,
                             const coap_pdu_t* request,
                             const coap_string_t* query,
                             coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "PATCH request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received PATCH request for resource:" << uriPath;

        CoAPResourceModel* model = q->d->m_resModel;
        QByteArray payload = model->patchPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        size_t len = payload.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_add_data(response, len, data);
        outputMessage(resource, session, request, query, response);
    }
    static void ipatchHandler(coap_resource_t* resource,
                              coap_session_t* session,
                              const coap_pdu_t* request,
                              const coap_string_t* query,
                              coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "iPATCH request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received iPATCH request for resource:" << uriPath;

        CoAPResourceModel* model = q->d->m_resModel;
        QByteArray payload = model->ipatchPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        size_t len = payload.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_add_data(response, len, data);
        outputMessage(resource, session, request, query, response);
    }
    static void fetchHandler(coap_resource_t* resource,
                             coap_session_t* session,
                             const coap_pdu_t* request,
                             const coap_string_t* query,
                             coap_pdu_t* response)
    {
        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (!(q && q->d && q->d->m_resModel)) {
            qCWarning(xCoAPServerLog) << "FETCH request received but inner error occurred!";
            return;
        }

        QString uriPath = CoAPCommon::getCoAPResource(resource);
        qCDebug(xCoAPServerLog) << "Received FETCH request for resource:" << uriPath;

        CoAPResourceModel* model = q->d->m_resModel;
        QByteArray payload = model->fetchPayload(uriPath, CoAPCommon::getCoAPPayload(request));
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
        size_t len = payload.size();
        const uint8_t* data = reinterpret_cast<const uint8_t*>(payload.constData());
        coap_add_data(response, len, data);
        outputMessage(resource, session, request, query, response);
    }

public:
    void registerResource(coap_context_t* ctx, QStringList& registeredUriPaths)
    {
        m_uriPathsMutex.lock();
        QStringList uriPaths = m_resModel->uriPaths();
        m_uriPathsMutex.unlock();

        // Resource list not changed
        if (uriPaths == registeredUriPaths) {
            return;
        }

        // Cancel deleted resources
        for (const QString& oldUri : std::as_const(registeredUriPaths)) {
            if (!uriPaths.contains(oldUri)) {
                const QByteArray bytes = oldUri.toUtf8();
                coap_str_const_t* str = coap_make_str_const(bytes.constData());
                coap_resource_t* resource = coap_resource_init(str, 0);
                coap_register_handler(resource, COAP_REQUEST_GET, NULL);
            }
        }

        // Add new resources
        for (const QString& uriPath : std::as_const(uriPaths)) {
            QByteArray tmp = uriPath.toUtf8();
            if (registeredUriPaths.contains(tmp)) {
                continue;
            }

            registeredUriPaths.append(tmp);
            coap_str_const_t* str = coap_make_str_const(tmp);
            coap_resource_t* resource = coap_resource_init(str, 0);
            if (!resource) {
                qWarning() << "Failed to create CoAP resource";
                continue;
            }

            // clang-format off
            coap_register_request_handler(resource, COAP_REQUEST_GET, CoAPServerPrivate::getHandler);
            coap_register_request_handler(resource, COAP_REQUEST_POST, CoAPServerPrivate::postHandler);
            coap_register_request_handler(resource, COAP_REQUEST_PUT, CoAPServerPrivate::putHandler);
            coap_register_request_handler(resource, COAP_REQUEST_DELETE, CoAPServerPrivate::deleteHandler);
            coap_register_request_handler(resource, COAP_REQUEST_PATCH, CoAPServerPrivate::patchHandler);
            coap_register_request_handler(resource, COAP_REQUEST_IPATCH, CoAPServerPrivate::ipatchHandler);
            coap_add_resource(ctx, resource);
            // clang-format on
            qCDebug(xCoAPServerLog) << "Registered CoAP resource:" << uriPath;
        }

        registeredUriPaths = uriPaths;
    }

private:
    CoAPServer* q{nullptr};

private:
    static void outputMessage(coap_resource_t* resource,
                              coap_session_t* session,
                              const coap_pdu_t* request,
                              const coap_string_t* query,
                              coap_pdu_t* response)
    {
        quint16 port = CoAPCommon::getCoAPRemotePort(session);
        QString address = CoAPCommon::getCoAPRemoteAddress(session);
        QString server = QString("%1:%2").arg(address).arg(port);
        port = CoAPCommon::getCoAPLocalPort(session);
        address = CoAPCommon::getCoAPLocalAddress(session);
        QString client = QString("%1:%2").arg(address).arg(port);

        std::shared_ptr<CoAPMsgItem> requestPdu = std::make_shared<CoAPMsgItem>();
        int id = ntohs(request->mid);
        requestPdu->isRx = false;
        requestPdu->messageId = id;
        requestPdu->version = (request->hdr_size & 0xC0);
        requestPdu->clientHost = client;
        requestPdu->serverHost = server;
        requestPdu->type = request->type;
        requestPdu->code = request->code;
        if (request->e_token_length > 0 && request->actual_token.length > 0) {
            const char* tokenData = reinterpret_cast<const char*>(request->actual_token.s);
            int tokenLen = static_cast<int>(request->actual_token.length);
            requestPdu->token.append(tokenData, tokenLen);
        }

        std::shared_ptr<CoAPMsgItem> responsePdu = std::make_shared<CoAPMsgItem>();
        responsePdu->isRx = true;
        responsePdu->messageId = id;
        responsePdu->version = (response->hdr_size & 0xC0);
        responsePdu->clientHost = client;
        responsePdu->serverHost = server;
        responsePdu->type = response->type;
        responsePdu->code = response->code;
        if (response->e_token_length > 0 && response->actual_token.length > 0) {
            const char* tokenData = reinterpret_cast<const char*>(response->actual_token.s);
            int tokenLen = static_cast<int>(response->actual_token.length);
            responsePdu->token.append(tokenData, tokenLen);
        }

        CoAPServer* q = static_cast<CoAPServer*>(session->context->app);
        if (q) {
            emit static_cast<CoAPServer*>(q)->messageReceived(responsePdu, requestPdu);
        }
    }
};

CoAPServer::CoAPServer(QObject* parent)
    : QThread(parent)
{
    d = new CoAPServerPrivate(this);
}

CoAPServer::~CoAPServer()
{
    stopServer();
}

void CoAPServer::startServer(const QJsonObject& parameters)
{
    d->m_parametersMutex.lock();
    d->m_parameters = parameters;
    d->m_parametersMutex.unlock();
    if (isRunning()) {
        exit();
        wait();
    }
    start();
}

void CoAPServer::stopServer()
{
    if (isRunning()) {
        requestInterruption();
        exit();
        wait();
    }
}

void CoAPServer::setCoAPResourceModel(CoAPResourceModel* model)
{
    d->m_resModel = model;
}

void CoAPServer::run()
{
    qInfo() << "CoAP server thread started:" << QThread::currentThread();

    d->m_parametersMutex.lock();
    CoAPCommon::ServerParameters params = CoAPCommon::jsonObject2ServerParameters(d->m_parameters);
    d->m_parametersMutex.unlock();

    if (!CoAPCommon::isValidProtocol(params.protocol)) {
        qWarning() << "Invalid CoAP protocol:" << params.protocol;
        return;
    }

    coap_context_t* ctx = coap_new_context(nullptr);
    if (!ctx) {
        qWarning() << "Failed to create CoAP context";
        return;
    }

    coap_address_t addr;
    coap_address_init(&addr);
    addr.addr.sin.sin_family = AF_INET;
    addr.addr.sin.sin_port = htons(params.serverPort);
    void* addrPtr = reinterpret_cast<void*>(&addr.addr.sin.sin_addr);
    if (inet_pton(AF_INET, params.serverAddress.toStdString().c_str(), addrPtr) == -1) {
        qWarning() << "Invalid CoAP server address:" << params.serverAddress;
        coap_free_context(ctx);
        return;
    }

    coap_proto_t proto = static_cast<coap_proto_t>(params.protocol);
    coap_endpoint_t* endpoint = coap_new_endpoint(ctx, &addr, proto);
    endpoint->context->app = this;
    if (!endpoint) {
        qWarning() << "Failed to create CoAP endpoint";
        coap_free_context(ctx);
        return;
    }

    // CoAP event loop
    QTimer* processTimer = new QTimer();
    processTimer->setInterval(10);
    connect(processTimer, &QTimer::timeout, processTimer, [ctx]() {
        int result = coap_io_process(ctx, 50);
        Q_UNUSED(result);
    });

    // Register resources(Using timer)
    QTimer* resourceTimer = new QTimer();
    resourceTimer->setInterval(100);
    QStringList registeredUriPaths;
    connect(resourceTimer, &QTimer::timeout, resourceTimer, [this, ctx, &registeredUriPaths]() {
        this->d->registerResource(ctx, registeredUriPaths);
    });

    ctx->app = this;
    processTimer->start();
    resourceTimer->start();
    exec();
    processTimer->stop();
    resourceTimer->stop();
    coap_free_context(ctx);
    qInfo() << "CoAP server thread finished:" << QThread::currentThread();
}

} // namespace xCoAP
