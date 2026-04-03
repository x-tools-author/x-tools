/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "x/common/xpage.h"

namespace xTools {

class xToolsPrivate;
class xTools : public xPage
{
    Q_OBJECT
    xToolsPrivate *d{nullptr};

public:
    enum class WindowGrid { Grid1x1, Grid1x2, Grid2x1, Grid2x2 };

public:
    xTools(QWidget *parent = nullptr);
    ~xTools();

    QJsonObject save() const override;
    void load(const QJsonObject &obj) override;
    QMenu *toolButtonMenu() override;

    int windowGrid() const;
    void setWindowGrid(int grid);
};

}; // namespace xTools