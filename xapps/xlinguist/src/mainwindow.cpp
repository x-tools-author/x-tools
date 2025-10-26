/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaEnum>
#include <QThreadPool>

#include "common/iconengine.h"
#include "x/xapp.h"

#include "translator.h"
#include "tsfile.h"
#include "tsfilemanager.h"
#include "tsfileview.h"
#include "tsitem.h"

MainWindow::MainWindow(QWidget *parent)
    : xUi(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    ui->pushButtonStop->setEnabled(false);
    ui->comboBoxEngine->addItem(tr("Google Translate"), "google");
    ui->comboBoxLanguage->addItem(tr("Auto"), QString("auto"));
    ui->comboBoxLanguage->setMaxVisibleItems(50);
    ui->tableView->setModel(&TsFileManager::instance());

    setWindowTitle(QString("xLinguist") + QString(" - ") + tr("A Translation Assistant"));
    connect(ui->pushButtonStart, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->pushButtonStop, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);
    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &MainWindow::onBrowseButtonClicked);
    connect(ui->pushButtonRemove, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);
    connect(ui->tableView, &QTableView::doubleClicked, this, &MainWindow::onViewDoubleClicked);
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MainWindow::onOpenButtonClicked);
    connect(ui->pushButtonSave, &QPushButton::clicked, this, &MainWindow::onSavePushButtonClicked);

    m_checkThreadPoolTimer = new QTimer(this);
    m_checkThreadPoolTimer->setInterval(1000);
    connect(m_checkThreadPoolTimer, &QTimer::timeout, this, &MainWindow::onCheckThreadPoolTimeout);

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

    m_rootPath = xAPP->value(m_keys.lastOpenedDirectory, QDir::currentPath()).toString();
    m_lastOpenedFile = xAPP->value(m_keys.lastOpenedFile, QString()).toString();
    loadTranslationFiles(m_rootPath, m_lastOpenedFile);

    QMenu *fileMenu = new QMenu(tr("File"), this);
    menuBar()->insertMenu(m_optionMenu->menuAction(), fileMenu);
    // clang-format off
    fileMenu->addAction(ui->pushButtonSave->text(), this, &MainWindow::onSavePushButtonClicked, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->pushButtonOpen->text(), this, &MainWindow::onOpenButtonClicked, QKeySequence::Open);
    fileMenu->addAction(ui->pushButtonBrowse->text(), this, &MainWindow::onBrowseButtonClicked, QKeySequence::New);
    fileMenu->addSeparator();
    fileMenu->addAction(ui->pushButtonRemove->text(), this, &MainWindow::onRemoveButtonClicked, QKeySequence::Delete);
    fileMenu->addAction(ui->pushButtonStart->text(), this, &MainWindow::onStartButtonClicked, QKeySequence::Refresh);
    fileMenu->addAction(ui->pushButtonStop->text(), this, &MainWindow::onStopButtonClicked, QKeySequence::Cancel);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &QWidget::close, QKeySequence::Quit);
    // clang-format on
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    onStopButtonClicked();
    QMainWindow::closeEvent(event);
}

void MainWindow::onStartButtonClicked()
{
    if (QThreadPool::globalInstance()->activeThreadCount() > 0) {
        return;
    }

    Translator::setRequestInterrupted(false);
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    m_translators.clear();

    auto tsFileViews = gTsFileMgr.tsFileViews();
    for (TsFileView *view : tsFileViews) {
        TsFile *tsFile = view->tsFile();
        auto tsItems = tsFile->tsItems();
        for (TsItem *item : tsItems) {
            if (item->isFinishedTranslation()) {
                continue;
            }

            if (!item->isSource()) {
                continue;
            }

            QString sourceText = item->cookedText();
            QString fromLanguage = ui->comboBoxLanguage->currentData().toString();
            QString toLanguage = tsFile->targetLanguage();
            Translator *translator = new Translator(fromLanguage,
                                                    toLanguage,
                                                    tsFile->filePath(),
                                                    sourceText,
                                                    item->lineNumber());
            m_translators.append(translator);
            QThreadPool::globalInstance()->start(translator);
        }
    }

    m_checkThreadPoolTimer->start();
}

void MainWindow::onStopButtonClicked()
{
    m_checkThreadPoolTimer->stop();
    ui->pushButtonStop->setEnabled(false);
    Translator::setRequestInterrupted(true);
    QThreadPool::globalInstance()->waitForDone();
    ui->pushButtonStart->setEnabled(true);
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

void MainWindow::onOpenButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open Translation File"),
                                                    m_rootPath,
                                                    tr("Translation Files (*.ts)"));
    if (filePath.isEmpty()) {
        return;
    }

    m_lastOpenedFile = filePath;
    xAPP->setValue(m_keys.lastOpenedFile, m_lastOpenedFile);
    QFileInfo fileInfo(m_lastOpenedFile);
    loadTranslationFiles(fileInfo.absolutePath(), m_lastOpenedFile);
}

void MainWindow::onRemoveButtonClicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please select an item to remove."));
        return;
    }

    int ret = QMessageBox::question(
        this,
        tr("Confirm Removal"),
        tr("Are you sure you want to remove the selected translation file?"),
        QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    int row = index.row();
    TsFileView *tsFileView = gTsFileMgr.tsFileViewAt(row);
    int tabIndex = ui->tabWidget->indexOf(tsFileView);
    if (tsFileView && tabIndex != -1) {
        ui->tabWidget->removeTab(tabIndex);
    }

    gTsFileMgr.removeRows(row, 1);
}

void MainWindow::onViewDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    int row = index.row();
    int tabIndex = row + 1;
    if (tabIndex < ui->tabWidget->count()) {
        ui->tabWidget->setCurrentIndex(tabIndex);
    }
}

void MainWindow::onCheckThreadPoolTimeout()
{
    if (QThreadPool::globalInstance()->activeThreadCount() == 0) {
        onStopButtonClicked();
    }
}

void MainWindow::onSavePushButtonClicked() {}

void MainWindow::loadTranslationFiles(const QString &dir, const QString &specifiedFile)
{
    for (int i = 1; i < ui->tabWidget->count(); ++i) {
        QWidget *tab = ui->tabWidget->widget(i);
        if (tab) {
            ui->tabWidget->removeTab(i);
            --i;
        }
    }

    gTsFileMgr.loadTranslationFiles(dir, specifiedFile);

    const QList<TsFileView *> tsFileViews = gTsFileMgr.tsFileViews();
    for (TsFileView *view : tsFileViews) {
        ui->tabWidget->addTab(view, view->windowTitle());
    }
}
