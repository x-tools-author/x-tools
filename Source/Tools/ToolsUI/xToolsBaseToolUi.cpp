/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBaseToolUi.h"
#include "xToolsBaseTool.h"

#include <QDebug>

xToolsBaseToolUi::xToolsBaseToolUi(QWidget *parent)
    : QWidget{parent}
{}

void xToolsBaseToolUi::setupTool(xToolsBaseTool *tool)
{
    if (!tool) {
        return;
    }

    connect(tool, &xToolsBaseTool::isWorkingChanged, this, [this]() { setEnabled(false); });
}

QVariantMap xToolsBaseToolUi::save() const
{
    return QVariantMap();
}

void xToolsBaseToolUi::load(const QVariantMap &data)
{
    Q_UNUSED(data);
}
