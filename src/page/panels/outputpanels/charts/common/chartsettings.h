/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

class PlotSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ChartSettings(QWidget *parent = Q_NULLPTR);
    ~PlotSettings() override;

    virtual QVariantMap save() const = 0;
    virtual void load(const QVariantMap &parameters) = 0;
};