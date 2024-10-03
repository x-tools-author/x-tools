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

#include "AbstractModelIO.h"
#include "IO/xIO.h"

namespace xTools {

class PresetModel;
class Preset : public AbstractModelIO
{
    Q_OBJECT
public:
    explicit Preset(QObject *parent = nullptr);

    QVariant tableModel() const override;
    QVariantMap saveItem(const int row) const override;
    void loadItem(const int row, const QVariantMap &item) override;
    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override;

private:
    struct Item
    {
        QString description{"Demo"};
        xIO::TextItem textContext;
    };
    PresetModel *m_tableModel{nullptr};
    int rowCount(const QModelIndex &parent) const;
    QList<Item> m_items;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool removeRows(int row, int count, const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
};

} // namespace xTools
