/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadmodel.h"

#include <QMutex>

#include <coap3/coap.h>

namespace xCoAP {

class CoAPPayloadModelPrivate : public QObject
{
public:
    CoAPPayloadModelPrivate(CoAPPayloadModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPPayloadModelPrivate() override {}

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
    CoAPPayloadModel* q{nullptr};
};

CoAPPayloadModel::CoAPPayloadModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPPayloadModelPrivate(this);
}

CoAPPayloadModel::~CoAPPayloadModel() {}

QVariant CoAPPayloadModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
    }

    return QVariant();
}

int CoAPPayloadModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    d->m_msgListMutex.lock();
    int count = d->m_msgList.count();
    d->m_msgListMutex.unlock();
    return count;
}

int CoAPPayloadModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant CoAPPayloadModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case CO_AP_PAYLOAD_MODEL_COLUMN_DESCRIPTION:
                return tr("Description");
            case CO_AP_PAYLOAD_MODEL_COLUMN_FORMAT:
                return tr("Format");
            case CO_AP_PAYLOAD_MODEL_COLUMN_DATA:
                return tr("Data");
            default:
                return QVariant();
            }
        }
    } else {
        if (role == Qt::DisplayRole) {
            return QString::number(section + 1);
        }
    }

    return QVariant();
}

} // namespace xCoAP
