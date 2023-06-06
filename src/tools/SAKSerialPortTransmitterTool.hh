/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSERIALPORTTRANSMITTERTOOL_HH
#define SAKSERIALPORTTRANSMITTERTOOL_HH

#include "SAKTransmitterTool.hh"

class SAKSerialPortTransmitterTool : public SAKTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKSerialPortTransmitterTool(QObject *parent = nullptr);

    Q_INVOKABLE virtual QVariant itemContext(int index) override;

protected:
    virtual int rowCount(const QModelIndex &parent
                         = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent
                            = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual bool insertRows(int row,
                            int count,
                            const QModelIndex &parent
                            = QModelIndex()) override;
    virtual bool removeRows(int row,
                            int count,
                            const QModelIndex &parent
                            = QModelIndex()) override;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const = 0;
};

#endif // SAKSERIALPORTTRANSMITTERTOOL_HH
