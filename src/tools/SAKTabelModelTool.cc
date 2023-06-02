/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QJsonDocument>
#include "SAKTabelModelTool.hh"

SAKTabelModelTool::SAKTabelModelTool(const char *logCategory, QObject *parent)
    : SAKBaseTool{logCategory, parent}
{
    mTableModel = new SAKTableModel(this);

    connect(mTableModel, &SAKTableModel::invokeGetRowCount,
            this, &SAKTabelModelTool::onInvokeGetRowCount,
            Qt::DirectConnection);
    connect(mTableModel, &SAKTableModel::invokeGetColumnCount,
            this, &SAKTabelModelTool::onInvokeGetColumnCount,
            Qt::DirectConnection);
    connect(mTableModel, &SAKTableModel::invokeGetData,
            this, &SAKTabelModelTool::onInvokeGetData,
            Qt::DirectConnection);
    connect(mTableModel, &SAKTableModel::invokeSetData,
            this, &SAKTabelModelTool::onInvokeSetData,
            Qt::DirectConnection);
    connect(mTableModel, &SAKTableModel::invokeInsertRows,
            this, &SAKTabelModelTool::onInvokeInsertRows,
            Qt::DirectConnection);
    connect(mTableModel, &SAKTableModel::invokeRemoveRows,
            this, &SAKTabelModelTool::onInvokeRemoveRows,
            Qt::DirectConnection);
    connect(mTableModel, &SAKTableModel::invokeGetHeaderData,
            this, &SAKTabelModelTool::onInvokeGetHeaderData,
            Qt::DirectConnection);
}

QVariant SAKTabelModelTool::tableModel()
{
    return QVariant::fromValue(mTableModel);
}

QStringList SAKTabelModelTool::headers() const
{
    int count = columnCount();
    QStringList strList;
    for (int i = 0; i < count; i++) {
        QVariant var = headerData(i, Qt::Horizontal);
        strList.append(var.toString());
    }

    return strList;
}

void SAKTabelModelTool::addItem(const QString &jsonCtx, int index)
{
    QByteArray json = jsonCtx.toLatin1();
    QJsonObject jsonObj = QJsonDocument::fromJson(json).object();
    if (!(index >= 0 && index < rowCount())) {
        mTableModel->insertRows(mTableModel->rowCount(), 1);
        index = mTableModel->rowCount() - 1;
    }

    for (int i = 0; i < headers().count(); i++) {
        if (i >= columnCount()) {
            outputMessage(QtWarningMsg, "Invalid column index!");
        }

        auto key = headers().at(i);
        auto modelIndex = mTableModel->index(index, i);
        mTableModel->setData(modelIndex, jsonObj.value(key), Qt::EditRole);
    }
}

void SAKTabelModelTool::onInvokeGetRowCount(int &count)
{
    count = rowCount();
}

void SAKTabelModelTool::onInvokeGetColumnCount(int &count)
{
    count = columnCount();
}

void SAKTabelModelTool::onInvokeGetData(QVariant &data,
                                        const QModelIndex &index,
                                        int role)
{
    data = this->data(index, role);
}

void SAKTabelModelTool::onInvokeSetData(bool &result,
                                        const QModelIndex &index,
                                        const QVariant &value,
                                        int role )
{
    result = setData(index, value, role);
}

void SAKTabelModelTool::onInvokeInsertRows(bool &result,
                                           int row,
                                           int count,
                                           const QModelIndex &parent)
{
    result = insertRows(row, count, parent);
}

void SAKTabelModelTool::onInvokeRemoveRows(bool &result,
                                           int row,
                                           int count,
                                           const QModelIndex &parent)
{
    result = removeRows(row, count, parent);
}

void SAKTabelModelTool::onInvokeGetHeaderData(QVariant &data,
                                              int section,
                                              Qt::Orientation orientation,
                                              int role)
{
    data = headerData(section, orientation, role);
}
