/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseTool.h"

#include <QMutex>

class xToolsCommunicationTool : public xToolsBaseTool
{
    Q_OBJECT
public:
    explicit xToolsCommunicationTool(QObject *parent = nullptr);
    ~xToolsCommunicationTool() override;
    Q_INVOKABLE void inputBytes(const QByteArray &bytes) override;

signals:
    void bytesRead(const QByteArray &bytes, const QString &from);
    void bytesWritten(const QByteArray &bytes, const QString &to);

protected:
    void run() override;

    virtual bool initialize(QString &errStr) = 0;
    virtual void writeBytes(const QByteArray &bytes) = 0;
    virtual void uninitialize() = 0;

private:
    QList<QByteArray> m_inputBytesList;
    QMutex m_inputBytesMutex;
};
