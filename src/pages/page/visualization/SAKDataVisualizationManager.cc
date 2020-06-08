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
#include "SAKThroughputWidget.hh"
#include "SAKDataVisualizationManager.hh"

#include "ui_SAKDataVisualizationManager.h"

SAKDataVisualizationManager::SAKDataVisualizationManager(SAKDebugPage *page, QWidget *parent)
    :QWidget (parent)
    ,debugPage (page)
    ,ui (new Ui::SAKDataVisualizationManager)
{
    ui->setupUi(this);
    tabWidget = ui->tabWidget;

    setAttribute(Qt::WA_DeleteOnClose, true);

    initPage();
}

SAKDataVisualizationManager::~SAKDataVisualizationManager()
{
    delete ui;
    delete throughputWidget;
}

void SAKDataVisualizationManager::initPage()
{
    throughputWidget = new SAKThroughputWidget(debugPage);
    tabWidget->addTab(throughputWidget, tr("吞吐量"));
}
