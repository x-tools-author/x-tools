/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKToolBoxUi.hh"
#include "SAKToolBoxUiParameters.hh"

#include "ui_SAKToolBoxUi.h"

SAKToolBoxUi::SAKToolBoxUi(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKToolBoxUi)
{
    ui->setupUi(this);
    mToolBoxUiParameters = new SAKToolBoxUiParameters(this);
    mToolBoxUiParameters->setModal(true);

    mToolBox = new SAKToolBox(this);

}

SAKToolBoxUi::~SAKToolBoxUi()
{
    delete ui;
}

void SAKToolBoxUi::init()
{
    initUi();
    initSettings();
    initSignals();
}

void SAKToolBoxUi::initUi()
{
    initUiCommunication();
    initUiInput();
    initUiOutput();
}

void SAKToolBoxUi::initUiCommunication()
{
    ui->widgetCommunicationController->setLayout(new QHBoxLayout());
    ui->widgetCommunicationController->layout()->setContentsMargins(0, 0, 0, 0);
    QWidget *w = controller();
    if (w) {
        ui->widgetCommunicationController->layout()->addWidget(w);
    } else {
        qCWarning(mLoggingCategory) << "The controller is null!";
    }
}

void SAKToolBoxUi::initUiInput()
{

}

void SAKToolBoxUi::initUiOutput()
{

}

void SAKToolBoxUi::initSettings()
{
    initSettingsCommunication();
    initSettingsInput();
    initSettingsOutput();
}

void SAKToolBoxUi::initSettingsCommunication()
{

}

void SAKToolBoxUi::initSettingsInput()
{

}

void SAKToolBoxUi::initSettingsOutput()
{

}

void SAKToolBoxUi::initSignals()
{
    initSignalsCommunication();
    initSignalsInput();
    initSignalsOutput();
}

void SAKToolBoxUi::initSignalsCommunication()
{
    connect(ui->pushButtonCommunicationSettings, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonCommunicationSettingsClicked);
    connect(ui->pushButtonCommunicationOpen, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonCommunicationOpenClicked);
}

void SAKToolBoxUi::initSignalsInput()
{
    connect(ui->pushButtonInputSettings, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonInputSettingsClicked);
}

void SAKToolBoxUi::initSignalsOutput()
{
    connect(ui->pushButtonOutputSettings, &QPushButton::clicked,
            this, &SAKToolBoxUi::onPushButtonOutputSettingsClicked);
}

void SAKToolBoxUi::onPushButtonCommunicationSettingsClicked()
{
    mToolBoxUiParameters->showDialog(0);
}

void SAKToolBoxUi::onPushButtonCommunicationOpenClicked()
{

}

void SAKToolBoxUi::onPushButtonOutputSettingsClicked()
{
    mToolBoxUiParameters->showDialog(2);
}

void SAKToolBoxUi::onPushButtonInputSettingsClicked()
{
    mToolBoxUiParameters->showDialog(1);
}
