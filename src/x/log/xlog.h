/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "x/xpage.h"

namespace xLog {

class xLog : public xPage
{
    Q_OBJECT
public:
    explicit xLog(QWidget* parent = nullptr);
    ~xLog() override;

    QJsonObject save() const override;
    void load(const QJsonObject& obj) override;
};

} // namespace xLog