/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDATAVISUALIZATIONMANAGER_HH
#define SAKDATAVISUALIZATIONMANAGER_HH

#include <QWidget>
#include <QTabWidget>

namespace Ui {
    class SAKDataVisualizationManager;
};

class SAKDebugPage;
class SAKThroughputWidget;
class SAKDataVisualizationManager:public QWidget
{
    Q_OBJECT
public:
    SAKDataVisualizationManager(SAKDebugPage *page, QWidget *parent = Q_NULLPTR);
    ~SAKDataVisualizationManager();
private:
    SAKDebugPage *debugPage;
    SAKThroughputWidget *throughputWidget;

private:
    void initPage();

private:
    Ui::SAKDataVisualizationManager *ui;
    QTabWidget *tabWidget;
};

#endif
