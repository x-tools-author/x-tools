/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakvelometertoolui.h"
#include "xToolsVelometerTool.h"
#include "ui_sakvelometertoolui.h"

SAKVelometerToolUi::SAKVelometerToolUi(QWidget *parent)
    : SAKBaseToolUi(parent)
    , ui(new Ui::SAKVelometerToolUi)
{
    ui->setupUi(this);
}

SAKVelometerToolUi::~SAKVelometerToolUi()
{
    delete ui;
}

void SAKVelometerToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)

    if (!tool) {
        qWarning() << "Parameter tool can not be nullptr!";
        return;
    }

    if (!tool->inherits("SAKVelometerTool")) {
        qWarning() << "Invalid tool, "
                                       "it must inherits SAKVelometerTool";
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
