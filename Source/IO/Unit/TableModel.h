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

namespace xTools {

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);
    void setEitableColumns(const QList<int> &columns);

public:
    // clang-format off
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // clang-format on

    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    // You must connect these signals using Qt::DirectConnection mode.
    // clang-format off
    void invokeGetRowCount(int &count);
    void invokeGetColumnCount(int &count);
    void invokeGetData(QVariant &data, const QModelIndex &index, int role = Qt::DisplayRole);
    void invokeSetData(bool &result, const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void invokeInsertRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void invokeRemoveRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void invokeGetHeaderData(QVariant &data, int section, Qt::Orientation orientation, int role = Qt::DisplayRole);
    // clang-format on

private:
    QList<int> m_editableColumns;
};

} // namespace xTools
