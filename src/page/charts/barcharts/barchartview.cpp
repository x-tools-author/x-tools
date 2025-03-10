/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "barchartview.h"

#include <QChartView>
#include <QCheckBox>
#include <QFileDialog>
#include <QGraphicsLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLineSeries>
#include <QMenu>
#include <QPointF>
#include <QPushButton>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QStyleHints>
#include <QTimer>
#include <QWidgetAction>

#include <xlsxdocument.h>

#include "barchartsettings.h"
#include "common/xtools.h"
#include "page/charts/utilities/chartdatahandler.h"

BarChartView::BarChartView(QWidget *parent)
    : ChartView(parent)
{
    m_settings = new BarChartSettings();

    // clang-format off
    connect(m_settings, &BarChartSettings::dataFormatChanged, this, &BarChartView::onDataFormatChanged);
    connect(m_settings, &BarChartSettings::invokeSetLegendVisible, this, &BarChartView::onSetLegendVisible);
    connect(m_settings, &BarChartSettings::channelVisibleChanged, this, &BarChartView::onSetChannelVisible);
    connect(m_settings, &BarChartSettings::channelColorChanged, this, &BarChartView::onSetChannelColor);
    connect(m_settings, &BarChartSettings::channelNameChanged, this, &BarChartView::onSetChannelName);
    // clang-format on

    m_axisX = new QCategoryAxis();
    m_axisX->setRange(0, 100);
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 1);

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setMargins(QMargins(0, 0, 0, 0));

#if xEnableColorScheme
    auto currentScheme = qApp->styleHints()->colorScheme();
    if (currentScheme == Qt::ColorScheme::Dark) {
        m_chart->setTheme(QChart::ChartThemeDark);
    } else if (currentScheme == Qt::ColorScheme::Light) {
        m_chart->setTheme(QChart::ChartThemeLight);
    }
#endif

    m_barSeries = new QStackedBarSeries(m_chart);
    m_barSeries->setUseOpenGL(true);
    const int channelCount = 16;
    for (int i = 0; i < channelCount; ++i) {
        QBarSet *set = new QBarSet("", m_chart);
        m_barSeries->append(set);
    }

    m_settings->load(BarChartView::save());
}

BarChartView::~BarChartView()
{
    m_chart->deleteLater();
    m_settings->deleteLater();
}

QVariantMap BarChartView::save() const
{
    QVariantMap data;

    BarChartSettingsKeys keys;
    data[keys.dataFormat] = m_settings->dataType();
    data[keys.legendVisible] = m_settings->legendVisible();
    data[keys.cachePoints] = m_settings->cachePoints();
    QJsonArray channels;
    for (int i = 0; i < m_barSets.size(); ++i) {
        QJsonObject obj;
        // obj[keys.channel.channelName] = m_axisX->categoriesLabels().at(i);
        // obj[keys.channel.channelVisible] = m_barSets[i].hid
        // obj[keys.channel.channelColor] = m_barSets[i]->color().name();
        // channels.append(obj);
    }

    return data;
}

void BarChartView::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    BarChartSettingsKeys keys;
    m_settings->load(parameters);
    QJsonArray channels = parameters.value(keys.channels).toJsonArray();

    if (channels.size() != m_barSets.size()) {
        qWarning() << "The number of channels is not equal to the number of series.";
        return;
    }

    for (int i = 0; i < channels.size(); ++i) {
        QJsonObject obj = channels[i].toObject();
        QString name = obj.value(keys.channel.channelName).toString();
        QString color = obj.value(keys.channel.channelColor).toString();
        bool visible = channels[i].toObject().value(keys.channel.channelVisible).toBool();

        // QBarSet *series = m_barSets[i];
        // series->setName(name);
        // series->setVisible(visible);
        // series->setColor(color);
    }
}

ChartSettings *BarChartView::chartSettingsWidget()
{
    return m_settings;
}

void BarChartView::resetChart()
{
    // for (auto &series : m_barSets) {
    //     series->clear();
    // }

    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 1);
}

void BarChartView::onDataFormatChanged(int type)
{
    m_chartDataHandler->setDataFormat(type);
}

void BarChartView::onSetLegendVisible(bool visible)
{
    m_chart->legend()->setVisible(visible);
}

void BarChartView::onClearChannels()
{
    // for (auto &series : m_barSets) {
    //     series->clear();
    // }

    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 1);
}

void BarChartView::onImportChannels()
{
    const QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                          tr("Import Data from Excel"),
                                                          "",
                                                          "Excel(*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    QXlsx::Document xlsx(fileName);
    QStringList sheetNames = xlsx.sheetNames();
    for (int i = 0; i < sheetNames.size(); ++i) {
        xlsx.selectSheet(sheetNames.at(i));
        QList<QPointF> points;
        for (int j = 2; j < std::numeric_limits<int>::max(); ++j) {
            QVariant x = xlsx.read(j, 1);
            QVariant y = xlsx.read(j, 2);
            if (x.isNull() && y.isNull()) {
                break;
            }

            points.append(QPointF(x.toDouble(), y.toDouble()));
        }

        if (i < m_barSets.size()) {
            //m_barSets[i]->replace(points);
        }
    }
}

