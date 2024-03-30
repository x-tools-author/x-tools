/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWebSocket>

#include "xToolsSocketClientTool.h"

class xToolsWebSocketClientTool : public xToolsSocketClientTool
{
    Q_OBJECT
public:
    explicit xToolsWebSocketClientTool(QObject *parent = Q_NULLPTR);

protected:
    virtual bool initialize(QString &errStr) override;
    virtual void writeBytes(const QByteArray &bytes) override;
    virtual void uninitialize() final;

private:
    QWebSocket *m_webSocket{Q_NULLPTR};
};
