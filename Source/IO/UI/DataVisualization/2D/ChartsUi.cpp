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

#include <QChart>
#include <QChartView>
#include <QCheckBox>
#include <QLineSeries>
#include <QMenu>
#include <QPointF>
#include <QPushButton>
#include <QTimer>
#include <QValueAxis>
#include <QWidgetAction>

#include "ChartsUiSettings.h"
#include "IO/IO/DataVisualization/2D/Charts.h"

namespace xTools {

struct ChartsUiDataKeys
{
    const QString dataType{"dataType"};
    const QString channelCount{"channelCount"};
};

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

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 100);
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);

    QChart *chart = new QChart();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->widgetChartView->setChart(chart);
    ui->widgetChartView->setRenderHint(QPainter::Antialiasing);

    int channelCount = ChartsUiSettings::channelCount();
    for (int i = 0; i < channelCount; ++i) {
        QLineSeries *series = new QLineSeries();
        chart->addSeries(series);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        series->setName("CH" + QString::number(i + 1));
        series->append(QPointF(0, 0));
        series->append(QPointF(100, 100 - 5 * i));

        m_series.append(series);
    }
}

ChartsUi::~ChartsUi()
{
    m_settings->deleteLater();
    delete ui;
}

QVariantMap ChartsUi::save() const
{
    QVariantMap data;
    return data;
}

void ChartsUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void ChartsUi::setupIO(AbstractIO *io)
{
    AbstractIOUi::setupIO(io);

    auto *charts = qobject_cast<Charts *>(io);
    if (!charts) {
        return;
    }

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

void ChartsUi::onNewPoints(const QList<QPointF> &points)
{
    int count = qMin(points.size(), m_series.size());
    for (int i = 0; i < count; ++i) {
        m_series[i]->append(points[i]);
    }
}

} // namespace xTools
