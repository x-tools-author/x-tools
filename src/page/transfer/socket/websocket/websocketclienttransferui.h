/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "sockettransferui.h"

class WebSocketClientTransferUi : public SocketTransferUi
{
    Q_OBJECT
public:
    WebSocketClientTransferUi(QWidget *parent = nullptr);
    ~WebSocketClientTransferUi() override;

    void setupIO(AbstractIO *io) override;
};
