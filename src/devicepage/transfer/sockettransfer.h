/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "abstracttransfer.h"

namespace xTools {

class SocketTransfer : public AbstractTransfer
{
public:
    SocketTransfer(QObject *parent = nullptr);
    virtual ~SocketTransfer();

    QVariantMap saveItem(const int row) const override;
    void loadItem(const int row, const QVariantMap &item) override;
};

} // namespace xTools
