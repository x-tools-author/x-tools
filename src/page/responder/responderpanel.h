/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/common/panel.h"

class ResponderView;
class ResponderPanelPrivate;
class ResponderPanel : public Panel
{
    Q_OBJECT
    ResponderPanelPrivate *d{nullptr};

public:
    explicit ResponderPanel(QWidget *parent = nullptr);
    ~ResponderPanel() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

    void onBytesRead(const QByteArray &bytes, const QString &flag) override;
};