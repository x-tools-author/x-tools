/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Socket.h"

Socket::Socket(QObject *parent)
    : Device(parent)
{
    static bool registered = false;
    if (!registered) {
        registered = true;
        qRegisterMetaType<Socket::SocketPrivateSignal>("Socket::SocketPrivateSignal");
    }
}

Socket::~Socket() {}

void Socket::setParameters(const QVariantMap &parameters)
{
    Device::setParameters(parameters);

    m_clientPort = parameters.value("clientPort").toUInt();
    m_clientAddress = parameters.value("clientAddress").toString();
    m_serverPort = parameters.value("serverPort").toUInt();
    m_serverAddress = parameters.value("serverAddress").toString();
    m_channel = parameters.value("channel").toInt();

    m_channel = parameters.value("channel").toInt();
    m_authentication = parameters.value("authentication").toBool();
    m_username = parameters.value("username").toString();
    m_password = parameters.value("password").toString();
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
