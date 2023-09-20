/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKSERIALPORTTRANSMITTERTOOL_HH
#define SAKSERIALPORTTRANSMITTERTOOL_HH

#include <QMutex>
#include <QVector>

#include "SAKTransmitterTool.h"

class SAKSerialPortTransmitterTool : public SAKTransmitterTool
{
    Q_OBJECT
public:
    struct ItemContextKey {
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
    explicit SAKSerialPortTransmitterTool(QObject *parent = nullptr);
    virtual QString cookHeaderString(const QString &str) override;
    Q_INVOKABLE virtual QVariant itemContext(int index) override;
    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) override;

protected:
    virtual int columnCount(const QModelIndex &parent
                            = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;
    virtual SAKCommunicationTool *createTool() override;
};

#endif // SAKSERIALPORTTRANSMITTERTOOL_HH
