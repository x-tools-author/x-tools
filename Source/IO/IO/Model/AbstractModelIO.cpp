/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AbstractModelIO.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

namespace xTools {

AbstractModelIO::AbstractModelIO(QObject *parent)
    : AbstractIO{parent}
{
    m_tableModel = new TableModel(this);

    connect(m_tableModel,
            &TableModel::invokeGetRowCount,
            this,
            &AbstractModelIO::onInvokeGetRowCount,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeGetColumnCount,
            this,
            &AbstractModelIO::onInvokeGetColumnCount,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeGetData,
            this,
            &AbstractModelIO::onInvokeGetData,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeSetData,
            this,
            &AbstractModelIO::onInvokeSetData,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeInsertRows,
            this,
            &AbstractModelIO::onInvokeInsertRows,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeRemoveRows,
            this,
            &AbstractModelIO::onInvokeRemoveRows,
            Qt::DirectConnection);
    connect(m_tableModel,
            &TableModel::invokeGetHeaderData,
            this,
            &AbstractModelIO::onInvokeGetHeaderData,
            Qt::DirectConnection);
}

QVariant AbstractModelIO::tableModel() const
{
    return QVariant::fromValue(m_tableModel);
}

void AbstractModelIO::onInvokeGetRowCount(int &count)
{
    count = rowCount();
}

void AbstractModelIO::onInvokeGetColumnCount(int &count)
{
    count = columnCount();
}

void AbstractModelIO::onInvokeGetData(QVariant &data, const QModelIndex &index, int role)
{
    data = this->data(index, role);
}

void AbstractModelIO::onInvokeSetData(bool &result,
                                    const QModelIndex &index,
                                    const QVariant &value,
                                    int role)
{
    result = setData(index, value, role);
}

void AbstractModelIO::onInvokeInsertRows(bool &result, int row, int count, const QModelIndex &parent)
{
    result = insertRows(row, count, parent);
}

void AbstractModelIO::onInvokeRemoveRows(bool &result, int row, int count, const QModelIndex &parent)
{
    result = removeRows(row, count, parent);
}

void AbstractModelIO::onInvokeGetHeaderData(QVariant &data,
                                          int section,
                                          Qt::Orientation orientation,
                                          int role)
{
    data = headerData(section, orientation, role);
}

} // namespace xTools
