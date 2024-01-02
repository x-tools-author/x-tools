/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTRANSMITTERTOOL_H
#define SAKTRANSMITTERTOOL_H

#include <QMutex>

#include "sakcommunicationtool.h"
#include "saktablemodeltool.h"

class SAKTransmitterTool : public SAKTableModelTool
{
    Q_OBJECT
public:
    explicit SAKTransmitterTool(QObject *parent = nullptr);
    SAKCommunicationTool *communicationTool(int index);

    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override { exec(); }

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual SAKCommunicationTool *createTool() = 0;

protected:
    QVector<SAKCommunicationTool *> m_tools;
    QMutex m_toolsMutex;

private:
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int>());
};

#endif // SAKTRANSMITTERTOOL_H
