/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKDEBUGPAGECHARTSMANAGER_HH
#define SAKDEBUGPAGECHARTSMANAGER_HH

#include <QWidget>
#include <QTabWidget>

namespace Ui {
    class SAKDebugPageChartsController;
};

class SAKDebugPage;
class SAKChartsXYSerialWidget;
class SAKChartsThroughputWidget;
class SAKDebugPageChartsController:public QWidget
{
    Q_OBJECT
public:
    SAKDebugPageChartsController(SAKDebugPage *page, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageChartsController();
private:
    SAKDebugPage *mDebugPage;
    SAKChartsXYSerialWidget *mXYSerialWidget;
    SAKChartsThroughputWidget *mThroughputWidget;
private:
    Ui::SAKDebugPageChartsController *mUi;
    QTabWidget *mTabWidget;
};

#endif