void BarChartView::onExportChannels()
{
    const QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                          tr("Export Data to Excel"),
                                                          "data.xlsx",
                                                          "Excel(*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    QXlsx::Document xlsx;
    // for (int i = 0; i < BarChartSettings::channelCount(); ++i) {
    //     xlsx.addSheet(m_barSets.at(i)->name());
    //     xlsx.selectSheet(i);
    //     xlsx.write(1, 1, "x");
    //     xlsx.write(1, 2, "y");

    //     QList<QPointF> points = m_barSets.at(i)->points();
    //     for (int j = 0; j < points.size(); ++j) {
    //         xlsx.write(j + 2, 1, points.at(j).x());
    //         xlsx.write(j + 2, 2, points.at(j).y());
    //     }
    // }

    xlsx.selectSheet(0);
    if (xlsx.saveAs(fileName)) {
        qDebug() << "File saved successfully!";
    } else {
        qDebug() << "Failed to save file!";
    }
}

void BarChartView::onNewValues(const QList<double> &values)
{
    int count = qMin(values.size(), m_barSets.size());
    // for (int i = 0; i < count; ++i) {
    //     double value = values[i];
    //     QXYSeries *series = m_barSets[i];

    //     QPointF pos = QPointF(series->count(), value);
    //     series->append(pos);

    //     if (m_settings->cachePoints() > 0 && series->count() > m_settings->cachePoints()) {
    //         if (pos.x() > m_axisX->max()) {
    //             m_axisX->setMax(pos.x());
    //             m_axisX->setMin(pos.x() - m_settings->cachePoints());
    //         }
    //     }

    //     if (value > m_axisY->max()) {
    //         m_axisY->setMax(value);
    //     }

    //     if (value < m_axisY->min()) {
    //         m_axisY->setMin(value);
    //     };

    //     if (pos.x() > m_axisX->max()) {
    //         m_axisX->setMax(pos.x());
    //     }
    // }
}

void BarChartView::onNewPoints(const QList<QPointF> &points)
{
    int count = qMin(points.size(), m_barSets.size());
    // for (int i = 0; i < count; ++i) {
    //     m_barSets[i]->append(points[i]);

    //     if (m_settings->cachePoints() > 0 && m_barSets[i]->count() > m_settings->cachePoints()) {
    //         m_barSets[i]->remove(0);
    //     }

    //     if (points[i].y() > m_axisY->max()) {
    //         m_axisY->setMax(points[i].y() + points[i].y() * 0.1);
    //     }

    //     if (points[i].y() < m_axisY->min()) {
    //         m_axisY->setMin(points[i].y() - points[i].y() * 0.1);
    //     }

    //     if (points[i].x() > m_axisX->max()) {
    //         m_axisX->setMax(points[i].x() + points[i].x() * 0.1);
    //     }

    //     if (points[i].x() < m_axisX->min()) {
    //         m_axisX->setMin(points[i].x() - points[i].x() * 0.1);
    //     }
    // }
}

void BarChartView::onSetChannelVisible(int channelIndex, bool visible)
{
    // if (channelIndex >= 0 && channelIndex < m_barSets.size()) {
    //     m_barSets[channelIndex]->setVisible(visible);
    // }
}

void BarChartView::onSetChannelType(int channelIndex, int type)
{
    // if (channelIndex >= 0 && channelIndex < m_barSets.size()) {
    //     QAbstractSeries::SeriesType seriesType = static_cast<QAbstractSeries::SeriesType>(type);
    //     if (m_barSets[channelIndex]->type() != seriesType) {
    //         QXYSeries *newSeries = nullptr;
    //         if (seriesType == QAbstractSeries::SeriesType::SeriesTypeLine) {
    //             newSeries = new QLineSeries();
    //         } else if (seriesType == QAbstractSeries::SeriesType::SeriesTypeSpline) {
    //             newSeries = new QSplineSeries();
    //         } else if (seriesType == QAbstractSeries::SeriesType::SeriesTypeScatter) {
    //             newSeries = new QScatterSeries();
    //         } else {
    //             qWarning() << "Unknown series type.";
    //         }

    //         if (newSeries) {
    //             auto *oldSeries = m_barSets[channelIndex];
    //             m_barSets[channelIndex] = newSeries;
    //             m_chart->addSeries(newSeries);

    //             newSeries->setName(oldSeries->name());
    //             newSeries->setVisible(oldSeries->isVisible());
    //             newSeries->setColor(oldSeries->color());
    //             newSeries->replace(oldSeries->points());
    //             newSeries->attachAxis(m_axisX);
    //             newSeries->attachAxis(m_axisY);
    //             newSeries->setUseOpenGL(true);

    //             oldSeries->setParent(nullptr);
    //             oldSeries->deleteLater();
    //             oldSeries = newSeries;

    //             for (auto &series : m_barSets) {
    //                 m_chart->removeSeries(series);
    //             }

    //             for (auto &series : m_barSets) {
    //                 m_chart->addSeries(series);
    //             }
    //         }
    //     }
    // }
}

void BarChartView::onSetChannelColor(int channelIndex, const QColor &color)
{
    if (channelIndex >= 0 && channelIndex < m_barSets.size()) {
        m_barSets[channelIndex]->setColor(color);
    }
}

void BarChartView::onSetChannelName(int channelIndex, const QString &name)
{
    // if (channelIndex >= 0 && channelIndex < m_barSets.size()) {
    //     m_barSets[channelIndex]->setName(name);
    // }
}
