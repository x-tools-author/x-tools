/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <qcustomplot.h>

#include "../../../common/panel.h"
#include "../common/plotdatahandler.h"

class PlotPanel : public Panel
{
    Q_OBJECT
public:
    explicit PlotPanel(QWidget *parent = nullptr);
    ~PlotPanel() override;
    void inputBytes(const QByteArray &bytes) override;

protected:
    QCustomPlot *m_plot;
    const int m_maxChannels = 16;

protected:
    virtual void onNewValues(const QList<double> &values);

private:
    PlotDataHandler *m_dataHandler;
};