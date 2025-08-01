/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "chartpanel.h"

#include <QStyleHints>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "../utilities/chartdatahandler.h"
#include "chartsettings.h"
#include "common/xtools.h"

ChartPanel::ChartPanel(QWidget *parent)
    : Panel(parent)
    , m_chartDataHandler(Q_NULLPTR)
    , m_settingsMenu(Q_NULLPTR)
{
    m_chartView = new QChartView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    setLayout(layout);

    m_chartView->setContentsMargins(0, 0, 0, 0);
    m_chartView->setRenderHint(QPainter::Antialiasing);
#if 0
    setAttribute(Qt::WA_TranslucentBackground);
    viewport()->setAttribute(Qt::WA_TranslucentBackground);
#endif

    m_chart = new QChart();
    m_chartView->setChart(m_chart);

    m_chartDataHandler = new ChartDataHandler(this);
    connect(m_chartDataHandler, &ChartDataHandler::newValues, this, &ChartPanel::onNewValues);
    connect(m_chartDataHandler, &ChartDataHandler::newPoints, this, &ChartPanel::onNewPoints);
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

ChartPanel::~ChartPanel() {}

QMenu *ChartPanel::chartSettingsMenu()
{
    if (!m_settingsMenu) {
        m_settingsMenu = new QMenu(this);
        QWidgetAction *action = new QWidgetAction(m_settingsMenu);
        ChartSettings *settings = chartSettingsWidget();
        action->setDefaultWidget(settings);
        m_settingsMenu->addAction(action);
    }

    return m_settingsMenu;
}

ChartDataHandler *ChartPanel::chartDataHandler() const
{
    return m_chartDataHandler;
}

void ChartPanel::updateChartsTheme(bool darkMode)
{
    m_chart->setTheme(darkMode ? QChart::ChartThemeDark : QChart::ChartThemeLight);
}
