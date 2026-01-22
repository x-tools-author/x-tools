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
#include <QThread>

#include <coap3/coap.h>

#include <cstring>

namespace xCoAP {

namespace {

static void hnd_get_hello(coap_resource_t* resource,
                          coap_session_t* session,
                          const coap_pdu_t* request,
                          const coap_string_t* query,
                          coap_pdu_t* response)
{
    Q_UNUSED(resource);
    Q_UNUSED(session);
    Q_UNUSED(request);
    Q_UNUSED(query);

    static const char kPayload[] = "Hello from xTools CoAP server";
    coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
    coap_add_data(response, std::strlen(kPayload), reinterpret_cast<const uint8_t*>(kPayload));
}

} // namespace

class CoAPServerPrivate : public QObject
{
public:
    CoAPServerPrivate(CoAPServer* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPServerPrivate() override {}

public:
private:
    CoAPServer* q{nullptr};
};

CoAPServer::CoAPServer(QObject* parent)
    : QThread(parent)
{}

CoAPServer::~CoAPServer() {}

void CoAPServer::startServer(const QJsonObject& config)
{
    Q_UNUSED(config);
    if (isRunning()) {
        exit();
        wait();
    }
}

void CoAPServer::run()
{
    qInfo() << "CoAP server thread started:" << QThread::currentThread();
    coap_context_t* ctx = coap_new_context(nullptr);
    if (!ctx) {
        qWarning() << "Failed to create CoAP context";
        return;
    }

    coap_address_t addr;
    coap_address_init(&addr);

#if defined(Q_OS_WIN)
    // 显式指定 IPv4 任意地址，避免在 Windows 上产生无效参数错误
    addr.addr.sin.sin_family = AF_INET;
    addr.addr.sin.sin_addr.s_addr = INADDR_ANY;
    addr.addr.sin.sin_port = htons(433);
#endif

    coap_endpoint_t* endpoint = coap_new_endpoint(ctx, &addr, COAP_PROTO_UDP);
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

    coap_register_request_handler(resource, COAP_REQUEST_GET, hnd_get_hello);
    coap_add_resource(ctx, resource);

    while (!isInterruptionRequested()) {
        int result = coap_io_process(ctx, 1000);
        if (result < 0) {
            qWarning() << "coap_io_process returned error" << result;
            break;
        }
    }

    coap_free_context(ctx);

    qInfo() << "CoAP server thread finished:" << QThread::currentThread();
}

} // namespace xCoAP
