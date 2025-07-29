/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QMenu>
#include <QStackedBarSeries>
#include <QValueAxis>
#include <QXYSeries>

#include "../common/chartview.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using QtCharts::QBarCategoryAxis;
using QtCharts::QBarSet;
using QtCharts::QStackedBarSeries;
using QtCharts::QValueAxis;
using QtCharts::QXYSeries;
#endif

namespace Ui {
class ChartsUi;
}

class BarChartSettings;
class BarChartView : public ChartView
{
    Q_OBJECT
public:
    explicit BarChartView(QWidget *parent = Q_NULLPTR);
    ~BarChartView() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    PlotSettings *chartSettingsWidget() override;
    void resetChart() override;

private:
    Ui::ChartsUi *ui;
    BarChartSettings *m_settings;
    QList<QBarSet *> m_barSets;
    QStackedBarSeries *m_barSeries;
    QBarCategoryAxis *m_axisX;
    QValueAxis *m_axisY;

private:
    void onNewValues(const QList<double> &values) override;
    void onNewPoints(const QList<QPointF> &points) override;

    void onDataFormatChanged(int type);
    void onChannelColorChanged(int channelIndex, const QColor &color);
    void onChannelNameChanged(int channelIndex, const QString &name);
};
