/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

class xToolsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit xToolsTableModel(QObject *parent = nullptr);

public:
    // clang-format off
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // clang-format on

signals:
    // You must connect these signals using Qt::DirectConnection way.
    // clang-format off
    void invokeGetRowCount(int &count);
    void invokeGetColumnCount(int &count);
    void invokeGetData(QVariant &data, const QModelIndex &index, int role = Qt::DisplayRole);
    void invokeSetData(bool &result, const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void invokeInsertRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void invokeRemoveRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void invokeGetHeaderData(QVariant &data, int section, Qt::Orientation orientation, int role = Qt::DisplayRole);
    // clang-format on
};
