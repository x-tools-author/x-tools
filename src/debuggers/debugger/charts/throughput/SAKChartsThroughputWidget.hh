/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCHARTSTHROUGHPUTWIDGET_HH
#define SAKCHARTSTHROUGHPUTWIDGET_HH

#include <QTimer>
#include <QChart>
#include <QWidget>
#include <QChartView>
#include <QLineSeries>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif

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
