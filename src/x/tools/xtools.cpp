/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xtools.h"

namespace xTools {

xTools::xTools(QWidget *parent)
    : xPage(parent)
{}

xTools::~xTools() {}

QJsonObject xTools::save() const
{
    QJsonObject json = xPage::save();
    return json;
}

void xTools::load(const QJsonObject &json)
{
    xPage::load(json);
}

} // namespace xTools