/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QPair>

#include "Communication.h"

namespace xTools {

class Socket : public Communication
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    ~Socket();

    QVariantMap save() const override;
    void load(const QVariantMap &data) override;

    void setDataChannel(int channel);
    void setParameters(const QVariantMap &parameters) override;

protected:
    quint16 m_clientPort{0};
    QString m_clientAddress{"0.0.0.0"};
    quint16 m_serverPort{0};
    QString m_serverAddress{"0.0.0.0"};

    int m_channel;
    bool m_authentication{false};
    QString m_username;
    QString m_password;

    struct SocketPrivateSignal
    {};

protected:
    QString makeFlag(const QString &address, quint16 port) const;
    QPair<QString, quint16> splitFlag(const QString &flag) const;
    bool isValidFlag(const QPair<QString, quint16> &pair) const;
};

} // namespace xTools
