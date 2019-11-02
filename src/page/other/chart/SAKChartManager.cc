/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKDebugPage.hh"
#include "SAKChartManager.hh"
#include "MoreChartWidget.hh"
#include "ThroughputWidget.hh"

#include <QDebug>

SAKChartManager::SAKChartManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
    ,throughputWidget (nullptr)
    ,moreChartWidget (nullptr)
{
    throughputPushButton = _debugPage->throughputPushButton;
//    moreChartPushButton = _debugPage->moreChartPushButton;

    connect(throughputPushButton, &QPushButton::clicked, this, &SAKChartManager::showThroughputPushWidget);
//    connect(moreChartPushButton, &QPushButton::clicked, this, &SAKChartManager::showMoreChartWidget);
}

SAKChartManager::~SAKChartManager()
{
    if (throughputWidget){
        delete throughputWidget;
    }

    if (moreChartWidget){
        delete moreChartWidget;
    }
}

void SAKChartManager::showThroughputPushWidget()
{
    if (throughputWidget){
        if (throughputWidget->isHidden()){
            throughputWidget->show();
        }else if (throughputWidget->isMinimized()){
            throughputWidget->showNormal();
        }else{
            throughputWidget->activateWindow();
        }
    }else{
        throughputWidget = new ThroughputWidget(_debugPage);
        throughputWidget->show();
        connect(throughputWidget, &QWidget::destroyed, this, &SAKChartManager::resetThroughputWidgetPtr);
    }
}

void SAKChartManager::showMoreChartWidget()
{
    if (moreChartWidget){
        if (moreChartWidget->isHidden()){
            moreChartWidget->show();
        }else if (moreChartWidget->isMinimized()){
            moreChartWidget->showNormal();
        }else{
            moreChartWidget->activateWindow();
        }
    }else{
        moreChartWidget = new MoreChartWidget(_debugPage);
        moreChartWidget->show();
        connect(moreChartWidget, &QWidget::destroyed, this, &SAKChartManager::resetMoreChartPushButtonPtr);
    }
}

void SAKChartManager::resetThroughputWidgetPtr()
{
    throughputWidget = nullptr;
}

void SAKChartManager::resetMoreChartPushButtonPtr()
{
    moreChartWidget = nullptr;
}
