/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTHROUGHPUTWIDGET_HH
#define SAKTHROUGHPUTWIDGET_HH

#include <QTimer>
#include <QChart>
#include <QWidget>
#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class SAKDebugPage;
class SAKThroughputWidget:public QWidget
{
    Q_OBJECT
public:
    SAKThroughputWidget(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~SAKThroughputWidget();

private:
    struct DataContext {
        qreal rxBytes;
        qreal txBytes;
        qreal rxMax;
        qreal txMax;
    }dataContext;

    SAKDebugPage *debugPage;

    QLineSeries *rxLineSeries;
    QLineSeries *txLineSeries;

    QChart *chart;
    QChartView *chartView;

    QTimer updateTimer;
    void updateTimerTimeout();

    void dataRead(QByteArray data);
    void dataWite(QByteArray data);
};

#endif
