/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "panel.h"

namespace Ui {
class LuaPanel;
}

class LuaPanel : public Panel
{
    Q_OBJECT
public:
    explicit LuaPanel(QWidget *parent = nullptr);
    ~LuaPanel() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

private:
    Ui::LuaPanel *ui;
};