/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>
#include <QMutex>

#include "xToolsBaseTool.h"

class xToolsMaskerTool : public xToolsBaseTool
{
    Q_OBJECT
public:
    explicit xToolsMaskerTool(QObject *parent = Q_NULLPTR);
    ~xToolsMaskerTool() override;

    void inputBytes(const QByteArray &bytes) override;

    Q_INVOKABLE void setMaskCode(qint8 maskCode);

protected:
    void run() override;

private:
    std::atomic<quint8> m_mask;
    QList<QByteArray> m_inputBytesList;
    QMutex m_inputBytesListMutex;
};
