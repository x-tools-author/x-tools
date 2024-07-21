/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSocketTool.h"

xToolsSocketTool::xToolsSocketTool(QObject *parent)
    : xToolsBaseTool(parent)
{

}

xToolsSocketTool::~xToolsSocketTool() {}

QVariantMap xToolsSocketTool::save() const
{
    QVariantMap baseData = xToolsBaseTool::save();
    QVariantMap data;
    data.insert("baseData", baseData);
    data.insert("clientPort", m_clientPort);
    data.insert("clientAddress", m_clientAddress);
    data.insert("serverPort", m_serverPort);
    data.insert("serverAddress", m_serverAddress);
    data.insert("channel", m_channel);
    data.insert("authentication", m_authentication);
    data.insert("username", m_username);
    data.insert("password", m_password);

    return data;
}

void xToolsSocketTool::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    xToolsBaseTool::load(parameters.value("baseData").toMap());

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

QString xToolsSocketTool::makeFlag(const QString &address, quint16 port) const
{
    return QString("%1:%2").arg(address).arg(port);
}

QPair<QString, quint16> xToolsSocketTool::splitFlag(const QString &flag) const
{
    QStringList list = flag.split(":", Qt::SkipEmptyParts);
    if (list.size() == 2) {
        return qMakePair(list.at(0), list.at(1).toUShort());
    }

    return qMakePair(QString(), 0);
}

bool xToolsSocketTool::isValidFlag(const QPair<QString, quint16> &pair) const
{
    if (pair.first.isEmpty() || pair.second == 0) {
        return false;
    }

    return true;
}
