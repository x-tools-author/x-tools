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

#include "barcharts/barchartview.h"
#include "linecharts/linechartview.h"
#include "utilities/chartdatahandler.h"

ChartsView::ChartsView(QWidget *parent)
    : QWidget(parent)
{
    m_buttonGroup = new QButtonGroup(this);
    m_layout = new QStackedLayout(this);
    setLayout(m_layout);

    addChartView<LineChartView>(QString(":/res/icons/line_series.svg"), 0);
    addChartView<BarChartView>(QString(":/res/icons/bar.svg"), 1);
}

ChartsView::~ChartsView() {}

QList<QToolButton *> ChartsView::chartControllers()
{
    return m_chartControllers;
}

void ChartsView::resetCharts()
{
    for (ChartView *&view : m_chartViews) {
        view->resetChart();
    }
}

void ChartsView::inputBytes(const QByteArray &bytes)
{
    if (bytes.isEmpty()) {
        return;
    }

    for (ChartView *&view : m_chartViews) {
        auto dataHandler = view->chartDataHandler();
        dataHandler->inputBytes(bytes);
    }
}

QVariantMap ChartsView::save()
{
    QVariantMap data;
    data.insert("currentIndex", m_layout->currentIndex());

    for (ChartView *&view : m_chartViews) {
        data.insert(view->metaObject()->className(), view->save());
    }
    return data;
}

void ChartsView::load(const QVariantMap &parameters)
{
    int index = parameters.value("currentIndex").toInt();
    if (index >= 0 && index < m_layout->count()) {
        m_chartControllers.at(index)->setChecked(true);
        m_layout->setCurrentIndex(index);
    }

    for (ChartView *&view : m_chartViews) {
        view->load(parameters.value(view->metaObject()->className()).toMap());
    }
}
