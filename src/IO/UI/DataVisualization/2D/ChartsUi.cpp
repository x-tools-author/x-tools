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

#include "ChartsUiSettings.h"
#include "Common/xTools.h"
#include "IO/IO/DataVisualization/2D/Charts.h"

namespace xTools {

ChartsUi::ChartsUi(QWidget *parent)
    : AbstractIOUi(parent)
    , ui(new Ui::ChartsUi)
{
    ui->setupUi(this);
    ui->widgetChartView->setContentsMargins(0, 0, 0, 0);
    ui->widgetChartView->setRenderHint(QPainter::Antialiasing);
    ui->widgetChartView->setAttribute(Qt::WA_TranslucentBackground);
    ui->widgetChartView->viewport()->setAttribute(Qt::WA_TranslucentBackground);

    m_settings = new ChartsUiSettings();

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
    m_axisY->setRange(0, 1);

    m_chart = new QChart();
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setMargins(QMargins(0, 0, 0, 0));

    xTools &xTools = xTools::singleton();
    auto currentScheme = xTools.settingsColorScheme();
    if (currentScheme == static_cast<int>(Qt::ColorScheme::Dark)) {
        m_chart->setTheme(QChart::ChartThemeDark);
    } else if (currentScheme == static_cast<int>(Qt::ColorScheme::Light)) {
        m_chart->setTheme(QChart::ChartThemeLight);
    }

    ui->widgetChartView->setChart(m_chart);
    int channelCount = ChartsUiSettings::channelCount();
    for (int i = 0; i < channelCount; ++i) {
        QLineSeries *series = new QLineSeries();
        m_chart->addSeries(series);
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);
        series->setName(QString::number(i + 1));
        series->setUseOpenGL(true);
        m_series.append(series);
    }

    m_settings->load(ChartsUi::save());
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

void ChartsUi::load(const QVariantMap &parameters)
{
    if (parameters.isEmpty()) {
        return;
    }

    ChartsUiDataKeys keys;
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
    connect(io, &AbstractIO::started, this, [this]() {
        this->m_settings->updateUiState(true);
        onClearChannels();
    });
    connect(io, &AbstractIO::finished, this, [this]() { this->m_settings->updateUiState(false); });
}

QMenu *ChartsUi::settingsMenu()
{
    if (!m_settingsMenu) {
        m_settingsMenu = new QMenu(this);
        QWidgetAction *action = new QWidgetAction(m_settingsMenu);
        action->setDefaultWidget(m_settings);
        m_settingsMenu->addAction(action);
    }

    return m_settingsMenu;
}

QWidget *ChartsUi::settingsWidget()
{
    return m_settings;
}

void ChartsUi::updateChartsTheme(bool darkMode)
{
    m_chart->setTheme(darkMode ? QChart::ChartThemeDark : QChart::ChartThemeLight);
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

    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 1);
}

void ChartsUi::onImportChannels()
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

void ChartsUi::onExportChannels()
{
    const QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                          tr("Export Data to Excel"),
                                                          "data.xlsx",
                                                          "Excel(*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    QXlsx::Document xlsx;
    for (int i = 0; i < ChartsUiSettings::channelCount(); ++i) {
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

void ChartsUi::onNewValues(const QList<double> &values)
{
    int count = qMin(values.size(), m_series.size());
    for (int i = 0; i < count; ++i) {
        double value = values[i];
        QXYSeries *series = m_series[i];

        QPointF pos = QPointF(series->count(), value);
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
}

void ChartsUi::onNewPoints(const QList<QPointF> &points)
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

                for (auto series : m_series) {
                    m_chart->removeSeries(series);
                }

                for (auto series : m_series) {
                    m_chart->addSeries(series);
                }
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
