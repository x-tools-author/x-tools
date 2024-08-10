/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "SocketTransferModel.h"

namespace xTools {

class WebSocketServerTransferModel : public SocketTransferModel
{
    Q_OBJECT
public:
    WebSocketServerTransferModel(QObject *parent = nullptr);
    ~WebSocketServerTransferModel() override;

protected:
    Socket *createSocket() override;
};

} // namespace xTools
