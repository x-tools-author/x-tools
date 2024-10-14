/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>
#include <QXYSeries>

#include "IO/UI/AbstractIOUi.h"

namespace Ui {
class ChartsUi;
}

namespace xTools {

class ChartsUiSettings;
class ChartsUi : public AbstractIOUi
{
    Q_OBJECT
public:
    explicit ChartsUi(QWidget *parent = Q_NULLPTR);
    ~ChartsUi() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupIO(AbstractIO *io) override;

    QMenu *settingsMenu() const;

private:
    Ui::ChartsUi *ui;
    ChartsUiSettings *m_settings;
    QMenu *m_settingsMenu;
    QList<QXYSeries *> m_series;

private:
    void onNewValues(const QList<double> &values);
    void onNewPoints(const QList<QPointF> &points);
};

} // namespace xTools
