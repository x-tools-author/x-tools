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
#include <QValueAxis>
#include <QHBoxLayout>

#include "SAKDebugPage.hh"
#include "SAKThroughputWidget.hh"

SAKThroughputWidget::SAKThroughputWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,debugPage (debugPage)
{
    rxLineSeries = new QLineSeries();
    txLineSeries = new QLineSeries();
    chart = new QChart();
    chartView = new QChartView();

    dataContext.rxBytes = 0;
    dataContext.txBytes = 0;
    dataContext.rxMax = 10;
    dataContext.txMax = 10;

    resize(800, 400);
    setWindowTitle(tr("吞吐量"));

    rxLineSeries->setName(tr("接收数据"));
    txLineSeries->setName(tr("发送数据"));

    chart->addSeries(rxLineSeries);
    chart->addSeries(txLineSeries);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(QVariant::fromValue(0), QVariant::fromValue(60));
    chart->axes(Qt::Horizontal).first()->setReverse(true);
    reinterpret_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first())->setTickCount(30);
    reinterpret_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first())->setLabelFormat("%d");   // 坐标显示格式（整数）

    chart->axes(Qt::Vertical).first()->setMin(QVariant::fromValue(-1));
    reinterpret_cast<QValueAxis*>(chart->axes(Qt::Vertical).first())->setLabelFormat("%d");

    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);

    updateTimer.setInterval(1*1000);
    connect(&updateTimer, &QTimer::timeout, this, &SAKThroughputWidget::updateTimerTimeout);
    updateTimer.start();

    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKThroughputWidget::dataRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKThroughputWidget::dataWite);
}

SAKThroughputWidget::~SAKThroughputWidget()
{
    delete rxLineSeries;
    delete txLineSeries;
    delete chart;
    delete chartView;
}

void SAKThroughputWidget::updateTimerTimeout()
{
    while (rxLineSeries->count() >= 60) {
        rxLineSeries->remove(0);
    }

    while (txLineSeries->count() >= 60) {
        txLineSeries->remove(0);
    }

    auto updateLineSeries = [&](QLineSeries *line, qreal &bytes, qreal &max){
        qreal maxTemp = 10;
        for (int i = 0; i < line->count(); i++){
            if (line->at(i).y() > maxTemp){
                maxTemp = line->at(i).y();
            }
            QPointF p = line->at(i);
            p.setX(p.x()+1);
            line->replace(i, p);
        }
        if (maxTemp <= 10){
            max = 10;
        }else{
            max = maxTemp;
        }

        this->chart->axes(Qt::Vertical).first()->setMax(QVariant::fromValue(qMax(this->dataContext.rxMax, this->dataContext.txMax) + 1));
        line->append(0, bytes);
        bytes = 0;
    };

    updateLineSeries(rxLineSeries, dataContext.rxBytes, dataContext.rxMax);
    updateLineSeries(txLineSeries, dataContext.txBytes, dataContext.txMax);
}

void SAKThroughputWidget::dataRead(QByteArray data)
{
    dataContext.rxBytes += data.length();
}

void SAKThroughputWidget::dataWite(QByteArray data)
{
    dataContext.txBytes += data.length();
}
