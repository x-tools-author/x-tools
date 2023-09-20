/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "SAKStatisticianTool.h"
#include "SAKStatisticianToolUi.h"
#include "ui_SAKStatisticianToolUi.h"

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

void SAKStatisticianToolUi::onBaseToolUiInitialized(
    SAKBaseTool *tool, const QString &settingsGroup)
{
    Q_UNUSED(settingsGroup)

    if (!tool) {
        return;
    }

    if (!tool->inherits("SAKStatisticianTool")) {
        return;
    }

    auto cookedTool = qobject_cast<SAKStatisticianTool*>(tool);
    if (!cookedTool) {
        return;
    }

    connect(cookedTool, &SAKStatisticianTool::bytesChanged,
            this, [=](int bytes){
        ui->labelBytes->setText(QString::number(bytes));
    });
    connect(cookedTool, &SAKStatisticianTool::framesChanged,
            this, [=](int frames){
        ui->labelFrames->setText(QString::number(frames));
    });
    connect(cookedTool, &SAKStatisticianTool::started, this, [=](){
        ui->labelBytes->setText(QString::number(0));
        ui->labelFrames->setText(QString::number(0));
    });
}
