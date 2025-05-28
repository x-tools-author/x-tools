/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class SettingsMenu;
}

class SettingsMenu : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsMenu(QWidget *parent = nullptr);
    ~SettingsMenu();

    bool enablePadding();
    int padding();

private:
    const struct
    {
        const QString enablePadding = "enablePadding";
        const QString padding = "padding";
    } m_settingKeys;

private:
    Ui::SettingsMenu *ui;
};
