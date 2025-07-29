/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QChart>
#include <QChartView>
#include <QMenu>
#include <QPointF>
#include <QVariantMap>

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using QtCharts::QChart;
using QtCharts::QChartView;
#endif

class PlotSettings;
class ChartDataHandler;
class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(QWidget *parent = Q_NULLPTR);
    ~ChartView() override;

    virtual QVariantMap save() const = 0;
    virtual void load(const QVariantMap &parameters) = 0;
    virtual PlotSettings *chartSettingsWidget() = 0;
    virtual void resetChart() = 0;

    QMenu *chartSettingsMenu();
    ChartDataHandler *chartDataHandler() const;
    void updateChartsTheme(bool darkMode);

protected:
    virtual void onNewValues(const QList<double> &values) = 0;
    virtual void onNewPoints(const QList<QPointF> &points) = 0;

protected:
    ChartDataHandler *m_chartDataHandler;
    QChart *m_chart;

private:
    QMenu *m_settingsMenu;
};
