/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKWEBSOCKETCLIENTTOOL_H
#define SAKWEBSOCKETCLIENTTOOL_H

#include <QWebSocket>

#include "saksocketclienttool.h"

class SAKWebSocketClientTool : public SAKSocketClientTool
{
    Q_OBJECT
public:
    explicit SAKWebSocketClientTool(QObject *parent = Q_NULLPTR);

protected:
    virtual bool initialize(QString &errStr) override;
    virtual void writeBytes(const QByteArray &bytes) override;
    virtual void uninitialize() final;

private:
    QWebSocket *m_webSocket{Q_NULLPTR};
    QString m_peerInfo;
};

#endif // SAKWEBSOCKETCLIENTTOOL_H
