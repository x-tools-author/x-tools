/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKCHARTSTHROUGHPUTWIDGET_HH
#define SAKCHARTSTHROUGHPUTWIDGET_HH

#include <QTimer>
#include <QChart>
#include <QWidget>
#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

class SAKDebugPage;
class SAKChartsThroughputWidget:public QWidget
{
    Q_OBJECT
public:
    SAKChartsThroughputWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKChartsThroughputWidget();
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
