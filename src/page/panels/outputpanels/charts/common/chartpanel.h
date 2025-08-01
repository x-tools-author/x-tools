/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QPointF>
#include <QVariantMap>

#include "../../../common/panel.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
using QtCharts::QChart;
using QtCharts::QChartView;
#endif

class ChartSettings;
class ChartDataHandler;
class ChartPanel : public Panel
{
    Q_OBJECT
public:
    enum class DataFormat { BinaryY, BinaryXY, TextY, TextXY };
    Q_ENUM(DataFormat);

public:
    explicit ChartPanel(QWidget *parent = Q_NULLPTR);
    ~ChartPanel() override;
    void inputBytes(const QByteArray &bytes) override;

    virtual void resetChart() = 0;

    ChartDataHandler *chartDataHandler() const;

protected:
    virtual void onNewValues(const QList<double> &values) = 0;
    virtual void onNewPoints(const QList<QPointF> &points) = 0;

protected:
    ChartDataHandler *m_chartDataHandler;
    QChart *m_chart;
    QChartView *m_chartView;
};
