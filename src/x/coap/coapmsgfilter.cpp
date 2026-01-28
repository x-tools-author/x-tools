/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapmsgfilter.h"

#include <QMutex>

#include <coap3/coap.h>

namespace xCoAP {

class CoAPMsgFilterPrivate : public QObject
{
public:
    CoAPMsgFilterPrivate(CoAPMsgFilter* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPMsgFilterPrivate() {}

private:
    CoAPMsgFilter* q{nullptr};
};

CoAPMsgFilter::CoAPMsgFilter(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    d = new CoAPMsgFilterPrivate(this);
}

CoAPMsgFilter::~CoAPMsgFilter() {}

} // namespace xCoAP
