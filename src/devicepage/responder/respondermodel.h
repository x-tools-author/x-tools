/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QMutex>

#include "common/xtools.h"

namespace xTools {

class ResponderModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ResponderModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    struct Item
    {
        bool enable{true};
        QString description{"Demo"};
        ResponseOption option = ResponseOption::InputEqualReference;
        int delay{1000};
        TextItem referenceTextContext;
        TextItem responseTextContext;
    };

private:
    QList<Item> m_items;
};

} // namespace xTools
