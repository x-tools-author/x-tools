/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>

#include "devicepage/common/abstractmodelio.h"

class ResponderModel;
class Responder : public AbstractModelIO
{
    Q_OBJECT
public:
    explicit Responder(QObject *parent = Q_NULLPTR);

    QVariant tableModel() const override;
    QVariantMap saveItem(const int row) const override;
    void loadItem(const int row, const QVariantMap &item) override;
    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override;

private:
    ResponderModel *m_tableModel{nullptr};
};
