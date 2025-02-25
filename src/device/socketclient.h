/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "socket.h"

namespace xTools {

class SocketClient : public Socket
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient() override;
};

} // namespace xTools
