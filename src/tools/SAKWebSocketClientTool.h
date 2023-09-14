/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETCLIENTTOOL_H
#define SAKWEBSOCKETCLIENTTOOL_H

#include <QWebSocket>
#include "SAKSocketClientTool.h"


class SAKWebSocketClientTool : public SAKSocketClientTool
{
    Q_OBJECT
public:
    explicit SAKWebSocketClientTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void uninitialize() final;

private:
    QWebSocket *mWebSocket{nullptr};
    QString mPeerInfo;
};

#endif // SAKWEBSOCKETCLIENTTOOL_H
