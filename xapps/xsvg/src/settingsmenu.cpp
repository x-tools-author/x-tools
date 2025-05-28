/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SettingsMenu.h"
#include "ui_SettingsMenu.h"

#include "Common/xTools.h"

SettingsMenu::SettingsMenu(QWidget *parent)
    : QWidget{parent}
    , ui{new Ui::SettingsMenu}
{
    ui->setupUi(this);
    int padding = g_xTools.settings()->value(m_settingKeys.padding, 0).toInt();
    ui->spinBoxPadding->setValue(padding);
    connect(ui->spinBoxPadding,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            [this](int value) { g_xTools.settings()->setValue(m_settingKeys.padding, value); });

    bool ret = g_xTools.settings()->value(m_settingKeys.enablePadding, false).toBool();
    ui->checkBoxEnable->setChecked(ret);
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    connect(ui->checkBoxEnable, &QCheckBox::checkStateChanged, this, [this](int state) {
#else
    connect(ui->checkBoxEnable, &QCheckBox::stateChanged, this, [this](int state) {
#endif
        g_xTools.settings()->setValue(m_settingKeys.enablePadding, state == Qt::Checked);
    });
}

SettingsMenu::~SettingsMenu()
{
    delete ui;
}

bool SettingsMenu::enablePadding()
{
    return ui->checkBoxEnable->isChecked();
}

int SettingsMenu::padding()
{
    return ui->spinBoxPadding->value();
}
