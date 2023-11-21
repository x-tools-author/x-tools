/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTABLEMODEL_H
#define SAKTABLEMODEL_H

#include <QAbstractTableModel>

class SAKTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SAKTableModel(QObject *parent = nullptr);

public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const final;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const final;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const final;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) final;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) final;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) final;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const final;

signals:
    // You must connect these signals using Qt::DirectConnection way.
    void invokeGetRowCount(int &count);
    void invokeGetColumnCount(int &count);
    void invokeGetData(QVariant &data, const QModelIndex &index, int role = Qt::DisplayRole);
    void invokeSetData(bool &result,
                       const QModelIndex &index,
                       const QVariant &value,
                       int role = Qt::EditRole);
    void invokeInsertRows(bool &result,
                          int row,
                          int count,
                          const QModelIndex &parent = QModelIndex());
    void invokeRemoveRows(bool &result,
                          int row,
                          int count,
                          const QModelIndex &parent = QModelIndex());
    void invokeGetHeaderData(QVariant &data,
                             int section,
                             Qt::Orientation orientation,
                             int role = Qt::DisplayRole);
};

#endif // SAKTABLEMODEL_H
