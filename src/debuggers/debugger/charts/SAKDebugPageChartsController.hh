/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGECHARTSMANAGER_HH
#define SAKDEBUGPAGECHARTSMANAGER_HH

#include <QWidget>
#include <QTabWidget>

namespace Ui {
    class SAKDebugPageChartsController;
};

class SAKDebugger;
class SAKChartsXYSerialWidget;
class SAKChartsThroughputWidget;
class SAKDebugPageChartsController:public QWidget
{
    Q_OBJECT
public:
    SAKDebugPageChartsController(SAKDebugger *page, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageChartsController();
private:
    SAKDebugger *mDebugPage;
    SAKChartsXYSerialWidget *mXYSerialWidget;
    SAKChartsThroughputWidget *mThroughputWidget;
private:
    Ui::SAKDebugPageChartsController *mUi;
    QTabWidget *mTabWidget;
};

#endif
