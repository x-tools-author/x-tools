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
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef THROUGHPUTWIDGET_HH
#define THROUGHPUTWIDGET_HH

#include "SAKDebugPage.hh"

#include <QTimer>
#include <QChart>
#include <QWidget>
#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class SAKDebugPage;

class ThroughputWidget:public QWidget
{
    Q_OBJECT
public:
    ThroughputWidget(SAKDebugPage *debugPage, QWidget *parent = nullptr);
    ~ThroughputWidget();

private:
    struct DataContext {
        qreal rxBytes;
        qreal txBytes;
        qreal rxMax;
        qreal txMax;
    }dataContext;

    SAKDebugPage *_debugPage;

    QLineSeries *rxLineSeries;
    QLineSeries *txLineSeries;

    QChart *chart;
    QChartView *chartView;

    QTimer updateTimer;
    void updateTimerTimeout();
    void dataReadOrwritten(QByteArray data, SAKDebugPage::OutputParameters parameters);
};

#endif
