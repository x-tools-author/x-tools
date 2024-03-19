/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakbasetoolui.h"
#include "xToolsBaseTool.h"

SAKBaseToolUi::SAKBaseToolUi(QWidget *parent)
    : QWidget{parent}
{}

void SAKBaseToolUi::initialize(xToolsBaseTool *tool,
                               const QString &settingsGroup,
                               const char *loggingCategory)
{
    mTool = tool;
    if (mLoggingCategory) {
        delete mLoggingCategory;
        mLoggingCategory = nullptr;
    }

    mLoggingCategory = new QLoggingCategory(loggingCategory);

    if (!tool) {
        qCWarning((*mLoggingCategory)) << "The value of tool is nullptr!";
        return;
    }

    connect(tool, &xToolsBaseTool::isWorkingChanged, this, [=]() {
        onIsWorkingChanged(tool->isWorking());
    });

    onBaseToolUiInitialized(tool, settingsGroup);
}

void SAKBaseToolUi::onIsWorkingChanged(bool isWorking)
{
    Q_UNUSED(isWorking)
}

void SAKBaseToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(tool)
    Q_UNUSED(settingsGroup)
}
