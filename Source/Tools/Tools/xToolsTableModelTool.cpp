/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTableModelTool.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

xToolsTableModelTool::xToolsTableModelTool(QObject *parent)
    : xToolsBaseTool{parent}
{
    m_tableModel = new xToolsTableModel(this);
    
    connect(m_tableModel,
            &xToolsTableModel::invokeGetRowCount,
            this,
            &xToolsTableModelTool::onInvokeGetRowCount,
            Qt::DirectConnection);
    connect(m_tableModel,
            &xToolsTableModel::invokeGetColumnCount,
            this,
            &xToolsTableModelTool::onInvokeGetColumnCount,
            Qt::DirectConnection);
    connect(m_tableModel,
            &xToolsTableModel::invokeGetData,
            this,
            &xToolsTableModelTool::onInvokeGetData,
            Qt::DirectConnection);
    connect(m_tableModel,
            &xToolsTableModel::invokeSetData,
            this,
            &xToolsTableModelTool::onInvokeSetData,
            Qt::DirectConnection);
    connect(m_tableModel,
            &xToolsTableModel::invokeInsertRows,
            this,
            &xToolsTableModelTool::onInvokeInsertRows,
            Qt::DirectConnection);
    connect(m_tableModel,
            &xToolsTableModel::invokeRemoveRows,
            this,
            &xToolsTableModelTool::onInvokeRemoveRows,
            Qt::DirectConnection);
    connect(m_tableModel,
            &xToolsTableModel::invokeGetHeaderData,
            this,
            &xToolsTableModelTool::onInvokeGetHeaderData,
            Qt::DirectConnection);
}

QVariant xToolsTableModelTool::tableModel()
{
    return QVariant::fromValue(m_tableModel);
}

QStringList xToolsTableModelTool::headers() const
{
    int count = columnCount();
    QStringList strList;
    for (int i = 0; i < count; i++) {
        QVariant var = headerData(i, Qt::Horizontal);
        strList.append(var.toString());
    }

    return strList;
}

void xToolsTableModelTool::addItem(const QString &jsonCtx, int index)
{
    QByteArray json = jsonCtx.toUtf8();
    QJsonObject jsonObj = QJsonDocument::fromJson(json).object();
    if (!(index >= 0 && index < rowCount())) {
        m_tableModel->insertRows(m_tableModel->rowCount(), 1);
        index = m_tableModel->rowCount() - 1;
    }

    for (int i = 0; i < headers().count(); i++) {
        if (i >= columnCount()) {
            qWarning() << "Invalid column index!";
            return;
        }

        auto key = headers().at(i);
        auto modelIndex = m_tableModel->index(index, i);
        m_tableModel->setData(modelIndex, jsonObj.value(key), Qt::EditRole);
        qInfo() << qPrintable(QString("set %1 as").arg(key)) << jsonObj.value(key);
    }
}

QVariant xToolsTableModelTool::itemsContext()
{
    QJsonArray arr;
    int rowCount = m_tableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        arr.append(itemContext(i).toJsonObject());
    }

    return QVariant::fromValue(arr);
}

QString xToolsTableModelTool::cookHeaderString(const QString &str)
{
    return str;
}

void xToolsTableModelTool::onInvokeGetRowCount(int &count)
{
    count = rowCount();
}

void xToolsTableModelTool::onInvokeGetColumnCount(int &count)
{
    count = columnCount();
}

void xToolsTableModelTool::onInvokeGetData(QVariant &data, const QModelIndex &index, int role)
{
    data = this->data(index, role);
}

void xToolsTableModelTool::onInvokeSetData(bool &result,
                                        const QModelIndex &index,
                                        const QVariant &value,
                                        int role)
{
    result = setData(index, value, role);
}

void xToolsTableModelTool::onInvokeInsertRows(bool &result,
                                           int row,
                                           int count,
                                           const QModelIndex &parent)
{
    result = insertRows(row, count, parent);
}

void xToolsTableModelTool::onInvokeRemoveRows(bool &result,
                                           int row,
                                           int count,
                                           const QModelIndex &parent)
{
    result = removeRows(row, count, parent);
}

void xToolsTableModelTool::onInvokeGetHeaderData(QVariant &data,
                                              int section,
                                              Qt::Orientation orientation,
                                              int role)
{
    data = headerData(section, orientation, role);
}
