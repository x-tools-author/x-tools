/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/common/tablemodel.h"

class Device;

class TransferModel : public TableModel
{
    Q_OBJECT
public:
    TransferModel(QObject *parent = nullptr);
    ~TransferModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    void inputBytes(const QByteArray &bytes);
    void startAll();
    void stopAll();

signals:
    void outputBytes(const QByteArray &bytes);
    void bytesRead(const QByteArray &bytes, const QString &flag);
    void bytesWritten(const QByteArray &bytes, const QString &flag);

protected:
    struct Item
    {
        Device *transfer{nullptr};
        QString description;
        int option;
        bool isEnable;
    };
    QList<Item> m_transfers;
    bool m_enableRestart{false};

protected:
    virtual Device *createTransfer() = 0;
    virtual bool isEnableRestartColumn(int column) const;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual void onDataChanged(const QModelIndex &topLeft,
                               const QModelIndex &bottomRight,
                               const QList<int> &roles = QList<int>());
#else
    virtual void onDataChanged(const QModelIndex &topLeft,
                               const QModelIndex &bottomRight,
                               const QVector<int> &roles = QVector<int>());
#endif
};
