/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QValueAxis>
#include <QXYSeries>

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using QtCharts::QChart;
using QtCharts::QValueAxis;
using QtCharts::QXYSeries;
#endif

namespace Ui {
class ChartsUi;
}

struct ChartsUiDataKeys
{
    const QString dataType{"dataType"};
    const QString testData{"testData"};
    const QString channels{"channels"};
    const QString legendVisible{"legendVisible"};
    const QString cachePoints{"cachePoints"};

    const QString channelName{"channelName"};
    const QString channelVisible{"channelVisible"};
    const QString channelColor{"channelColor"};
    const QString channelType{"channelType"};
};

class linechartsettings;
class LineChartView : public QChartView
{
    Q_OBJECT
public:
    explicit LineChartView(QWidget *parent = Q_NULLPTR);
    ~LineChartView() override;

    QVariantMap save() const;
    void load(const QVariantMap &parameters);

    QMenu *settingsMenu();
    QWidget *settingsWidget();
    void updateChartsTheme(bool darkMode);

private:
    Ui::ChartsUi *ui;
    linechartsettings *m_settings;
    QMenu *m_settingsMenu{nullptr};
    QList<QXYSeries *> m_series;
    QChart *m_chart;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

private:
    void onNewValues(const QList<double> &values);
    void onNewPoints(const QList<QPointF> &points);

    void onSetDataType(int type);
    void onSetLegendVisible(bool visible);
    void onClearChannels();
    void onImportChannels();
    void onExportChannels();
    void onSetChannelVisible(int channelIndex, bool visible);
    void onSetChannelType(int channelIndex, int type);
    void onSetChannelColor(int channelIndex, const QColor &color);
    void onSetChannelName(int channelIndex, const QString &name);
};
