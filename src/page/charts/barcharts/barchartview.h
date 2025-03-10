/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QBarSeries>
#include <QCategoryAxis>
#include <QChart>
#include <QChartView>
#include <QMenu>
#include <QValueAxis>
#include <QXYSeries>

#include "page/charts/common/chartview.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
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
    ChartSettings *chartSettingsWidget() override;
    void resetChart() override;

private:
    Ui::ChartsUi *ui;
    BarChartSettings *m_settings;
    QList<QXYSeries *> m_series;
    QCategoryAxis *m_axisX;
    QValueAxis *m_axisY;

private:
    void onNewValues(const QList<double> &values) override;
    void onNewPoints(const QList<QPointF> &points) override;

    void onDataFormatChanged(int type);
    void onSetLegendVisible(bool visible);
    void onClearChannels();
    void onImportChannels();
    void onExportChannels();
    void onSetChannelVisible(int channelIndex, bool visible);
    void onSetChannelType(int channelIndex, int type);
    void onSetChannelColor(int channelIndex, const QColor &color);
    void onSetChannelName(int channelIndex, const QString &name);
};
