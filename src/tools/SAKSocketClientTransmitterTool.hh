/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTRANSMITTERTOOL_HH
#define SAKSOCKETCLIENTTRANSMITTERTOOL_HH

#include "SAKTransmitterTool.hh"
#include "SAKSocketClientTool.hh"

class SAKSocketClientTransmitterTool : public SAKTransmitterTool
{
    Q_OBJECT
public:
    explicit SAKSocketClientTransmitterTool(const char *logCategory,
                                            QObject *parent = nullptr);
    Q_INVOKABLE virtual QVariant itemContext(int index) override;

public:
    struct ItemContextKeys {
        const QString enable{"enable"};
        const QString clientIp{"clientIp"};
        const QString clientPort{"clientPort"};
        const QString specifiedClientIpPort{"specifiedClientIpAndPort"};
        const QString serverIp{"serverIp"};
        const QString serverPort{"serverPort"};

        const QString messageType{"messageType"};
    };

protected:
    virtual int columnCount(const QModelIndex &parent
                            = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual bool insertRows(int row,
                            int count,
                            const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;

    virtual SAKSocketClientTool *createTool() = 0;
};

#endif // SAKSOCKETCLIENTTRANSMITTERTOOL_HH
