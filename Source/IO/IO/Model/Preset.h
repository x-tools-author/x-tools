/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTimer>

#include "../../xIO.h"
#include "AbstractModel.h"

namespace xTools {

class Preset : public AbstractModel
{
    Q_OBJECT
public:
    struct Item
    {
        QString description{"Demo"};
        xIO::TextItemContext textContext;
    };

    struct ItemKeys : public xIO::TextItemParameterKeys
    {
        const QString description{"description"};
        const xIO::TextItemParameterKeys textContext{};
    };

public:
    explicit Preset(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

protected:
    void inputBytes(const QByteArray &bytes) override;
    void run() override;

private:
    QList<Item> m_items;
};

} // namespace xTools
