/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#define DATARECORDS_COLUMN_COLUMN_DATETIME 0
#define DATARECORDS_COLUMN_COLUMN_TYPE 1
#define DATARECORDS_COLUMN_COLUMN_FLAG 2
#define DATARECORDS_COLUMN_COLUMN_DATA 3

class DataRecordsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum DataRecordsModelItemType {
        DataRecordsModelItemTypeAll,
        DataRecordsModelItemTypeRx,
        DataRecordsModelItemTypeTx
    };
    Q_ENUM(DataRecordsModelItemType);

    struct Item
    {
        QString dateTime;
        int type;
        QString flag;
        QByteArray bytes;
    };

public:
    explicit DataRecordsModel(QObject *parent = nullptr);
    ~DataRecordsModel();

    void addRxItem(const QString &flag, const QByteArray &bytes);
    void addTxItem(const QString &flag, const QByteArray &bytes);
    void addItem(int type, const QString &flag, const QByteArray &bytes);
    void setFormat(int format);
    void setMaxRecords(int count);
    void clear();

    Item itemAt(int row) const;
    QString dataText(const Item &item) const;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<Item> m_items;
    int m_format = 0;
    int m_maxRecords = -1;
};