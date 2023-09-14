/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKBaseTool.h"
#include "SAKBaseToolUi.h"

SAKBaseToolUi::SAKBaseToolUi(QWidget *parent)
    : QWidget{parent}
{

}

void SAKBaseToolUi::initialize(SAKBaseTool *tool,
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

    connect(tool, &SAKBaseTool::isWorkingChanged, this, [=](){
        onIsWorkingChanged(tool->isWorking());
    });

    onBaseToolUiInitialized(tool, settingsGroup);
}

void SAKBaseToolUi::onIsWorkingChanged(bool isWorking)
{
    Q_UNUSED(isWorking)
}

void SAKBaseToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                             const QString &settingsGroup)
{
    Q_UNUSED(tool)
    Q_UNUSED(settingsGroup)
}
