/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakvelometertool.h"
#include "SAKVelometerToolUi.h"
#include "ui_SAKVelometerToolUi.h"

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

void SAKVelometerToolUi::onBaseToolUiInitialized(SAKBaseTool *tool,
                                                 const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)

    if (!tool) {
        qCWarning(mLoggingCategory) << "Parameter tool can not be nullptr!";
        return;
    }

    if (!tool->inherits("SAKVelometerTool")) {
        qCWarning(mLoggingCategory) << "Invalid tool, "
                                       "it must inherits SAKVelometerTool";
        return;
    }

    auto cookedTool = qobject_cast<SAKVelometerTool*>(tool);
    if (!cookedTool) {
        qCWarning(mLoggingCategory) << "Invalid tool!";
        return;
    }

    connect(cookedTool, &SAKVelometerTool::velocityChanged,
            this, [=](const QString &v){
        ui->labelVelocity->setText(v);
    });
}
