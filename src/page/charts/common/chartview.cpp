/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "chartview.h"

#include <QStyleHints>
#include <QWidgetAction>

#include "chartsettings.h"
#include "common/xtools.h"
#include "page/charts/utilities/chartdatahandler.h"

ChartView::ChartView(QWidget *parent)
    : QChartView(parent)
    , m_chartDataHandler(Q_NULLPTR)
    , m_settingsMenu(Q_NULLPTR)
{
    setContentsMargins(0, 0, 0, 0);
    setRenderHint(QPainter::Antialiasing);
#if 0
    setAttribute(Qt::WA_TranslucentBackground);
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
#endif

    m_chart = new QChart();
    setChart(m_chart);

    m_chartDataHandler = new ChartDataHandler(this);
    connect(m_chartDataHandler, &ChartDataHandler::newValues, this, &ChartView::onNewValues);
    connect(m_chartDataHandler, &ChartDataHandler::newPoints, this, &ChartView::onNewPoints);
    m_chartDataHandler->start();

#if xEnableColorScheme
    auto currentScheme = qApp->styleHints()->colorScheme();
    if (currentScheme == Qt::ColorScheme::Dark) {
        m_chart->setTheme(QChart::ChartThemeDark);
    } else if (currentScheme == Qt::ColorScheme::Light) {
        m_chart->setTheme(QChart::ChartThemeLight);
    }
#endif
}

ChartView::~ChartView() {}

QMenu *ChartView::chartSettingsMenu()
{
    if (!m_settingsMenu) {
        m_settingsMenu = new QMenu(this);
        QWidgetAction *action = new QWidgetAction(m_settingsMenu);
        action->setDefaultWidget(chartSettingsWidget());
        m_settingsMenu->addAction(action);
    }

    return m_settingsMenu;
}

ChartDataHandler *ChartView::chartDataHandler() const
{
    return m_chartDataHandler;
}

void ChartView::updateChartsTheme(bool darkMode)
{
    m_chart->setTheme(darkMode ? QChart::ChartThemeDark : QChart::ChartThemeLight);
}
