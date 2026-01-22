/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xcoap.h"

#include "coapclientui.h"
#include "coapserverui.h"

namespace xCoAP {

class xCoAPPrivate : public QObject
{
public:
    xCoAPPrivate(xCoAP* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        m_client = new CoAPClientUi(q);
        m_server = new CoAPServerUi(q);
    }
    ~xCoAPPrivate() {}

public:
    CoAPClientUi* m_client{nullptr};
    CoAPServerUi* m_server{nullptr};

private:
    xCoAP* q{nullptr};
};

xCoAP::xCoAP(QWidget* parent)
    : LeftRightUi(parent)
{
    d = new xCoAPPrivate(this);
    setupUi(tr("CoAP Client"), d->m_client, tr("CoAP Server"), d->m_server);
}

xCoAP::~xCoAP() {}

QJsonObject xCoAP::save()
{
    QJsonObject obj;
    return obj;
}

void xCoAP::load(const QJsonObject& obj) {}

} // namespace xCoAP
