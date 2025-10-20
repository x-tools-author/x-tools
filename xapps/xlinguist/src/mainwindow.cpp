/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QLocale>
#include <QMetaEnum>

#include "common/iconengine.h"
#include "x/xapp.h"

MainWindow::MainWindow(QWidget *parent)
    : xUi(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonStart, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->pushButtonStop, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);
    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &MainWindow::onBrowseButtonClicked);

    m_rootPath = xAPP->value(m_keys.lastOpenedDirectory, QDir::currentPath()).toString();
    loadTranslationFiles(m_rootPath);

    ui->comboBoxEngine->addItem(tr("Google Translate"), "google");
    ui->comboBoxLanguage->addItem(tr("Auto"), QString("auto"));
    ui->comboBoxLanguage->setMaxVisibleItems(50);
#if 0
    QMetaEnum territoryEnum = QMetaEnum::fromType<QLocale::Territory>();
    ui->comboBoxLanguage->insertSeparator(1);
    for (int i = 0; i < territoryEnum.keyCount(); ++i) {
        QLocale locale(static_cast<QLocale::Language>(territoryEnum.value(i)));
        QString nativeLanguageName = locale.nativeLanguageName();
        if (nativeLanguageName.isEmpty()) {
            continue;
        }

        if (ui->comboBoxLanguage->findData(locale.name()) == -1) {
            ui->comboBoxLanguage->addItem(nativeLanguageName, locale.name());
        }
    }
#endif
    ui->toolButtonRemove->setIcon(xIcon(":/res/icons/remove.svg"));
    setWindowTitle(QString("xLinguist") + QString(" - ") + tr("Translation Assistant"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartButtonClicked()
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item) {
            QString fullFilePath = QDir(m_rootPath).filePath(item->text());
            qInfo() << "Translating file:" << fullFilePath;
        }
    }
}

void MainWindow::onStopButtonClicked()
{
    // Stop button clicked logic
}

void MainWindow::onBrowseButtonClicked()
{
    QString tmp = xAPP->value(m_keys.lastOpenedDirectory, QDir::currentPath()).toString();
    m_rootPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), tmp);
    if (m_rootPath.isEmpty()) {
        return;
    }

    xAPP->setValue(m_keys.lastOpenedDirectory, m_rootPath);
    loadTranslationFiles(m_rootPath);
}

void MainWindow::loadTranslationFiles(const QString &dir)
{
    ui->listWidget->clear();
    QDir directory(dir);
    directory.setNameFilters(QStringList() << "*.ts");
    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    if (fileInfoList.isEmpty()) {
        return;
    }

    for (const QFileInfo &info : std::as_const(fileInfoList)) {
        ui->listWidget->addItem(info.fileName());
    }

    ui->listWidget->setCurrentRow(0);
}
