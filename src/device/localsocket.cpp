/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "localsocket.h"

#include "common/xtools.h"
#include "utilities/compatibility.h"

LocalSocket::LocalSocket(QObject *parent)
    : Device(parent)
{}

LocalSocket::~LocalSocket() {}

QObject *LocalSocket::initDevice()
{
    m_socket = new QLocalSocket();
    connect(m_socket, &QLocalSocket::connected, this, &LocalSocket::opened);
    connect(m_socket, &QLocalSocket::disconnected, this, &LocalSocket::closed);
    connect(m_socket, &QLocalSocket::readyRead, this, [this]() {
        QByteArray bytes = this->m_socket->readAll();
        emit bytesRead(bytes, m_socket->serverName());
    });
    connect(m_socket, xLocalSocketErrorOccurred, this, [this]() {
        emit errorOccurred(m_socket->errorString());
    });

    QVariantMap parameters = save();
    LocalSocketParametersKeys keys;
    QString defaultServerName = QString("xtools.local.socket");
    QString socketName = parameters.value(keys.serverName, defaultServerName).toString();
    m_socket->connectToServer(socketName);
    return m_socket;
}

void LocalSocket::deinitDevice()
{
    if (m_socket) {
        m_socket->disconnectFromServer();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

void LocalSocket::writeActually(const QByteArray &bytes)
{
    if (m_socket && m_socket->state() == QLocalSocket::ConnectedState) {
        if (m_socket->write(bytes) > 0) {
            emit bytesWritten(bytes, m_socket->serverName());
        }
    } else {
        emit errorOccurred("Socket is not connected");
    }
}
