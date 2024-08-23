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

namespace xTools {

class Communication;

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
        Communication *transfer{nullptr};
        QString description;
        int option;
    };
    QList<Item> m_transfers;
    bool m_enableRestartTransfer{false};

protected:
    virtual Communication *createTransfer() = 0;
    virtual bool isEnableRestartingColumn(int column) const;

private:
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QList<int> &roles = QList<int>());
};

} // namespace xTools
