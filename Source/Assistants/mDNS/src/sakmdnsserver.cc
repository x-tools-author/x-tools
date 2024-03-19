/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakmdnsserver.h"

#include <QDebug>

SAKMdnsServer::SAKMdnsServer(QObject *parent)
    : QThread(parent)
{}

SAKMdnsServer::~SAKMdnsServer()
{
    exit();
    wait();
}

void SAKMdnsServer::setServiceType(const QByteArray &serviceType)
{
    m_serviceType = serviceType;
}

void SAKMdnsServer::run()
{
    QMdnsEngine::Server server;
    QMdnsEngine::Browser browser(&server, m_serviceType);
    connect(&browser, &QMdnsEngine::Browser::serviceAdded, this, &SAKMdnsServer::serviceAdded);
    connect(&browser, &QMdnsEngine::Browser::serviceUpdated, this, &SAKMdnsServer::serviceUpdated);
    connect(&browser, &QMdnsEngine::Browser::serviceRemoved, this, &SAKMdnsServer::serviceRemoved);

    qInfo() << "mDNS broser has been started.";
    exec();
    qInfo() << "mDNS broser has been stopped.";
}