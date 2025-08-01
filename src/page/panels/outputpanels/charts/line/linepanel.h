/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>
#include <QValueAxis>
#include <QXYSeries>

#include "../common/chartpanel.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using QtCharts::QValueAxis;
using QtCharts::QXYSeries;
#endif

namespace Ui {
class ChartsUi;
}

class LineSettings;
class LinePanel : public ChartPanel
{
    Q_OBJECT
public:
    struct DataKeys
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

public:
    explicit LinePanel(QWidget *parent = Q_NULLPTR);
    ~LinePanel() override;

    QWidget *menuWidget() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void resetChart() override;

private:
    Ui::ChartsUi *ui;
    LineSettings *m_settings;
    QList<QXYSeries *> m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_x{0.0};

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
