/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsMdnsServer.h"

#include <QDebug>

xToolsMdnsServer::xToolsMdnsServer(QObject *parent)
    : QThread(parent)
{}

xToolsMdnsServer::~xToolsMdnsServer()
{
    exit();
    wait();
}

void xToolsMdnsServer::setServiceType(const QByteArray &serviceType)
{
    m_serviceType = serviceType;
}

void xToolsMdnsServer::run()
{
    QMdnsEngine::Server server;
    QMdnsEngine::Browser browser(&server, m_serviceType);
    connect(&browser, &QMdnsEngine::Browser::serviceAdded, this, &xToolsMdnsServer::serviceAdded);
    connect(&browser, &QMdnsEngine::Browser::serviceUpdated, this, &xToolsMdnsServer::serviceUpdated);
    connect(&browser, &QMdnsEngine::Browser::serviceRemoved, this, &xToolsMdnsServer::serviceRemoved);

    qInfo() << "The mDNS broser has been started.";
    exec();
    qInfo() << "The mDNS broser has been stopped.";
}
