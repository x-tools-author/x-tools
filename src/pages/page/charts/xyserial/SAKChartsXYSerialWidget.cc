/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDialog>
#include <QDateTime>
#include <QHBoxLayout>

#include "SAKXYSerialEditDialog.hh"
#include "SAKChartsXYSerialWidget.hh"

#include "ui_SAKChartsXYSerialWidget.h"


SAKChartsXYSerialWidget::SAKChartsXYSerialWidget(QWidget *parent)
    :QWidget (parent)
    ,mXYSerialEditDialog(new SAKXYSerialEditDialog)
    ,mUi (new Ui::SAKChartsXYSerialWidget)
{
    mUi->setupUi(this);
    mChartViewerWidget = mUi->chartViewerWidget;
    mChartSettingsPushButton = mUi->chartSettingsPushButton;
    mDeletePushButton = mUi->deletePushButton;
    mEditPushButton = mUi->editPushButton;
    mAddPushButton = mUi->addPushButton;

    /// @brief 初始化表格视图
    mChartView = new QChartView(mChartViewerWidget);
    mChart = new QChart;
    mChartView->setChart(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);

    mXAxis = new QDateTimeAxis;
    mXAxis->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
    mChart->addAxis(mXAxis, Qt::AlignBottom);

    mYAxis = new QValueAxis;
    mYAxis->setRange(0, 100);
    mChart->addAxis(mYAxis, Qt::AlignLeft);

    QHBoxLayout *layout = new QHBoxLayout(mChartViewerWidget);
    layout->addWidget(mChartView);
    mChartViewerWidget->setLayout(layout);
}

SAKChartsXYSerialWidget::~SAKChartsXYSerialWidget()
{
    delete mUi;
}

void SAKChartsXYSerialWidget::inputBytes(QByteArray bytes)
{
    Q_UNUSED(bytes);
}

void SAKChartsXYSerialWidget::on_chartSettingsPushButton_clicked()
{

}

void SAKChartsXYSerialWidget::on_editPushButton_clicked()
{
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() != QDialog::Accepted){
        return;
    }
}

void SAKChartsXYSerialWidget::on_addPushButton_clicked()
{
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() != QDialog::Accepted){
        return;
    }

    SAKXYSerialEditDialog::ParametersContext ctx = mXYSerialEditDialog->parameters();
    QXYSeries *xySerial = Q_NULLPTR;
    if (ctx.chartParameters.chartType == SAKXYSerialEditDialog::ParametersContext::ChartParametersContext::ChartTypeLine){
        xySerial = new QLineSeries;
    }else{
        xySerial = new QScatterSeries;
    }

    mChart->addSeries(xySerial);
    xySerial->append(QDateTime::currentMSecsSinceEpoch(), 1);
    xySerial->append(QDateTime::currentMSecsSinceEpoch()+1000, 1);
    xySerial->attachAxis(mXAxis);
    xySerial->attachAxis(mYAxis);
}
