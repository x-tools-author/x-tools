/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "page/common/panel.h"

namespace xTools {
class SyntaxHighlighterLua;
}

class LuaRunner;
class LuaPanelPrivate;
class LuaPanel : public Panel
{
    Q_OBJECT
    LuaPanelPrivate *d{nullptr};

public:
    explicit LuaPanel(QWidget *parent = nullptr);
    ~LuaPanel() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

    QByteArray handleData(const QByteArray &data) const;
    bool isBypassed() const;
};
