/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebugPage.hh"
#include "SAKDebugPageChartsController.hh"
#include "SAKChartsXYSerialWidget.hh"
#include "SAKChartsThroughputWidget.hh"

#include "ui_SAKDebugPageChartsController.h"

SAKDebugPageChartsController::SAKDebugPageChartsController(SAKDebugPage *page, QWidget *parent)
    :QWidget (parent)
    ,mDebugPage (page)
    ,mUi (new Ui::SAKDebugPageChartsController)
{
    mUi->setupUi(this);
    mTabWidget = mUi->tabWidget;
    Q_ASSERT_X(page, __FUNCTION__, "The parameter(page) can not be null!");

#if 0
    setAttribute(Qt::WA_DeleteOnClose, true);
#endif

    mXYSerialWidget = new SAKChartsXYSerialWidget;
    connect(mDebugPage, &SAKDebugPage::bytesRead, mXYSerialWidget, &SAKChartsXYSerialWidget::inputBytes);
    mTabWidget->addTab(mXYSerialWidget, tr("XY serial"));

    mThroughputWidget = new SAKChartsThroughputWidget(mDebugPage);
    mTabWidget->addTab(mThroughputWidget, tr("Throughput"));
}

SAKDebugPageChartsController::~SAKDebugPageChartsController()
{
    delete mUi;
}
