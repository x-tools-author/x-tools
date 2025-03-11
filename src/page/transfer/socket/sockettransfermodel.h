/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/transfer/common/transfermodel.h"

#define SOCKET_ROW_OPTION 0
#define SOCKET_ROW_ADDRESS 1
#define SOCKET_ROW_PORT 2
#define SOCKET_ROW_CHANNEL 3
#define SOCKET_ROW_AUTHENTICATION 4
#define SOCKET_ROW_USERNAME 5
#define SOCKET_ROW_PASSWORD 6
#define SOCKET_ROW_DESCRIPTION 7

class Socket;
class SocketTransferModel : public TransferModel
{
    Q_OBJECT
public:
    SocketTransferModel(QObject *parent = nullptr);
    ~SocketTransferModel() override;

    QVariantMap saveRow(const int row) override;
    void loadRow(const int row, const QVariantMap &item) override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

protected:
    virtual Socket *createSocket() = 0;
    Device *createTransfer() override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QList<int> &roles = QList<int>()) override;
#else
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int>()) override;
#endif
};
