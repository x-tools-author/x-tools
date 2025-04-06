/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QLocalServer>
#include <QLocalSocket>

#include "device.h"

struct LocalServerParametersKeys
{
    const QString serverName{"serverName"};
};

class LocalServer : public Device
{
    Q_OBJECT
public:
    explicit LocalServer(QObject *parent = nullptr);
    ~LocalServer() override;
    void setTarget(QLocalSocket *socket);

    QObject *initDevice() override;
    void deinitDevice() override;

signals:
    void socketConnected(QLocalSocket *socket, const QString &socketName);
    void socketDisconnected(QLocalSocket *socket);

protected:
    void writeActually(const QByteArray &bytes);

private:
    QString getClientName(QLocalSocket *socket);

private:
    QLocalServer *m_server{nullptr};
    QLocalSocket *m_target{nullptr};
};