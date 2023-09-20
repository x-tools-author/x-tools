/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKTRANSMITTERTOOL_H
#define SAKTRANSMITTERTOOL_H

#include <QMutex>

#include "saktablemodeltool.h"
#include "sakcommunicationtool.h"

class SAKTransmitterTool : public SAKTableModelTool
{
    Q_OBJECT
public:
    explicit SAKTransmitterTool(const char *logCategory,
                                QObject *parent = nullptr);
    SAKCommunicationTool *communicationTool(int index);

protected:
    virtual void run() override {exec();}

protected:
    virtual int rowCount(const QModelIndex &parent
                         = QModelIndex()) const override;
    virtual bool removeRows(int row,
                            int count,
                            const QModelIndex &parent
                            = QModelIndex()) override;
    virtual bool insertRows(int row,
                            int count,
                            const QModelIndex &parent
                            = QModelIndex()) override;
    virtual SAKCommunicationTool *createTool() = 0;

protected:
    QVector<SAKCommunicationTool*> mToolVector;
    QMutex mToolVectorMutex;

private:
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int>());
};

#endif // SAKTRANSMITTERTOOL_H
