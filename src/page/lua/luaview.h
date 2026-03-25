/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class LuaView;
}

class LuaView : public QWidget
{
    Q_OBJECT
public:
    explicit LuaView(QWidget *parent = nullptr);
    ~LuaView() override;

private:
    Ui::LuaView *ui;
};