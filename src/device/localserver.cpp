/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "localserver.h"

#include <QDateTime>
#include <QLocalSocket>

LocalServer::LocalServer(QObject *parent)
    : Device(parent)
{}

LocalServer::~LocalServer() {}

void LocalServer::setTarget(QLocalSocket *socket)
{
    m_target = socket;
}

QObject *LocalServer::initDevice()
{
    m_server = new QLocalServer(this);
    connect(m_server, &QLocalServer::newConnection, this, [this]() {
        QLocalSocket *socket = m_server->nextPendingConnection();
        emit this->socketConnected(socket, getClientName(socket));
        connect(socket, &QLocalSocket::readyRead, this, [this, socket]() {
            QByteArray bytes = socket->readAll();
            emit bytesRead(bytes, getClientName(socket));
        });
        connect(socket, &QLocalSocket::disconnected, this, [this, socket]() {
            emit this->socketDisconnected(socket);
        });
    });

    QVariantMap parameters = save();
    LocalServerParametersKeys keys;
    QString serverName = parameters.value(keys.serverName, QString("xToolsLocalSocket")).toString();
    if (!m_server->listen(serverName)) {
        qWarning() << "Failed to start local server:" << m_server->errorString();
        m_server->deleteLater();
        return nullptr;
    }

    return m_server;
}

void LocalServer::deinitDevice()
{
    if (m_server) {
        m_server->close();
        m_server->deleteLater();
        m_server = nullptr;
    }
}

void LocalServer::writeActually(const QByteArray &bytes)
{
    if (!m_server) {
        return;
    }

    if (m_target == nullptr) {
        for (QLocalSocket *socket : m_server->findChildren<QLocalSocket *>()) {
            socket->write(bytes);
        }
    } else {
        for (QLocalSocket *socket : m_server->findChildren<QLocalSocket *>()) {
            if (socket == m_target) {
                socket->write(bytes);
                break;
            }
        }
    }
}

QString LocalServer::getClientName(QLocalSocket *socket)
{
    if (socket->objectName().isEmpty()) {
        QString dt = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        socket->setObjectName(dt);
    }

    return tr("Client") + QString("[%1]").arg(socket->objectName());
}