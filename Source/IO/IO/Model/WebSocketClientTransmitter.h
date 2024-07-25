/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "SocketClientTransmitter.h"

class Communication;

namespace xTools {

class WebSocketClientTransmitter : public SocketClientTransmitter
{
    Q_OBJECT
public:
    explicit WebSocketClientTransmitter(QObject *parent = nullptr);
    virtual Communication *createTool() override;
};

} // namespace xTools
