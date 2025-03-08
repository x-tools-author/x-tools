/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/transfer/socket/sockettransfermodel.h"

class WsClientTransferModel : public SocketTransferModel
{
    Q_OBJECT
public:
    WsClientTransferModel(QObject *parent = nullptr);
    ~WsClientTransferModel() override;

protected:
    Socket *createSocket() override;
    bool isEnableRestartColumn(int column) const override;
};
