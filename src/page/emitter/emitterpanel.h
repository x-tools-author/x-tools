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

class EmitterView;
class EmitterPanelPrivate;
class EmitterPanel : public Panel
{
    Q_OBJECT
    EmitterPanelPrivate *d;

public:
    explicit EmitterPanel(QWidget *parent = nullptr);
    ~EmitterPanel();

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

    EmitterView *emitterView() const;
};
