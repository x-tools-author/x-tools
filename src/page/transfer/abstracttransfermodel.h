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

class Device;

class AbstractTransferModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AbstractTransferModel(QObject *parent = nullptr);
    ~AbstractTransferModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    void inputBytes(const QByteArray &bytes);
    void startAll();
    void stopAll();
    void setEnableRestartTransfer(bool enable);

signals:
    void outputBytes(const QByteArray &bytes);

protected:
    struct Item
    {
        Device *transfer{nullptr};
        QString description;
        int option;
    };
    QList<Item> m_transfers;
    bool m_enableRestartTransfer{false};

protected:
    virtual Device *createTransfer() = 0;
    virtual bool isEnableRestartingColumn(int column) const;
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
