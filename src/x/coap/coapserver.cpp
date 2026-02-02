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
#include <QMutex>
#include <QThread>

#include <coap3/coap.h>

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#include "coapcommon.h"
#include "coapglobal.h"

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

public:
    static void getHandler(coap_resource_t* resource,
                           coap_session_t* session,
                           const coap_pdu_t* request,
                           const coap_string_t* query,
                           coap_pdu_t* response)
    {
        // Get request resource path
        coap_str_const_t* uri_path = coap_resource_get_uri_path(resource);
        qInfo() << "Received GET request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uri_path->s), uri_path->length));

        static const char kPayload[] = "Hello from xTools CoAP server";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }
    static void postHandler(coap_resource_t* resource,
                            coap_session_t* session,
                            const coap_pdu_t* request,
                            const coap_string_t* query,
                            coap_pdu_t* response)
    {
        Q_UNUSED(resource);
        Q_UNUSED(session);
        Q_UNUSED(request);
        Q_UNUSED(query);

        bool cache = gCoAPGlobal.isEnableCachePostMessages();
        if (cache) {
            QString rootPath = gCoAPGlobal.serverCachePath();
            qInfo() << "Caching POST message as enabled.";
        }

        // Get request resource path
        coap_str_const_t* uriPath = coap_resource_get_uri_path(resource);
        qInfo() << "Received POST request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uriPath->s), uriPath->length));

        static const char kPayload[] = "POST request received";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }
    static void putHandler(coap_resource_t* resource,
                           coap_session_t* session,
                           const coap_pdu_t* request,
                           const coap_string_t* query,
                           coap_pdu_t* response)
    {
        Q_UNUSED(resource);
        Q_UNUSED(session);
        Q_UNUSED(request);
        Q_UNUSED(query);

        // Get request resource path
        coap_str_const_t* uri_path = coap_resource_get_uri_path(resource);
        qInfo() << "Received PUT request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uri_path->s), uri_path->length));

        static const char kPayload[] = "PUT request received";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }
    static void deleteHandler(coap_resource_t* resource,
                              coap_session_t* session,
                              const coap_pdu_t* request,
                              const coap_string_t* query,
                              coap_pdu_t* response)
    {
        Q_UNUSED(resource);
        Q_UNUSED(session);
        Q_UNUSED(request);
        Q_UNUSED(query);

        // Get request resource path
        coap_str_const_t* uri_path = coap_resource_get_uri_path(resource);
        qInfo() << "Received DELETE request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uri_path->s), uri_path->length));

        static const char kPayload[] = "DELETE request received";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_DELETED);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }
    static void patchHandler(coap_resource_t* resource,
                             coap_session_t* session,
                             const coap_pdu_t* request,
                             const coap_string_t* query,
                             coap_pdu_t* response)
    {
        Q_UNUSED(resource);
        Q_UNUSED(session);
        Q_UNUSED(request);
        Q_UNUSED(query);

        // Get request resource path
        coap_str_const_t* uri_path = coap_resource_get_uri_path(resource);
        qInfo() << "Received PATCH request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uri_path->s), uri_path->length));

        static const char kPayload[] = "PATCH request received";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }
    static void ipatchHandler(coap_resource_t* resource,
                              coap_session_t* session,
                              const coap_pdu_t* request,
                              const coap_string_t* query,
                              coap_pdu_t* response)
    {
        Q_UNUSED(resource);
        Q_UNUSED(session);
        Q_UNUSED(request);
        Q_UNUSED(query);

        // Get request resource path
        coap_str_const_t* uri_path = coap_resource_get_uri_path(resource);
        qInfo() << "Received iPATCH request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uri_path->s), uri_path->length));

        static const char kPayload[] = "iPATCH request received";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }
    static void fetchHandler(coap_resource_t* resource,
                             coap_session_t* session,
                             const coap_pdu_t* request,
                             const coap_string_t* query,
                             coap_pdu_t* response)
    {
        Q_UNUSED(resource);
        Q_UNUSED(session);
        Q_UNUSED(request);
        Q_UNUSED(query);

        // Get request resource path
        coap_str_const_t* uri_path = coap_resource_get_uri_path(resource);
        qInfo() << "Received FETCH request for resource:"
                << QString::fromStdString(
                       std::string(reinterpret_cast<const char*>(uri_path->s), uri_path->length));

        static const char kPayload[] = "FETCH request received";
        coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
        coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
    }

private:
    CoAPServer* q{nullptr};
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
    if (!endpoint) {
        qWarning() << "Failed to create CoAP endpoint";
        coap_free_context(ctx);
        return;
    }

    coap_resource_t* resource = coap_resource_init(coap_make_str_const("hello"), 0);
    if (!resource) {
        qWarning() << "Failed to create CoAP resource";
        coap_free_context(ctx);
        return;
    }

    coap_register_request_handler(resource, COAP_REQUEST_GET, CoAPServerPrivate::getHandler);
    coap_add_resource(ctx, resource);

    while (!isInterruptionRequested()) {
        int result = coap_io_process(ctx, 50);
        if (result < 0) {
            qWarning() << "coap_io_process returned error" << result;
            break;
        }
    }

    coap_free_context(ctx);
    qInfo() << "CoAP server thread finished:" << QThread::currentThread();
}

} // namespace xCoAP
