/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>

namespace xTools {

class Menu : public QMenu
{
public:
    Menu(const QString &title, QWidget *parent = nullptr);
    Menu(QWidget *parent = nullptr);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
};

} // namespace xTools
