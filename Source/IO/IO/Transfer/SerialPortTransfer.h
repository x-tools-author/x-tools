/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "AbstractTransfer.h"

namespace xTools {

class SerialPortTransferModel;
class SerialPortTransfer : public AbstractTransfer
{
    Q_OBJECT
public:
    SerialPortTransfer(QObject *parent = nullptr);
    ~SerialPortTransfer() override;

    void inputBytes(const QByteArray &bytes) override;

    QVariant tableModel() override;
    virtual QVariantMap saveItem(const int row) const override;
    virtual void loadItem(const int row, const QVariantMap &item) override;

protected:
    SerialPortTransferModel *m_model;
};

} // namespace xTools
