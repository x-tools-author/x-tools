/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "linepanel.h"

#include <QChartView>
#include <QCheckBox>
#include <QDebug>
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

#include "../common/chartdatahandler.h"
#include "linesettings.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using QtCharts::QAbstractSeries;
using QtCharts::QLineSeries;
using QtCharts::QScatterSeries;
using QtCharts::QSplineSeries;
#endif

LinePanel::LinePanel(QWidget *parent)
    : ChartPanel(parent)
{
    m_settings = new LineSettings();

    // clang-format off
    connect(m_settings, &LineSettings::dataFormatChanged, this, &LinePanel::onDataFormatChanged);
    connect(m_settings, &LineSettings::invokeSetLegendVisible, this, &LinePanel::onSetLegendVisible);
    connect(m_settings, &LineSettings::invokeClearChannels, this, &LinePanel::onClearChannels);
    connect(m_settings, &LineSettings::invokeImportChannels, this, &LinePanel::onImportChannels);
    connect(m_settings, &LineSettings::invokeExportChannels, this, &LinePanel::onExportChannels);
    connect(m_settings, &LineSettings::channelVisibleChanged, this, &LinePanel::onSetChannelVisible);
    connect(m_settings, &LineSettings::channelTypeChanged, this, &LinePanel::onSetChannelType);
    connect(m_settings, &LineSettings::channelColorChanged, this, &LinePanel::onSetChannelColor);
    connect(m_settings, &LineSettings::channelNameChanged, this, &LinePanel::onSetChannelName);
    // clang-format on

    m_axisX = new QValueAxis();
    m_axisX->setRange(0, 100);
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 1);

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setMargins(QMargins(0, 0, 0, 0));

    int channelCount = LineSettings::channelCount();
    for (int i = 0; i < channelCount; ++i) {
        QLineSeries *series = new QLineSeries();
        m_chart->addSeries(series);
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);
        series->setName(QString::number(i + 1));
#if 0
        // TODO: error message is output after close main window.
        // qt.core.qobject.connect: QObject::disconnect: Unexpected nullptr parameter
        series->setUseOpenGL(true);
#endif
        m_series.append(series);
    }

    m_settings->load(LinePanel::save());
}

LinePanel::~LinePanel()
{
    m_settings->deleteLater();
}

QWidget *LinePanel::menuWidget()
{
    return m_settings;
}

QVariantMap LinePanel::save() const
{
    QVariantMap data;

    DataKeys keys;
    data[keys.dataType] = m_settings->dataType();
    data[keys.legendVisible] = m_settings->legendVisible();
    data[keys.cachePoints] = m_settings->cachePoints();
    QJsonArray channels;
    for (int i = 0; i < m_series.size(); ++i) {
        QJsonObject obj;
        obj[keys.channelName] = m_series[i]->name();
        obj[keys.channelVisible] = m_series[i]->isVisible();
        obj[keys.channelColor] = m_series[i]->color().name();
        obj[keys.channelType] = m_series[i]->type();
        channels.append(obj);
    }

    data[keys.channels] = channels;
    return data;
}

void LinePanel::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    DataKeys keys;
    m_settings->load(parameters);
    QJsonArray channels = parameters.value(keys.channels).toJsonArray();

    if (channels.size() != m_series.size()) {
        qWarning() << "The number of channels is not equal to the number of series.";
        return;
    }

    for (int i = 0; i < channels.size(); ++i) {
        QJsonObject obj = channels[i].toObject();
        QString name = obj.value(keys.channelName).toString();
        QString color = obj.value(keys.channelColor).toString();
        int type = obj.value(keys.channelType).toInt();
        bool visible = channels[i].toObject().value(keys.channelVisible).toBool();

        QXYSeries *oldSeries = m_series[i];
        oldSeries->setName(name);
        oldSeries->setVisible(visible);
        oldSeries->setColor(color);
        if (type != oldSeries->type()) {
            QXYSeries *newSeries = nullptr;
            if (type == QAbstractSeries::SeriesType::SeriesTypeLine) {
                newSeries = new QLineSeries();
            } else if (type == QAbstractSeries::SeriesType::SeriesTypeSpline) {
                newSeries = new QSplineSeries();
            } else if (type == QAbstractSeries::SeriesType::SeriesTypeScatter) {
                newSeries = new QScatterSeries();
            } else {
                qWarning() << "Unknown series type.";
            }

            if (newSeries) {
                newSeries->setName(name);
                newSeries->setVisible(visible);
                newSeries->setColor(color);
                newSeries->replace(oldSeries->points());
                newSeries->attachAxis(m_axisX);
                newSeries->attachAxis(m_axisY);
                newSeries->setUseOpenGL(true);

                m_chart->removeSeries(oldSeries);
                oldSeries->setParent(nullptr);
                oldSeries->deleteLater();
                oldSeries = nullptr;
            }
        }
    }
}

void LinePanel::resetChart()
{
    for (auto &series : m_series) {
        series->clear();
    }

    m_x = 0.0;
    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 1);
}

void LinePanel::onDataFormatChanged(int type)
{
    m_chartDataHandler->setDataFormat(type);
}

