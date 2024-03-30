/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseTool.h"
#include "xToolsTableModel.h"

class xToolsTableModelTool : public xToolsBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QVariant tableModel READ tableModel CONSTANT)
    Q_PROPERTY(QStringList headers READ headers CONSTANT)
public:
    explicit xToolsTableModelTool(QObject *parent = nullptr);
    QVariant tableModel();
    QStringList headers() const;

    Q_INVOKABLE void addItem(const QString &jsonCtx, int index = -1);
    Q_INVOKABLE QVariant itemsContext();
    Q_INVOKABLE virtual QString cookHeaderString(const QString &str);
    Q_INVOKABLE virtual QVariant itemContext(int index) = 0;

protected:
    // clang-format off
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) = 0;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) = 0;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) = 0;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const = 0;
    // clang-format on

protected:
    xToolsTableModel *m_tableModel;

private:
    // clang-format off
    void onInvokeGetRowCount(int &count);
    void onInvokeGetColumnCount(int &count);
    void onInvokeGetData(QVariant &data, const QModelIndex &index, int role = Qt::DisplayRole);
    void onInvokeSetData(bool &result, const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void onInvokeInsertRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void onInvokeRemoveRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void onInvokeGetHeaderData(QVariant &data, int section, Qt::Orientation orientation, int role = Qt::DisplayRole);
    // clang-format on
};
