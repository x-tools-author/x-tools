/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "socketserver.h"

namespace xTools {

SocketServer::SocketServer(QObject *parent)
    : Socket(parent)
{}

QStringList SocketServer::clients() const
{
    m_clientsMutex.lock();
    QStringList clients = m_clients;
    m_clientsMutex.unlock();
    return clients;
}

QString SocketServer::currentClientFlag() const
{
    m_currentClientMutex.lock();
    QString currentClient = m_currentClientFlag;
    m_currentClientMutex.unlock();
    return currentClient;
}

void SocketServer::setCurrentClientFlag(const QString &flag)
{
    m_currentClientMutex.lock();
    m_currentClientFlag = flag;
    m_currentClientMutex.unlock();
}

void SocketServer::addClient(const QString &flag)
{
    bool changed = false;
    m_clientsMutex.lock();
    if (!m_clients.contains(flag)) {
        m_clients.append(flag);
        changed = true;
    }
    m_clientsMutex.unlock();

    if (changed) {
        emit clientsChanged();
    }
}

void SocketServer::removeClient(const QString &flag)
{
    bool changed = false;
    m_clientsMutex.lock();
    if (m_clients.contains(flag)) {
        m_clients.removeAll(flag);
        changed = true;
    }
    m_clientsMutex.unlock();

    if (changed) {
        emit clientsChanged();
    }
}

void SocketServer::clearClients()
{
    m_clientsMutex.lock();
    m_clients.clear();
    m_clientsMutex.unlock();
    emit clientsChanged();
}

} // namespace xTools
