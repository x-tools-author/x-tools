/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsFileMergeAssistant.h"
#include "ui_xToolsFileMergeAssistant.h"

#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

xToolsFileMergeAssistant::xToolsFileMergeAssistant(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::xToolsFileMergeAssistant)
{
    ui->setupUi(this);
    ui->listWidget->setAlternatingRowColors(true);
    ui->progressBar->hide();
    m_desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    connect(ui->pushButtonImport,
            &QPushButton::clicked,
            this,
            &xToolsFileMergeAssistant::onImportPushButtonClicked);
    connect(ui->pushButtonMerge,
            &QPushButton::clicked,
            this,
            &xToolsFileMergeAssistant::onMergePushButtonClicked);
    connect(ui->pushButtonClear,
            &QPushButton::clicked,
            this,
            &xToolsFileMergeAssistant::onClearPushButtonClicked);
    connect(ui->pushButtonUp,
            &QPushButton::clicked,
            this,
            &xToolsFileMergeAssistant::onUpPushButtonClicked);
    connect(ui->pushButtonDown,
            &QPushButton::clicked,
            this,
            &xToolsFileMergeAssistant::onDownPushButtonClicked);
    connect(ui->pushButtonRemove,
            &QPushButton::clicked,
            this,
            &xToolsFileMergeAssistant::onRemovePushButtonClicked);
}

xToolsFileMergeAssistant::~xToolsFileMergeAssistant()
{
    delete ui;
}

void xToolsFileMergeAssistant::onImportPushButtonClicked()
{
    auto files = QFileDialog::getOpenFileNames(this,
                                               tr("Import Files"),
                                               m_desktopPath,
                                               tr("Bin file(*.bin);;All file(*)"));
    if (files.isEmpty()) {
        return;
    }

    ui->listWidget->clear();
    for (const auto &file : files) {
        ui->listWidget->addItem(file);
    }
}

void xToolsFileMergeAssistant::onMergePushButtonClicked()
{
    auto showOpenFileFailedMessageBox = [=](const QString &fileName, const QString &errorString) {
        QMessageBox::warning(this,
                             tr("Open File Failed"),
                             tr("Open file(%1) failed:").arg(fileName) + errorString);
    };

    if (ui->listWidget->count() <= 0) {
        QMessageBox::warning(this,
                             tr("Merge Files"),
                             tr("No file to merge, please import files first."));
        return;
    }

    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Save Files"),
                                                 m_desktopPath,
                                                 tr("Bin file(*.bin);;All file(*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile outFile(fileName);
    if (!outFile.open(QFile::WriteOnly | QFile::Truncate)) {
        showOpenFileFailedMessageBox(fileName, outFile.errorString());
        return;
    }

    setProgressBarRange();
    ui->progressBar->show();

    int mergedBytes = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QString fileName = ui->listWidget->item(i)->text();
        QFile inFile(fileName);
        if (!inFile.open(QFile::ReadOnly)) {
            showOpenFileFailedMessageBox(fileName, inFile.errorString());
            ui->progressBar->hide();
            return;
        }

        QByteArray data = inFile.readAll();
        inFile.close();

        QDataStream out(&outFile);
        out << data;

        mergedBytes += data.size();
        ui->progressBar->setValue(mergedBytes);
    }

    outFile.close();
    ui->progressBar->hide();
    QMessageBox::information(this, tr("Merge Files"), tr("Merge files successfully."));
}

void xToolsFileMergeAssistant::onClearPushButtonClicked()
{
    ui->listWidget->clear();
}

void xToolsFileMergeAssistant::onUpPushButtonClicked()
{
    auto items = ui->listWidget->selectedItems();
    for (auto &item : items) {
        int row = ui->listWidget->row(item);
        int newRow = row - 1;
        if (row >= 0) {
            ui->listWidget->takeItem(row);
            ui->listWidget->insertItem(newRow, item);
            item->setSelected(true);
        }
    }
}

void xToolsFileMergeAssistant::onDownPushButtonClicked()
{
    auto items = ui->listWidget->selectedItems();
    for (auto &item : items) {
        int row = ui->listWidget->row(item);
        int newRow = row + 1;
        if (row < ui->listWidget->count()) {
            ui->listWidget->takeItem(row);
            ui->listWidget->insertItem(newRow, item);
            item->setSelected(true);
        }
    }
}

void xToolsFileMergeAssistant::onRemovePushButtonClicked()
{
    auto items = ui->listWidget->selectedItems();
    for (auto item : items) {
        ui->listWidget->removeItemWidget(item);
        delete item;
    }
}

void xToolsFileMergeAssistant::setProgressBarRange()
{
    int allBytes = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QString fileName = ui->listWidget->item(i)->text();
        QFile file(fileName);
        file.open(QFile::ReadOnly);
        allBytes += file.size();
        file.close();
    }

    ui->progressBar->setRange(0, allBytes);
}
