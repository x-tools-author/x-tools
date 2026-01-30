/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadfilter.h"

#include <QMutex>

#include <coap3/coap.h>

namespace xCoAP {

class CoAPPayloadFilterPrivate : public QObject
{
public:
    CoAPPayloadFilterPrivate(CoAPPayloadFilter* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPPayloadFilterPrivate() override {}

public:
    struct CoAPMsgItem
    {
        bool isRx;
        const QString host;
        const quint16 port;
        const coap_pdu_t* pdu;
    };
    QList<CoAPMsgItem> m_msgList;
    QMutex m_msgListMutex;

private:
    CoAPPayloadFilter* q{nullptr};
};

CoAPPayloadFilter::CoAPPayloadFilter(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPPayloadFilterPrivate(this);
}

CoAPPayloadFilter::~CoAPPayloadFilter() {}

} // namespace xCoAP
