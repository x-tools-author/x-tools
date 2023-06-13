/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPTRANSMITTERTOOL_HH
#define SAKUDPTRANSMITTERTOOL_HH

#include "SAKUdpClientTool.hh"
#include "SAKTransmitterTool.hh"

class SAKUdpTransmitterTool : public SAKTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKUdpTransmitterTool(QObject *parent = nullptr);
    Q_INVOKABLE virtual QVariant itemContext(int index) override;

public:
    struct ItemContextKeys {
        const QString enable{"enable"};
        const QString clientIp{"clientIp"};
        const QString clientPort{"clientPort"};
        const QString specifiedClientIpPort{"specifiedClientIpAndPort"};
        const QString serverIp{"serverIp"};
        const QString serverPort{"serverPort"};
    };

protected:
    virtual int columnCount(const QModelIndex &parent
                            = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole);
    virtual bool insertRows(int row,
                            int count,
                            const QModelIndex &parent = QModelIndex());
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
};

#endif // SAKUDPTRANSMITTERTOOL_HH
