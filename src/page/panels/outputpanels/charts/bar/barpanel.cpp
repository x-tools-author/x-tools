/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "barpanel.h"

#include <xlsxdocument.h>

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

#include "barsettings.h"

BarPanel::BarPanel(QWidget *parent)
    : ChartPanel(parent)
{
    m_settings = new BarSettings();
    connect(m_settings, &BarSettings::channelColorChanged, this, &BarPanel::onChannelColorChanged);
    connect(m_settings, &BarSettings::channelNameChanged, this, &BarPanel::onChannelNameChanged);

    m_axisX = new QBarCategoryAxis();
    QStringList labels;
    const int channelCount = 16;
    for (int i = 0; i < channelCount; ++i) {
        labels.append(QString::number(i + 1));
    }
    m_axisX->setCategories(labels);
    m_axisX->append(labels);
    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 17);

    m_chart->legend()->hide();
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setMargins(QMargins(0, 0, 0, 0));

    m_barSeries = new QStackedBarSeries(this);
    m_chart->addSeries(m_barSeries);
    m_barSeries->setUseOpenGL(true);
    m_barSeries->attachAxis(m_axisX);
    m_barSeries->attachAxis(m_axisY);
    for (int i = 0; i < channelCount; ++i) {
        QBarSet *set = new QBarSet("", m_chart);

        for (int j = 0; j < 16; ++j) {
            set->append(0);
        }

        m_barSeries->append(set);
        m_barSets.append(set);
    }

    for (int i = 0; i < channelCount; i++) {
        m_barSets[i]->replace(i, i + 1);
    }

    m_settings->load(BarPanel::save());
}

BarPanel::~BarPanel()
{
    m_settings->deleteLater();
}

QWidget *BarPanel::menuWidget()
{
    return m_settings;
}

QVariantMap BarPanel::save() const
{
    QVariantMap data;

    BarSettingsKeys keys;
    data[keys.dataFormat] = m_settings->dataType();
    QJsonArray channels;
    for (int i = 0; i < m_barSets.size(); ++i) {
        QJsonObject obj;
        obj[keys.channel.channelName] = m_axisX->categories().at(i);
        obj[keys.channel.channelColor] = m_barSets[i]->color().name();
        channels.append(obj);
    }

    data[keys.channels] = channels;
    return data;
}

void BarPanel::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    BarSettingsKeys keys;
    m_settings->load(parameters);

    QJsonArray channels = parameters.value(keys.channels).toJsonArray();
    if (channels.size() != m_barSets.size()) {
        qWarning() << "The number of channels is not equal to the number of series.";
        return;
    }

    QStringList categories;
    for (int i = 0; i < channels.size(); ++i) {
        QJsonObject obj = channels[i].toObject();
        QString name = obj.value(keys.channel.channelName).toString();
        QString color = obj.value(keys.channel.channelColor).toString();

        categories.append(name);
        QBarSet *barSet = m_barSets[i];
        barSet->setColor(color);
    }

    m_axisX->setCategories(categories);
}

void BarPanel::resetChart()
{
    for (int i = 0; i < m_barSets.size(); ++i) {
        m_barSets[i]->replace(i, 0);
    }

    m_axisY->setRange(0, 1);
}

void BarPanel::onNewValues(const QList<double> &values)
{
    int count = qMin(values.size(), m_barSets.size());
    for (int i = 0; i < count; ++i) {
        m_barSets[i]->replace(i, values[i]);
        if (values[i] > m_axisY->max()) {
            m_axisY->setMax(values[i] + 1);
        }

        if (values[i] < m_axisY->min()) {
            m_axisY->setMin(values[i] - 1);
        }
    }
}

void BarPanel::onNewPoints(const QList<QPointF> &points)
{
    Q_UNUSED(points);
}

void BarPanel::onChannelColorChanged(int channelIndex, const QColor &color)
{
    if (channelIndex >= 0 && channelIndex < m_barSets.size()) {
        m_barSets[channelIndex]->setColor(color);
    }
}

void BarPanel::onChannelNameChanged(int channelIndex, const QString &name)
{
    QStringList categories = m_axisX->categories();
    if (channelIndex >= 0 && m_barSets.size() > channelIndex) {
        categories[channelIndex] = name;
        m_axisX->setCategories(categories);
    }
}
