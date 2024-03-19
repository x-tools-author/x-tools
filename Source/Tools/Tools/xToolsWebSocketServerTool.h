/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKWEBSOCKETSERVERTOOL_H
#define SAKWEBSOCKETSERVERTOOL_H

#include "saksocketservertool.h"
#include <QWebSocketServer>

class SAKWebSocketServerTool : public SAKSocketServerTool
{
    Q_OBJECT
public:
    explicit SAKWebSocketServerTool(QObject *parent = nullptr);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void writeBytes(const QByteArray &bytes) override;
    virtual void uninitialize() final;

private:
    void writeBytesInner(QWebSocket *client, const QByteArray &bytes);

private:
    QWebSocketServer *mWebSocketServer{nullptr};
    QList<QWebSocket *> mWebSocketList;
};

#endif // SAKWEBSOCKETSERVERTOOL_H
