/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

class Communication;

namespace xTools {

class AbstractTransferModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AbstractTransferModel(QObject *parent = nullptr);
    ~AbstractTransferModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

protected:
    struct Item
    {
        Communication *transfer{nullptr};
        bool enabled{false};
        QString description;
    };
    QList<Item> m_transfers;

protected:
    virtual Communication *createTransfer() = 0;
};

} // namespace xTools
