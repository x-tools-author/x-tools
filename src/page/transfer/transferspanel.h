/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/common/panel.h"

class TransfersPanelPrivate;
class TransfersPanel : public Panel
{
    Q_OBJECT
    TransfersPanelPrivate *d{nullptr};

public:
    explicit TransfersPanel(QWidget *parent = nullptr);
    ~TransfersPanel() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void onBytesRead(const QByteArray &bytes, const QString &flag) override;

signals:
    void bytesRead(const QByteArray &bytes, const QString &flag);
    void bytesWritten(const QByteArray &bytes, const QString &flag);
};