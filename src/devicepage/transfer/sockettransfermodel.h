/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "abstracttransfermodel.h"

class Socket;
class SocketTransferModel : public AbstractTransferModel
{
    Q_OBJECT
public:
    SocketTransferModel(QObject *parent = nullptr);
    ~SocketTransferModel() override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
    virtual Socket *createSocket() = 0;
    Communication *createTransfer() override;
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QList<int> &roles = QList<int>()) override;
};
