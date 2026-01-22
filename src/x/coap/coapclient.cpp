/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapclient.h"

#include <QThread>

#include <coap3/coap.h>
#include <coap3/coap_debug.h>

static coap_response_t coAPClientMessageHandler(coap_session_t *session,
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
        printf("%*.*s", (int) len, (int) len, (const char *) data);
        if (len + offset == total) {
            printf("\n");
            //quit = 1;
        }
    }
    return COAP_RESPONSE_OK;
}

namespace xCoAP {

class CoAPClientPrivate : public QObject
{
public:
    CoAPClientPrivate(CoAPClient *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPClientPrivate() override {}

private:
    CoAPClient *q{nullptr};
};

CoAPClient::CoAPClient(QObject *parent)
    : QThread(parent)
{}

CoAPClient::~CoAPClient() {}

void CoAPClient::run()
{
    coap_set_log_level(COAP_LOG_OSCORE);
    coap_context_t *ctx = coap_new_context(nullptr);
    coap_context_set_block_mode(ctx, COAP_BLOCK_USE_LIBCOAP);
}

} // namespace xCoAP