void LinePanel::onSetLegendVisible(bool visible)
{
    m_chart->legend()->setVisible(visible);
}

void LinePanel::onClearChannels()
{
    for (auto &series : m_series) {
        series->clear();
    }

    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 1);
}

void LinePanel::onImportChannels()
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

        if (i < m_series.size()) {
            m_series[i]->replace(points);
        }
    }
}

void LinePanel::onExportChannels()
{
    const QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                          tr("Export Data to Excel"),
                                                          "data.xlsx",
                                                          "Excel(*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    QXlsx::Document xlsx;
    for (int i = 0; i < LineSettings::channelCount(); ++i) {
        xlsx.addSheet(m_series.at(i)->name());
        xlsx.selectSheet(i);
        xlsx.write(1, 1, "x");
        xlsx.write(1, 2, "y");

        QList<QPointF> points = m_series.at(i)->points();
        for (int j = 0; j < points.size(); ++j) {
            xlsx.write(j + 2, 1, points.at(j).x());
            xlsx.write(j + 2, 2, points.at(j).y());
        }
    }

    xlsx.selectSheet(0);
    if (xlsx.saveAs(fileName)) {
        qDebug() << "File saved successfully!";
    } else {
        qDebug() << "Failed to save file!";
    }
}

void LinePanel::onNewValues(const QList<double> &values)
{
    int count = qMin(values.size(), m_series.size());
    for (int i = 0; i < count; ++i) {
        double value = values[i];
        QXYSeries *series = m_series[i];

        QPointF pos = QPointF(m_x, value);
        series->append(pos);

        if (m_settings->cachePoints() > 0 && series->count() > m_settings->cachePoints()) {
            if (pos.x() > m_axisX->max()) {
                m_axisX->setMax(pos.x());
                m_axisX->setMin(pos.x() - m_settings->cachePoints());
            }
        }

        if (value > m_axisY->max()) {
            m_axisY->setMax(value);
        }

        if (value < m_axisY->min()) {
            m_axisY->setMin(value);
        };

        if (pos.x() > m_axisX->max()) {
            m_axisX->setMax(pos.x());
        }
    }

    m_x += 1.0;
}

void LinePanel::onNewPoints(const QList<QPointF> &points)
{
    int count = qMin(points.size(), m_series.size());
    for (int i = 0; i < count; ++i) {
        m_series[i]->append(points[i]);

        if (m_settings->cachePoints() > 0 && m_series[i]->count() > m_settings->cachePoints()) {
            m_series[i]->remove(0);
        }

        if (points[i].y() > m_axisY->max()) {
            m_axisY->setMax(points[i].y() + points[i].y() * 0.1);
        }

        if (points[i].y() < m_axisY->min()) {
            m_axisY->setMin(points[i].y() - points[i].y() * 0.1);
        }

        if (points[i].x() > m_axisX->max()) {
            m_axisX->setMax(points[i].x() + points[i].x() * 0.1);
        }

        if (points[i].x() < m_axisX->min()) {
            m_axisX->setMin(points[i].x() - points[i].x() * 0.1);
        }
    }
}

void LinePanel::onSetChannelVisible(int channelIndex, bool visible)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        m_series[channelIndex]->setVisible(visible);
    }
}

void LinePanel::onSetChannelType(int channelIndex, int type)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        QAbstractSeries::SeriesType seriesType = static_cast<QAbstractSeries::SeriesType>(type);
        if (m_series[channelIndex]->type() != seriesType) {
            QXYSeries *newSeries = nullptr;
            if (seriesType == QAbstractSeries::SeriesType::SeriesTypeLine) {
                newSeries = new QLineSeries();
            } else if (seriesType == QAbstractSeries::SeriesType::SeriesTypeSpline) {
                newSeries = new QSplineSeries();
            } else if (seriesType == QAbstractSeries::SeriesType::SeriesTypeScatter) {
                newSeries = new QScatterSeries();
            } else {
                qWarning() << "Unknown series type.";
            }

            if (newSeries) {
                auto *oldSeries = m_series[channelIndex];
                m_series[channelIndex] = newSeries;
                m_chart->addSeries(newSeries);

                newSeries->setName(oldSeries->name());
                newSeries->setVisible(oldSeries->isVisible());
                newSeries->setColor(oldSeries->color());
                newSeries->replace(oldSeries->points());
                newSeries->attachAxis(m_axisX);
                newSeries->attachAxis(m_axisY);
                newSeries->setUseOpenGL(true);

                oldSeries->setParent(nullptr);
                oldSeries->deleteLater();
                oldSeries = newSeries;
                Q_UNUSED(oldSeries);

                for (auto &series : m_series) {
                    m_chart->removeSeries(series);
                }

                for (auto &series : m_series) {
                    m_chart->addSeries(series);
                }
            }
        }
    }
}

void LinePanel::onSetChannelColor(int channelIndex, const QColor &color)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        m_series[channelIndex]->setColor(color);
    }
}

void LinePanel::onSetChannelName(int channelIndex, const QString &name)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        m_series[channelIndex]->setName(name);
    }
}
