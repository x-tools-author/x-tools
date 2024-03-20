/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QByteArray>
#include <QThread>

#include <qmdnsengine/browser.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

class xToolsMdnsServer : public QThread
{
    Q_OBJECT
public:
    explicit xToolsMdnsServer(QObject *parent = nullptr);
    ~xToolsMdnsServer();

    void setServiceType(const QByteArray &serviceType);

signals:
    void serviceAdded(const QMdnsEngine::Service &service);
    void serviceUpdated(const QMdnsEngine::Service &service);
    void serviceRemoved(const QMdnsEngine::Service &service);

protected:
    void run() override;

private:
    QByteArray m_serviceType;
};
