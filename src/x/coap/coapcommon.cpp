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

} // namespace xCoAP
