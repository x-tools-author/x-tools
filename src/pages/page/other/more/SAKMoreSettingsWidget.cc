/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKDebugPage.hh"
#include "SAKMoreSettingsWidget.hh"

#include "ui_SAKMoreSettingsWidget.h"

SAKMoreSettingsWidget::SAKMoreSettingsWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,_debugPage (debugPage)
    ,ui (new Ui::SAKMoreSettingsWidget)
{
    ui->setupUi(this);
}

SAKMoreSettingsWidget::~SAKMoreSettingsWidget()
{
    delete ui;
}
