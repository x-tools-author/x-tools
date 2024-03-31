/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsTransmitterToolUi.h"

#include <QDebug>

#include "xToolsTransmitterTool.h"

xToolsTransmitterToolUi::xToolsTransmitterToolUi(QWidget *parent)
    : xToolsTableModelToolUi{parent}
{}

void xToolsTransmitterToolUi::afterRowEdited(int row)
{
    xToolsTransmitterTool *cookedTool = Q_NULLPTR;
    cookedTool = qobject_cast<xToolsTransmitterTool *>(m_tool);
    if (!cookedTool) {
        return;
    }

    xToolsCommunicationTool *tool = cookedTool->communicationTool(row);
    if (tool) {
        tool->exit();
        tool->wait();
        tool->start();
        qInfo() << "parameters changed, restart the thread";
    }
}
