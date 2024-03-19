/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakstatisticiantoolui.h"
#include "xToolsStatisticianTool.h"
#include "ui_sakstatisticiantoolui.h"

SAKStatisticianToolUi::SAKStatisticianToolUi(QWidget *parent)
    : SAKBaseToolUi{parent}
    , ui(new Ui::SAKStatisticianToolUi)
{
    ui->setupUi(this);
}

SAKStatisticianToolUi::~SAKStatisticianToolUi()
{
    delete ui;
}

void SAKStatisticianToolUi::onBaseToolUiInitialized(xToolsBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)

    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKStatisticianTool")) {
        return;
    }

    auto cookedTool = qobject_cast<xToolsStatisticianTool *>(tool);
    if (!cookedTool) {
        return;
    }

    connect(cookedTool, &xToolsStatisticianTool::bytesChanged, this, [=](int bytes) {
        ui->labelBytes->setText(QString::number(bytes));
    });
    connect(cookedTool, &xToolsStatisticianTool::framesChanged, this, [=](int frames) {
        ui->labelFrames->setText(QString::number(frames));
    });
    connect(cookedTool, &xToolsStatisticianTool::started, this, [=]() {
        ui->labelBytes->setText(QString::number(0));
        ui->labelFrames->setText(QString::number(0));
    });
}
