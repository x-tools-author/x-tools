/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "SocketTransfer.h"

namespace xTools {

class WebSocketClientTransferModel;
class WebSocketClientTransfer : public SocketTransfer
{
    Q_OBJECT
public:
    WebSocketClientTransfer(QObject *parent = nullptr);
    virtual ~WebSocketClientTransfer();

    QVariant tableModel() const override;

protected:
    WebSocketClientTransferModel *m_model;
};

} // namespace xTools
