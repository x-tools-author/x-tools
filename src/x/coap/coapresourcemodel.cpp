/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapresourcemodel.h"

namespace xCoAP {

class CoAPResourceModelPrivate : public QObject
{
public:
    CoAPResourceModelPrivate(CoAPResourceModel* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~CoAPResourceModelPrivate() override {}

private:
    CoAPResourceModel* q{nullptr};
};

CoAPResourceModel::CoAPResourceModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    d = new CoAPResourceModelPrivate(this);
}

CoAPResourceModel::~CoAPResourceModel() {}

int CoAPResourceModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 0;
}

int CoAPResourceModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 0;
}

QVariant CoAPResourceModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index);
    Q_UNUSED(role);
    return QVariant();
}

} // namespace xCoAP