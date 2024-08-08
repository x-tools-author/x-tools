/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QVector>

#include "AbstractTransmitter.h"

class Communication;

namespace xTools {

class SerialPortTransmitter : public AbstractTransmitter
{
    Q_OBJECT
public:
    struct ItemContextKey
    {
        const QString enable{"enable"};
        const QString portName{"portName"};
        const QString baudRate{"baudRate"};
        const QString parity{"parity"};
        const QString stopBits{"stopBits"};
        const QString dataBits{"dataBits"};
        const QString flowControl{"flowControl"};
        const QString description{"description"};
    };

public:
    explicit SerialPortTransmitter(QObject *parent = nullptr);
    virtual void inputBytes(const QByteArray &bytes) override;

protected:
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;
    virtual Communication *createTool() override;
};

} // namespace xTools
