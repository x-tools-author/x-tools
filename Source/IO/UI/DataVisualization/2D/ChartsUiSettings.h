/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QPushButton>

namespace Ui {
class ChartsUiSettings;
}

namespace xTools {

class ChartsUiSettings : public QPushButton
{
    Q_OBJECT
public:
    explicit ChartsUiSettings(QWidget *parent = Q_NULLPTR);
    ~ChartsUiSettings() override;

private:
    Ui::ChartsUiSettings *ui;
};

} // namespace xTools
