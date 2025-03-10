/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "chartsview.h"

#include <QDebug>
#include <QPointF>
#include <QStackedLayout>
#include <QTimer>
#include <QtMath>

#include "linecharts/linechartview.h"
#include "utilities/chartdatahandler.h"

ChartsView::ChartsView(QWidget *parent)
    : QWidget(parent)
{
    QStackedLayout *layout = new QStackedLayout(this);
    setLayout(layout);
    LineChartView *lineChartView = new LineChartView();
    layout->addWidget(lineChartView);
    m_chartViews.append(lineChartView);

    QToolButton *lineChartController = new QToolButton();
    lineChartController->setIcon(QIcon(":/res/icons/line_series.svg"));
    lineChartController->setMenu(lineChartView->chartSettingsMenu());
    lineChartController->setPopupMode(QToolButton::MenuButtonPopup);
    m_chartControllers.append(lineChartController);
    connect(lineChartController, &QToolButton::clicked, layout, [layout]() {
        layout->setCurrentIndex(0);
    });
}

ChartsView::~ChartsView() {}

QList<QToolButton *> ChartsView::chartControllers()
{
    return m_chartControllers;
}

void ChartsView::resetCharts()
{
    for (ChartView *view : m_chartViews) {
        view->resetChart();
    }
}

void ChartsView::inputBytes(const QByteArray &bytes)
{
    if (bytes.isEmpty()) {
        return;
    }

    for (ChartView *view : m_chartViews) {
        auto dataHandler = view->chartDataHandler();
        dataHandler->inputBytes(bytes);
    }
}

QVariantMap ChartsView::save()
{
    QVariantMap data;
    for (ChartView *view : m_chartViews) {
        data.insert(view->metaObject()->className(), view->save());
    }
    return data;
}

void ChartsView::load(const QVariantMap &parameters)
{
    for (ChartView *view : m_chartViews) {
        view->load(parameters.value(view->metaObject()->className()).toMap());
    }
}
