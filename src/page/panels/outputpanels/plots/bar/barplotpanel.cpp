/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "barplotpanel.h"

BarPlotPanel::BarPlotPanel(QWidget *parent)
    : PlotPanel(parent)
{
    QCPBars *bars = new QCPBars(m_plot->xAxis, m_plot->yAxis);
    bars->setPen(QPen(Qt::red));
    QVector<double> xData, yData;
    for (int i = 0; i < 10; ++i) {
        xData.append(i);
        yData.append(i);
    }
    bars->setData(xData, yData);
}

BarPlotPanel::~BarPlotPanel()
{
    // Destructor implementation can be added here if needed
}