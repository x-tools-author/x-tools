/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktablemodeltool.h"
#include <QJsonArray>
#include <QJsonDocument>

SAKTableModelTool::SAKTableModelTool(QObject *parent)
    : SAKBaseTool{parent}
{
    mTableModel = new SAKTableModel(this);

    connect(mTableModel,
            &SAKTableModel::invokeGetRowCount,
            this,
            &SAKTableModelTool::onInvokeGetRowCount,
            Qt::DirectConnection);
    connect(mTableModel,
            &SAKTableModel::invokeGetColumnCount,
            this,
            &SAKTableModelTool::onInvokeGetColumnCount,
            Qt::DirectConnection);
    connect(mTableModel,
            &SAKTableModel::invokeGetData,
            this,
            &SAKTableModelTool::onInvokeGetData,
            Qt::DirectConnection);
    connect(mTableModel,
            &SAKTableModel::invokeSetData,
            this,
            &SAKTableModelTool::onInvokeSetData,
            Qt::DirectConnection);
    connect(mTableModel,
            &SAKTableModel::invokeInsertRows,
            this,
            &SAKTableModelTool::onInvokeInsertRows,
            Qt::DirectConnection);
    connect(mTableModel,
            &SAKTableModel::invokeRemoveRows,
            this,
            &SAKTableModelTool::onInvokeRemoveRows,
            Qt::DirectConnection);
    connect(mTableModel,
            &SAKTableModel::invokeGetHeaderData,
            this,
            &SAKTableModelTool::onInvokeGetHeaderData,
            Qt::DirectConnection);
}

QVariant SAKTableModelTool::tableModel()
{
    return QVariant::fromValue(mTableModel);
}

QStringList SAKTableModelTool::headers() const
{
    int count = columnCount();
    QStringList strList;
    for (int i = 0; i < count; i++) {
        QVariant var = headerData(i, Qt::Horizontal);
        strList.append(var.toString());
    }

    return strList;
}

void SAKTableModelTool::addItem(const QString &jsonCtx, int index)
{
    QByteArray json = jsonCtx.toUtf8();
    QJsonObject jsonObj = QJsonDocument::fromJson(json).object();
    if (!(index >= 0 && index < rowCount())) {
        mTableModel->insertRows(mTableModel->rowCount(), 1);
        index = mTableModel->rowCount() - 1;
    }

    for (int i = 0; i < headers().count(); i++) {
        if (i >= columnCount()) {
            qWarning() << "Invalid column index!";
            return;
        }

        auto key = headers().at(i);
        auto modelIndex = mTableModel->index(index, i);
        mTableModel->setData(modelIndex, jsonObj.value(key), Qt::EditRole);
        qInfo() << qPrintable(QString("set %1 as").arg(key)) << jsonObj.value(key);
    }
}

QVariant SAKTableModelTool::itemsContext()
{
    QJsonArray arr;
    int rowCount = mTableModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        arr.append(itemContext(i).toJsonObject());
    }

    return QVariant::fromValue(arr);
}

QString SAKTableModelTool::cookHeaderString(const QString &str)
{
    return str;
}

void SAKTableModelTool::onInvokeGetRowCount(int &count)
{
    count = rowCount();
}

void SAKTableModelTool::onInvokeGetColumnCount(int &count)
{
    count = columnCount();
}

void SAKTableModelTool::onInvokeGetData(QVariant &data, const QModelIndex &index, int role)
{
    data = this->data(index, role);
}

void SAKTableModelTool::onInvokeSetData(bool &result,
                                        const QModelIndex &index,
                                        const QVariant &value,
                                        int role)
{
    result = setData(index, value, role);
}

void SAKTableModelTool::onInvokeInsertRows(bool &result,
                                           int row,
                                           int count,
                                           const QModelIndex &parent)
{
    result = insertRows(row, count, parent);
}

void SAKTableModelTool::onInvokeRemoveRows(bool &result,
                                           int row,
                                           int count,
                                           const QModelIndex &parent)
{
    result = removeRows(row, count, parent);
}

void SAKTableModelTool::onInvokeGetHeaderData(QVariant &data,
                                              int section,
                                              Qt::Orientation orientation,
                                              int role)
{
    data = headerData(section, orientation, role);
}
