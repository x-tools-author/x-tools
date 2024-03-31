/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsVelometerToolUi.h"
#include "ui_xToolsVelometerToolUi.h"

#include <QDebug>

#include "xToolsVelometerTool.h"

xToolsVelometerToolUi::xToolsVelometerToolUi(QWidget *parent)
    : xToolsBaseToolUi(parent)
    , ui(new Ui::xToolsVelometerToolUi)
{
    ui->setupUi(this);
}

xToolsVelometerToolUi::~xToolsVelometerToolUi()
{
    delete ui;
}

void xToolsVelometerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)

    if (!tool) {
        qWarning() << "Parameter tool can not be nullptr!";
        return;
    }

    if (!tool->inherits("xToolsVelometerTool")) {
        qWarning() << "Invalid tool, it must inherits xToolsVelometerTool";
        return;
    }

    auto cookedTool = qobject_cast<xToolsVelometerTool *>(tool);
    if (!cookedTool) {
        qWarning() << "Invalid tool!";
        return;
    }

    connect(cookedTool, &xToolsVelometerTool::velocityChanged, this, [=](const QString &v) {
        ui->labelVelocity->setText(v);
    });
}
