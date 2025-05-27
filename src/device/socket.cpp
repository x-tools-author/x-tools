/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "socket.h"

#include "common/xtools.h"

Socket::Socket(QObject *parent)
    : Device(parent)
{
    Socket::load(saveSocketItem(defaultSocketItem()));
}

void Socket::load(const QVariantMap &parameters)
{
    Device::load(parameters);

    SocketItem item = loadSocketItem(parameters);
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
    QVariantMap tmp = save();
    tmp.insert(SocketItemKeys().dataChannel, channel);
    load(tmp);
    m_channel = channel;
}

QString Socket::makeFlag(const QString &address, quint16 port) const
{
    return QString("%1:%2").arg(address).arg(port);
}

QPair<QString, quint16> Socket::splitFlag(const QString &flag) const
{
    QStringList list = flag.split(":", xSkipEmptyParts);
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
