/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "../AbstractIOUi.h"

namespace Ui {
class StatisticianUi;
}

class Statistician;
class StatisticianUi : public AbstractIOUi
{
    Q_OBJECT
public:
    StatisticianUi(QWidget *parent = nullptr);
    ~StatisticianUi();

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupIO(AbstractIO *io) override;

private:
    Ui::StatisticianUi *ui;
    Statistician *m_statistician;

private:
    void updateInfo();
};
