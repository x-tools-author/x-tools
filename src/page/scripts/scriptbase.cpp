/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "scriptbase.h"
#include "ui_scriptbase.h"

ScriptBase::ScriptBase(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScriptBase)
{
    ui->setupUi(this);

    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &ScriptBase::onRunButtonClicked);
    connect(ui->toolButtonNew, &QToolButton::clicked, this, &ScriptBase::onNewButtonClicked);
    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &ScriptBase::onOpenButtonClicked);
    connect(ui->toolButtonRefresh, &QToolButton::clicked, this, &ScriptBase::onRefreshButtonClicked);
    connect(ui->toolButtonHelp, &QToolButton::clicked, this, &ScriptBase::onHelpButtonClicked);
}

ScriptBase::~ScriptBase()
{
    delete ui;
}

ScriptRunner *ScriptBase::newRunner()
{
    return nullptr;
}

void ScriptBase::onRunButtonClicked(bool checked)
{
    if (checked) {
        startRunner();
    } else {
        stopRunner();
    }
}

void ScriptBase::onNewButtonClicked() {}

void ScriptBase::onOpenButtonClicked() {}

void ScriptBase::onRefreshButtonClicked() {}

void ScriptBase::onHelpButtonClicked() {}

void ScriptBase::startRunner()
{
    stopRunner();
}

void ScriptBase::stopRunner() {}