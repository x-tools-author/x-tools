/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "plotpanel.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>

#include "../common/plotdatahandler.h"
#include "plotpanelpalette.h"

PlotPanel::PlotPanel(QWidget *parent)
    : Panel(parent)
    , m_dataHandler(nullptr)
{
    m_plot = new QCustomPlot(this);
    m_plot->setBackground(qApp->palette().button());
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_plot);
    setLayout(layout);

    connect(&PlotPanelPalette::singleton(), &PlotPanelPalette::paletteChanged, this, [this]() {
        m_plot->setBackground(qApp->palette().button());
        m_plot->replot();
    });

    m_dataHandler = new PlotDataHandler(this);
    connect(m_dataHandler, &PlotDataHandler::newValues, this, [this](const QList<double> &values) {
        this->onNewValues(values);
    });
    m_dataHandler->start();
}

PlotPanel::~PlotPanel() {}

void PlotPanel::inputBytes(const QByteArray &bytes)
{
    m_dataHandler->inputBytes(bytes);
}

void PlotPanel::onNewValues(const QList<double> &values)
{
    Q_UNUSED(values);
}