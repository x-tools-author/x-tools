/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKMASKERTOOL_H
#define SAKMASKERTOOL_H

#include <atomic>
#include <QMutex>

#include "sakbasetool.h"

class SAKMaskerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKMaskerTool(QObject *parent = Q_NULLPTR);
    ~SAKMaskerTool() override;

    void inputBytes(const QByteArray &bytes) override;

    Q_INVOKABLE void setMaskCode(qint8 maskCode);

protected:
    void run() override;

private:
    std::atomic<quint8> m_mask;
    QList<QByteArray> m_inputBytesList;
    QMutex m_inputBytesListMutex;
};

#endif // SAKMASKERTOOL_H
