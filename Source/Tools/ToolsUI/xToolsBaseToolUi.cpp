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

void xToolsBaseToolUi::initialize(xToolsBaseTool *tool, const QString &settingsGroup)
{
    m_tool = tool;
    if (!tool) {
        qWarning() << "The value of tool is nullptr!";
        return;
    }

    connect(tool, &xToolsBaseTool::isWorkingChanged, this, [=]() {
        onIsWorkingChanged(tool->isWorking());
    });

    onBaseToolUiInitialized(tool, settingsGroup);
}

void xToolsBaseToolUi::onIsWorkingChanged(bool isWorking)
{
    Q_UNUSED(isWorking)
}

void xToolsBaseToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(tool)
    Q_UNUSED(settingsGroup)
}
