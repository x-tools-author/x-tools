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

#include "xToolsCommunicationTool.h"
#include "xToolsTableModelTool.h"

class xToolsTransmitterTool : public xToolsTableModelTool
{
    Q_OBJECT
public:
    explicit xToolsTransmitterTool(QObject *parent = nullptr);
    xToolsCommunicationTool *communicationTool(int index);

    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override { exec(); }

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual xToolsCommunicationTool *createTool() = 0;

protected:
    QVector<xToolsCommunicationTool *> m_tools;
    QMutex m_toolsMutex;

private:
    void onDataChanged(const QModelIndex &topLeft,
                       const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int>());
};
