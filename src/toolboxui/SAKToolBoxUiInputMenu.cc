/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QWidgetAction>
#include "SAKToolBoxUiInputMenu.hh"
#include "ui_SAKToolBoxUiInputMenu.h"

SAKToolBoxUiInputMenu::SAKToolBoxUiInputMenu(const QString &settingsGroup,
                                             QWidget *parent)
    : QMenu{parent}
    , ui(new Ui::SAKToolBoxUiInputMenu)
{
    QWidget *w = new QWidget(this);
    ui->setupUi(w);
    QWidgetAction *action = new QWidgetAction(w);
    action->setDefaultWidget(w);
    addAction(action);

    ui->comboBoxPrefix->setGroupKey(settingsGroup + "/input", "prefix", false);
    mParameters.prefix = ui->comboBoxPrefix->currentData().toInt();
    connect(ui->comboBoxPrefix,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, [=](){
                int ret = ui->comboBoxPrefix->currentData().toInt();
                this->mParameters.prefix = ret;
                emit parametersChanged();
            });

    ui->comboBoxSuffix->setGroupKey(settingsGroup + "/input", "suffix");
    mParameters.suffix = ui->comboBoxSuffix->currentData().toInt();
    connect(ui->comboBoxSuffix,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, [=](){
                int ret = ui->comboBoxSuffix->currentData().toInt();
                this->mParameters.suffix = ret;
                emit parametersChanged();
            });

    ui->comboBoxEscape->setGroupKey(settingsGroup + "/input", "escape");
    mParameters.escapeCharacter = ui->comboBoxEscape->currentData().toInt();
    connect(ui->comboBoxEscape,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, [=](){
                int ret = ui->comboBoxEscape->currentData().toInt();
                this->mParameters.escapeCharacter = ret;
                emit parametersChanged();
            });

    ui->checkBoxAppendCrc->setGroupKey(settingsGroup + "/input",
                                       "crcAppend");
    mParameters.appendCrc = ui->checkBoxAppendCrc->isChecked();
    connect(ui->checkBoxAppendCrc, &QCheckBox::clicked, this, [=](){
        this->mParameters.appendCrc = ui->checkBoxAppendCrc->isChecked();
    });

    ui->checkBoxBigEndian->setGroupKey(settingsGroup + "/input",
                                       "crcBigEndian");
    mParameters.bigEndian = ui->checkBoxBigEndian->isChecked();
    connect(ui->checkBoxBigEndian, &QCheckBox::clicked, this, [=](){
        this->mParameters.bigEndian = ui->checkBoxBigEndian->isChecked();
        emit parametersChanged();
    });

    ui->spinBoxStartIndex->setGroupKey(settingsGroup + "/input",
                                       "startIndex");
    mParameters.startIndex = ui->spinBoxStartIndex->value();
    connect(ui->spinBoxStartIndex, &QSpinBox::valueChanged, this, [=](){
        this->mParameters.startIndex = ui->spinBoxStartIndex->value();
        emit parametersChanged();
    });

    ui->spinBoxEndIndex->setGroupKey(settingsGroup + "/input",
                                     "endIndex");
    mParameters.endIndex = ui->spinBoxEndIndex->value();
    connect(ui->spinBoxEndIndex, &QSpinBox::valueChanged, this, [=](){
        this->mParameters.endIndex = ui->spinBoxEndIndex->value();
        emit parametersChanged();
    });


    ui->comboBoxAglorithm->setGroupKey(settingsGroup + "/input",
                                       "algorithm");
    mParameters.algorithm = ui->comboBoxAglorithm->currentData().toInt();
    connect(ui->comboBoxAglorithm,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, [=](){
                int ret = ui->comboBoxAglorithm->currentData().toInt();
                this->mParameters.algorithm = ret;
                emit parametersChanged();
            });
}

SAKToolBoxUiInputMenu::~SAKToolBoxUiInputMenu()
{
    delete ui;
}

SAKToolBoxUiInputMenu::Parameters SAKToolBoxUiInputMenu::parameters()
{
    return mParameters;
}
