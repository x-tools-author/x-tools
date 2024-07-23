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

class StatisticianUi : public AbstractIOUi
{
    Q_OBJECT
public:
    StatisticianUi(QWidget *parent = nullptr);
    ~StatisticianUi();

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;

private:
    Ui::StatisticianUi *ui;

private:
    void updateInfo(int frame, int bytes, int speed);
};
