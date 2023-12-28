/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKMDNSSERVER_H
#define SAKMDNSSERVER_H

#include <QByteArray>
#include <QThread>

#include <qmdnsengine/browser.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

class SAKMdnsServer : public QThread
{
    Q_OBJECT
public:
    explicit SAKMdnsServer(QObject *parent = nullptr);
    ~SAKMdnsServer();

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

#endif // SAKMDNSSERVER_H