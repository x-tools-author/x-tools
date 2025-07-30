/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "lineplotpanel.h"

#include "lineplotsettings.h"

LinePlotPanel::LinePlotPanel(QWidget *parent)
    : PlotPanel(parent)
    , m_settings(nullptr)
{
    for (int i = 0; i < m_maxChannels; ++i) {
        QCPCurve *curve = new QCPCurve(m_plot->xAxis, m_plot->yAxis);
        curve->setName(QString("ch %1").arg(i + 1));
        m_curves.append(curve);
    }
}

LinePlotPanel::~LinePlotPanel()
{
    // Destructor implementation can be added here if needed
}

QWidget *LinePlotPanel::menuWidget()
{
    if (!m_settings) {
        m_settings = new LinePlotSettings(this);
    }

    return m_settings;
}