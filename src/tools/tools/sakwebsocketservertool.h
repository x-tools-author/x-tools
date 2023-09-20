/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKWEBSOCKETSERVERTOOL_H
#define SAKWEBSOCKETSERVERTOOL_H

#include <QWebSocketServer>
#include "saksocketservertool.h"

class SAKWebSocketServerTool : public SAKSocketServerTool
{
    Q_OBJECT
public:
    explicit SAKWebSocketServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void writeBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;
    virtual void uninitialize() final;

private:
    void writeBytesInner(QWebSocket *client, const QByteArray &bytes,
                         const QVariant &context = QJsonObject());

private:
    QWebSocketServer *mWebSocketServer{nullptr};
    QList<QWebSocket*> mWebSocketList;
};

#endif // SAKWEBSOCKETSERVERTOOL_H
