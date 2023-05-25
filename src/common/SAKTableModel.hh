/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTABLEMODEL_HH
#define SAKTABLEMODEL_HH

#include <QAbstractTableModel>

class SAKTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SAKTableModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

signals:
    // You must connect these signals using Qt::DirectConnection way.
    void invokeGetRowCount(int &count) const;
    void invokeGetColumnCount(int &count) const;
    void invokeGetData(QVariant &data, const QModelIndex &index, int role = Qt::DisplayRole) const;
    void invokeSetData(bool &result, const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void invokeInsertRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void invokeRemoveRows(bool &result, int row, int count, const QModelIndex &parent = QModelIndex());
    void invokeGetHeaderData(QVariant &data, int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
};

#endif // SAKTABLEMODEL_HH
