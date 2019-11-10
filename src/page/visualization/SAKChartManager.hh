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
#ifndef SAKCHART_HH
#define SAKCHART_HH

#include <QObject>
#include <QPushButton>

class SAKDebugPage;
class SAKThroughputWidget;
class SAKChartManager:public QObject
{
    Q_OBJECT
public:
    SAKChartManager(SAKDebugPage *debugPage, QObject *parent = nullptr);
    ~SAKChartManager();
private:
    SAKDebugPage *_debugPage;

    QPushButton *throughputPushButton;

    SAKThroughputWidget *throughputWidget;

    /// 显示吞吐量面板
    void showThroughputPushWidget();

    void resetThroughputWidgetPtr();
};

#endif
