/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "ChartsUi.h"
#include "ui_ChartsUi.h"

#include <QChartView>
#include <QCheckBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QLineSeries>
#include <QMenu>
#include <QPointF>
#include <QPushButton>
#include <QScatterSeries>
#include <QSplineSeries>
#include <QTimer>
#include <QWidgetAction>

#include "ChartsUiSettings.h"
#include "IO/IO/DataVisualization/2D/Charts.h"

namespace xTools {

ChartsUi::ChartsUi(QWidget *parent)
    : AbstractIOUi(parent)
    , ui(new Ui::ChartsUi)
{
    ui->setupUi(this);
    ui->widgetChartView->setContentsMargins(0, 0, 0, 0);

    m_settings = new ChartsUiSettings();
    m_settingsMenu = new QMenu(this);
    QWidgetAction *action = new QWidgetAction(m_settingsMenu);
    action->setDefaultWidget(m_settings);
    m_settingsMenu->addAction(action);

    // clang-format off
    connect(m_settings, &ChartsUiSettings::invokeSetDataType, this, &ChartsUi::onSetDataType);
    connect(m_settings, &ChartsUiSettings::invokeSetLegendVisible, this, &ChartsUi::onSetLegendVisible);
    connect(m_settings, &ChartsUiSettings::invokeClearChannels, this, &ChartsUi::onClearChannels);
    connect(m_settings, &ChartsUiSettings::invokeImportChannels, this, &ChartsUi::onImportChannels);
    connect(m_settings, &ChartsUiSettings::invokeExportChannels, this, &ChartsUi::onExportChannels);
    connect(m_settings, &ChartsUiSettings::invokeSetChannelVisible, this, &ChartsUi::onSetChannelVisible);
    connect(m_settings, &ChartsUiSettings::invokeSetChannelType, this, &ChartsUi::onSetChannelType);
    connect(m_settings, &ChartsUiSettings::invokeSetChannelColor, this, &ChartsUi::onSetChannelColor);
    connect(m_settings, &ChartsUiSettings::invokeSetChannelName, this, &ChartsUi::onSetChannelName);
    // clang-format on

    m_axisX = new QValueAxis();
    m_axisX->setRange(0, 100);
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 100);

    m_chart = new QChart();
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    ui->widgetChartView->setChart(m_chart);
    ui->widgetChartView->setRenderHint(QPainter::Antialiasing);

    int channelCount = ChartsUiSettings::channelCount();
    for (int i = 0; i < channelCount; ++i) {
        QLineSeries *series = new QLineSeries();
        m_chart->addSeries(series);
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);
        series->setName(tr("Channel") + QString::number(i + 1));
        series->append(QPointF(0, 0));
        series->append(QPointF(100, 100 - 5 * i));

        m_series.append(series);
    }
}

ChartsUi::~ChartsUi()
{
    m_chart->deleteLater();
    m_settings->deleteLater();
    delete ui;
}

QVariantMap ChartsUi::save() const
{
    QVariantMap data;

    ChartsUiDataKeys keys;
    data[keys.dataType] = m_settings->dataType();
    data[keys.legendVisible] = m_settings->legendVisible();
    QJsonArray channels;
    for (int i = 0; i < m_series.size(); ++i) {
        QJsonObject obj;
        obj[keys.channelName] = m_series[i]->name();
        obj[keys.channelVisible] = m_series[i]->isVisible();
        obj[keys.channelColor] = m_series[i]->color().name();
        obj[keys.channelType] = m_series[i]->type();
    }

    data[keys.channels] = channels;
    return data;
}

void ChartsUi::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    ChartsUiDataKeys keys;
    m_settings->setDataType(parameters.value(keys.dataType).toInt());
    QJsonArray channels = parameters.value(keys.channels).toJsonArray();

    if (channels != m_series.size()) {
        qWarning() << "The number of channels is not equal to the number of series.";
        return;
    }

    for (int i = 0; i < channels.size(); ++i) {
        QJsonObject obj = channels[i].toObject();
        QString name = obj.value(keys.channelName).toString();
        QString color = obj.value(keys.channelColor).toString();
        int type = obj.value(keys.channelType).toInt();
        bool visible = channels[i].toObject().value(keys.channelVisible).toBool();

        m_series[i]->setName(name);
        m_series[i]->setVisible(visible);
        m_series[i]->setColor(color);
        if (type != m_series[i]->type()) {
            QXYSeries *newSeries = nullptr;
            if (type == QAbstractSeries::SeriesType::SeriesTypeLine) {
                newSeries = new QLineSeries();
                m_chart->addSeries(m_series[i]);
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

                m_series[i]->setParent(nullptr);
                m_series[i]->deleteLater();
                m_series[i] = newSeries;
            }
        }
    }
}

void ChartsUi::setupIO(AbstractIO *io)
{
    AbstractIOUi::setupIO(io);

    auto *charts = qobject_cast<Charts *>(io);
    if (!charts) {
        return;
    }

    m_io = io;
    int type = Qt::AutoConnection | Qt::UniqueConnection;
    auto cookedType = static_cast<Qt::ConnectionType>(type);
    connect(charts, &Charts::newValues, this, &ChartsUi::onNewValues, cookedType);
    connect(charts, &Charts::newPoints, this, &ChartsUi::onNewPoints, cookedType);
}

QMenu *ChartsUi::settingsMenu() const
{
    return m_settingsMenu;
}

void ChartsUi::onNewValues(const QList<double> &values)
{
    int count = qMin(values.size(), m_series.size());
    for (int i = 0; i < count; ++i) {
        m_series[i]->append(QPointF(m_series[i]->count(), values[i]));
    }
}

void ChartsUi::onSetDataType(int type)
{
    if (m_io) {
        m_io->load(save());
    }
}

void ChartsUi::onSetLegendVisible(bool visible)
{
    m_chart->legend()->setVisible(visible);
}

void ChartsUi::onClearChannels()
{
    for (auto series : m_series) {
        series->clear();
    }
}
void ChartsUi::onImportChannels() {}

void ChartsUi::onExportChannels() {}

void ChartsUi::onNewPoints(const QList<QPointF> &points)
{
    int count = qMin(points.size(), m_series.size());
    for (int i = 0; i < count; ++i) {
        m_series[i]->append(points[i]);
    }
}

void ChartsUi::onSetChannelVisible(int channelIndex, bool visible)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        m_series[channelIndex]->setVisible(visible);
    }
}

void ChartsUi::onSetChannelType(int channelIndex, int type)
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
                newSeries->setName(m_series[channelIndex]->name());
                newSeries->setVisible(m_series[channelIndex]->isVisible());
                newSeries->setColor(m_series[channelIndex]->color());

                m_series[channelIndex]->setParent(nullptr);
                m_series[channelIndex]->deleteLater();
                m_series[channelIndex] = newSeries;
            }
        }
    }
}

void ChartsUi::onSetChannelColor(int channelIndex, const QColor &color)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        m_series[channelIndex]->setColor(color);
    }
}

void ChartsUi::onSetChannelName(int channelIndex, const QString &name)
{
    if (channelIndex >= 0 && channelIndex < m_series.size()) {
        m_series[channelIndex]->setName(name);
    }
}

} // namespace xTools
