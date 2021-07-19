/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKPLUGINCHARTS_HH
#define SAKPLUGINCHARTS_HH

#include <QWidget>
#include <QTabWidget>

namespace Ui {
    class SAKPluginCharts;
};

class SAKPluginCharts : public QWidget
{
    Q_OBJECT
public:
    SAKPluginCharts(QWidget *parent = Q_NULLPTR);
    ~SAKPluginCharts();
private:
    Ui::SAKPluginCharts *mUi;
};

#endif
