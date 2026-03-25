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

#include "../common/chartdatahandler.h"
#include "chartsettings.h"

ChartPanel::ChartPanel(QWidget *parent)
    : Panel(parent)
    , m_chartDataHandler(Q_NULLPTR)
{
    m_chartView = new QChartView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_chartView);
    setLayout(layout);

    m_chartView->setContentsMargins(0, 0, 0, 0);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chart = new QChart();
    m_chartView->setChart(m_chart);
    m_chartDataHandler = new ChartDataHandler(this);
    connect(m_chartDataHandler, &ChartDataHandler::newValues, this, &ChartPanel::onNewValues);
    connect(m_chartDataHandler, &ChartDataHandler::newPoints, this, &ChartPanel::onNewPoints);
    m_chartDataHandler->start();

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QStyleHints *styleHints = qApp->styleHints();
    Qt::ColorScheme colorScheme = styleHints->colorScheme();
    if (colorScheme == Qt::ColorScheme::Dark) {
        m_chart->setTheme(QChart::ChartThemeDark);
    } else if (colorScheme == Qt::ColorScheme::Light) {
        m_chart->setTheme(QChart::ChartThemeLight);
    } else {
        m_chart->setTheme(QChart::ChartThemeQt);
    }

    connect(styleHints, &QStyleHints::colorSchemeChanged, this, [this](Qt::ColorScheme scheme) {
        if (scheme == Qt::ColorScheme::Dark) {
            m_chart->setTheme(QChart::ChartThemeDark);
        } else if (scheme == Qt::ColorScheme::Light) {
            m_chart->setTheme(QChart::ChartThemeLight);
        } else {
            m_chart->setTheme(QChart::ChartThemeQt);
        }
    });
#endif
}

ChartPanel::~ChartPanel() {}

void ChartPanel::inputBytes(const QByteArray &bytes, const QString &flag)
{
    Q_UNUSED(flag);
    m_chartDataHandler->inputBytes(bytes);
}

ChartDataHandler *ChartPanel::chartDataHandler() const
{
    return m_chartDataHandler;
}
