/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QLocalSocket>

#include "device.h"

struct LocalSocketParametersKeys
{
    const QString serverName{"serverName"};
};

class LocalSocket : public Device
{
    Q_OBJECT
public:
    explicit LocalSocket(QObject *parent = nullptr);
    ~LocalSocket() override;

    QObject *initDevice() override;
    void deinitDevice() override;

private:
    QLocalSocket *m_socket{nullptr};

protected:
    void writeActually(const QByteArray &bytes) override;
};