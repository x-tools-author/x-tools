/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AbstractModel.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

namespace xTools {

AbstractModel::AbstractModel(QObject *parent)
    : AbstractIO{parent}
{
    m_tableModel = new TableModel(this);

    connect(m_tableModel,
            &TableModel::invokeGetRowCount,
            this,
            &AbstractModel::onInvokeGetRowCount,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeGetColumnCount,
            this,
            &AbstractModel::onInvokeGetColumnCount,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeGetData,
            this,
            &AbstractModel::onInvokeGetData,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeSetData,
            this,
            &AbstractModel::onInvokeSetData,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeInsertRows,
            this,
            &AbstractModel::onInvokeInsertRows,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeRemoveRows,
            this,
            &AbstractModel::onInvokeRemoveRows,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeGetHeaderData,
            this,
            &AbstractModel::onInvokeGetHeaderData,
            Qt::DirectConnection);
}

QVariant AbstractModel::tableModel() const
{
    return QVariant::fromValue(m_tableModel);
}

void AbstractModel::onInvokeGetRowCount(int &count)
{
    count = rowCount();
}

void AbstractModel::onInvokeGetColumnCount(int &count)
{
    count = columnCount();
}

void AbstractModel::onInvokeGetData(QVariant &data, const QModelIndex &index, int role)
{
    data = this->data(index, role);
}

void AbstractModel::onInvokeSetData(bool &result,
                                    const QModelIndex &index,
                                    const QVariant &value,
                                    int role)
{
    result = setData(index, value, role);
}

void AbstractModel::onInvokeInsertRows(bool &result, int row, int count, const QModelIndex &parent)
{
    result = insertRows(row, count, parent);
}

void AbstractModel::onInvokeRemoveRows(bool &result, int row, int count, const QModelIndex &parent)
{
    result = removeRows(row, count, parent);
}

void AbstractModel::onInvokeGetHeaderData(QVariant &data,
                                          int section,
                                          Qt::Orientation orientation,
                                          int role)
{
    data = headerData(section, orientation, role);
}

} // namespace xTools
