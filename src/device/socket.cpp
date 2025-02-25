/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "socket.h"

#include "IO/xIO.h"

namespace xTools {

Socket::Socket(QObject *parent)
    : Communication(parent)
{
    static bool registered = false;
    if (!registered) {
        registered = true;
        qRegisterMetaType<Socket::SocketPrivateSignal>("Socket::SocketPrivateSignal");
    }

    m_channel = static_cast<int>(xIO::WebSocketDataChannel::Text);

    xIO::SocketItemKeys keys;
    QVariantMap tmp;
    tmp.insert(keys.clientPort, m_clientPort);
    tmp.insert(keys.clientAddress, m_clientAddress);
    tmp.insert(keys.serverPort, m_serverPort);
    tmp.insert(keys.serverAddress, m_serverAddress);
    tmp.insert(keys.dataChannel, m_channel);
    tmp.insert(keys.authentication, m_authentication);
    tmp.insert(keys.username, m_username);
    tmp.insert(keys.password, m_password);
    tmp.insert(keys.multicastAddress, m_multicastAddress);
    tmp.insert(keys.multicastPort, m_multicastPort);
    tmp.insert(keys.enableMulticast, m_enableMulticast);
    tmp.insert(keys.justMulticast, m_justMulticast);

    Socket::load(tmp);
}

void Socket::load(const QVariantMap &parameters)
{
    Communication::load(parameters);

    xIO::SocketItem item = xIO::loadSocketItem(QJsonObject::fromVariantMap(parameters));
    m_clientPort = item.clientPort;
    m_clientAddress = item.clientAddress;
    m_serverPort = item.serverPort;
    m_serverAddress = item.serverAddress;
    m_channel = static_cast<int>(item.dataChannel);
    m_authentication = item.authentication;
    m_username = item.username;
    m_password = item.password;
    m_multicastAddress = item.multicastAddress;
    m_multicastPort = item.multicastPort;
    m_enableMulticast = item.enableMulticast;
    m_justMulticast = item.justMulticast;
}

void Socket::setDataChannel(int channel)
{
    m_channel = channel;
}

QString Socket::makeFlag(const QString &address, quint16 port) const
{
    return QString("%1:%2").arg(address).arg(port);
}

QPair<QString, quint16> Socket::splitFlag(const QString &flag) const
{
    QStringList list = flag.split(":", Qt::SkipEmptyParts);
    if (list.size() == 2) {
        return qMakePair(list.at(0), list.at(1).toUShort());
    }

    return qMakePair(QString(), 0);
}

bool Socket::isValidFlag(const QPair<QString, quint16> &pair) const
{
    if (pair.first.isEmpty() || pair.second == 0) {
        return false;
    }

    return true;
}

} // namespace xTools
