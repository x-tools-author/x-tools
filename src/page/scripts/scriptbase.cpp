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
#include <QDir>
#include <QInputDialog>

#include "scriptrunner.h"

ScriptBase::ScriptBase(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScriptBase)
{
    ui->setupUi(this);

    connect(ui->comboBoxFile,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &ScriptBase::onScriptComboBoxCurrentIndexChanged);
    connect(ui->toolButtonOpen, &QToolButton::clicked, this, &ScriptBase::onRunButtonClicked);
    connect(ui->toolButtonNew, &QToolButton::clicked, this, &ScriptBase::onNewButtonClicked);
    connect(ui->toolButtonDir, &QToolButton::clicked, this, &ScriptBase::onOpenButtonClicked);
    connect(ui->toolButtonRefresh, &QToolButton::clicked, this, &ScriptBase::onRefreshButtonClicked);
    connect(ui->toolButtonHelp, &QToolButton::clicked, this, &ScriptBase::onHelpButtonClicked);
}

ScriptBase::~ScriptBase()
{
    delete ui;
}

void ScriptBase::loadScripts()
{
    ui->comboBoxFile->clear();
    loadScriptsApp();
    loadScriptsUser();
}

QStringList ScriptBase::ignoredFiles() const
{
    return QStringList();
}

void ScriptBase::loadScriptsApp()
{
    QString appDir = QApplication::applicationDirPath();
    QString dir = appDir + QString("/scripts/") + scriptDir();
    QDir d(dir);
    if (!d.exists()) {
        return;
    }

    QStringList filters;
    filters << ("*." + scriptSuffix());
    QFileInfoList files = d.entryInfoList(filters, QDir::Files | QDir::Readable, QDir::Name);
    QStringList ignored = ignoredFiles();
    for (const QFileInfo &f : files) {
        const QString fileName = f.baseName();
        if (ignored.contains(f.fileName(), Qt::CaseInsensitive)) {
            continue;
        }

        ui->comboBoxFile->addItem(fileName, f.absoluteFilePath());
    }
}

void ScriptBase::loadScriptsUser() {}

void ScriptBase::onScriptComboBoxCurrentIndexChanged()
{
    const QString fileName = ui->comboBoxFile->currentData().toString();
    if (fileName.isEmpty()) {
        ui->textEditScript->clear();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->textEditScript->clear();
        return;
    }

    QByteArray ba = file.readAll();
    ui->textEditScript->setPlainText(QString::fromUtf8(ba));
    file.close();
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

void ScriptBase::onNewButtonClicked()
{
    QString txt = QInputDialog::getText(nullptr,
                                        tr("New Script"),
                                        tr("Please input the script name:"),
                                        QLineEdit::Normal,
                                        QString("NewScript") + QString(".") + scriptSuffix(),
                                        nullptr,
                                        Qt::WindowCloseButtonHint);
    if (!txt.endsWith('.' + scriptSuffix())) {
        txt += '.' + scriptSuffix();
    }

    QString filePath = QApplication::applicationDirPath();
    filePath += QString("/scripts/");
    filePath += scriptDir();
    filePath += '/';
    filePath += txt;
    QFile file(filePath);
    if (file.exists()) {
        return;
    }

    QString fileName = file.fileName();
    ui->comboBoxFile->addItem(txt, fileName);
    ui->comboBoxFile->setCurrentIndex(ui->comboBoxFile->count() - 1);
    file.open(QIODevice::WriteOnly);
    file.close();
}

void ScriptBase::onOpenButtonClicked()
{
    QString filePath = QApplication::applicationDirPath();
    filePath += QString("/scripts/");
    filePath += scriptDir();

    QDesktopServices::openUrl(QUrl(filePath));
}

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

    connect(m_runner, &QThread::finished, this, [this]() {
        ui->toolButtonOpen->setEnabled(true);
        updateUiEnabled(false);
    });
    connect(m_runner, &QThread::started, this, [this]() {
        ui->toolButtonOpen->setEnabled(true);
        updateUiEnabled(true);
    });
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

void ScriptBase::updateUiEnabled(bool running)
{
    ui->toolButtonNew->setEnabled(!running);
    ui->toolButtonRefresh->setEnabled(!running);
    ui->textEditScript->setEnabled(!running);
}