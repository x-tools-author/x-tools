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

#include <QDesktopServices>

#include "scriptrunner.h"

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

QString ScriptBase::helpUrl() const
{
    return QString();
}

void ScriptBase::onRunButtonClicked(bool checked)
{
    ui->toolButtonOpen->setEnabled(false);
    if (checked) {
        startRunner();
    } else {
        stopRunner();
    }
}

void ScriptBase::onNewButtonClicked() {}

void ScriptBase::onOpenButtonClicked() {}

void ScriptBase::onRefreshButtonClicked() {}

void ScriptBase::onHelpButtonClicked()
{
    QString url = helpUrl();
    if (url.isEmpty()) {
        return;
    }

    QDesktopServices::openUrl(QUrl(url));
}

void ScriptBase::startRunner()
{
    stopRunner();

    m_runner = newRunner();
    if (!m_runner) {
        return;
    }

    connect(m_runner, &QThread::finished, this, [this]() { ui->toolButtonOpen->setEnabled(true); });
    connect(m_runner, &QThread::started, this, [this]() { ui->toolButtonOpen->setEnabled(true); });
    m_runner->start();
}

void ScriptBase::stopRunner()
{
    if (m_runner) {
        m_runner->requestInterruption();
        m_runner->exit();
        m_runner->wait();
        m_runner->deleteLater();
        m_runner = nullptr;
    }
}