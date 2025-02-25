/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "sockettransfer.h"

namespace xTools {

class WebSocketServerTransferModel;
class WebSocketServerTransfer : public SocketTransfer
{
    Q_OBJECT
public:
    WebSocketServerTransfer(QObject *parent = nullptr);
    virtual ~WebSocketServerTransfer();

    QVariant tableModel() const override;

protected:
    WebSocketServerTransferModel *m_model;
};

} // namespace xTools
