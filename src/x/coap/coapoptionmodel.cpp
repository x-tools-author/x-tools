/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "CoAPOptionModel.h"

#include <QMutex>

#include <coap3/coap.h>

namespace xCoAP {

class CoAPOptionModelPrivate : public QObject
{
public:
    CoAPOptionModelPrivate(CoAPOptionModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPOptionModelPrivate() override {}

public:
    struct MsgItem
    {
        bool isRx;
        const QString host;
        const quint16 port;
        const coap_pdu_t* pdu;
    };
    QList<MsgItem> m_msgList;
    QMutex m_msgListMutex;

private:
    CoAPOptionModel* q{nullptr};
};

CoAPOptionModel::CoAPOptionModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPOptionModelPrivate(this);
}

CoAPOptionModel::~CoAPOptionModel() {}

QVariant CoAPOptionModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
    }

    return QVariant();
}

int CoAPOptionModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    d->m_msgListMutex.lock();
    int count = d->m_msgList.count();
    d->m_msgListMutex.unlock();
    return count;
}

int CoAPOptionModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 10;
}

QVariant CoAPOptionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case COAP_MODEL_COLUMN_FLAG:
                return tr("Flag");
            case COAP_MODEL_COLUMN_HOST:
                return tr("Host");
            case COAP_MODEL_COLUMN_VERSION:
                return tr("Version");
            case COAP_MODEL_COLUMN_TYPE:
                return tr("Type");
            case COAP_MODEL_COLUMN_TOKEN_LENGTH:
                return tr("Token Length");
            case COAP_MODEL_COLUMN_CODE:
                return tr("Code");
            case COAP_MODEL_COLUMN_MESSAGE_ID:
                return tr("Message ID");
            case COAP_MODEL_COLUMN_TOKEN:
                return tr("Token");
            case COAP_MODEL_COLUMN_OPTIONS:
                return tr("Options");
            case COAP_MODEL_COLUMN_PAYLOAD:
                return tr("Payload");
            default:
                return QVariant();
            }
        }
    } else {
        Q_UNUSED(role);
        return QString::number(section + 1);
    }

    return QVariant();
}

} // namespace xCoAP