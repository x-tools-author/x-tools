/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardItemModel>

#include "SAKLogUi.hh"
#include "SAKSettings.hh"

#include "ui_SAKLogUi.h"

SAKLogUi::SAKLogUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAKLogUi)
{
    ui->setupUi(this);
    ui->comboBoxLevel->addItem(tr("Disable"), -1);
    ui->comboBoxLevel->addItem(tr("Debug"), QtMsgType::QtDebugMsg);
    ui->comboBoxLevel->addItem(tr("Info"), QtMsgType::QtInfoMsg);
    ui->comboBoxLevel->addItem(tr("Warning"), QtMsgType::QtWarningMsg);
    ui->comboBoxLevel->addItem(tr("Error"), QtMsgType::QtSystemMsg);

    ui->comboBoxLevel->setGroupKey("Log", "level");
    ui->comboBoxLifeCycle->setGroupKey("Log", "LifeCycle");

    connect(ui->comboBoxLevel, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onComboBoxLevelCurrentIndexChanged()));
    connect(ui->comboBoxLifeCycle, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onComboBoxLifeCycleCurrentIndexChanged()));

    QAbstractTableModel *tableModel = SAKLog::instance()->tableModel();
    QTableView *tableView = ui->tableView;
    QHeaderView *headerView = tableView->horizontalHeader();

    int columnCount = tableModel->columnCount();
    QStringList headers;
    for (int i = 0; i < columnCount; i++) {
        auto orientation = Qt::Orientation::Horizontal;
        QString str = tableModel->headerData(i, orientation).toString();
        headers.append(str);
    }
    QStandardItemModel *headerViewModel = new QStandardItemModel(headerView);

    tableView->setHorizontalHeader(headerView);
    tableView->setModel(tableModel);
    tableView->setAlternatingRowColors(true);

    headerViewModel->setColumnCount(headers.count());
    headerViewModel->setHorizontalHeaderLabels(headers);
    headerView->setModel(headerViewModel);
    headerView->setDefaultAlignment(Qt::AlignLeft);
    headerView->setSectionResizeMode(2, QHeaderView::Stretch);

    connect(ui->pushButtonClear, &QPushButton::clicked,
            this, &SAKLogUi::onPushButtonClearClicked);
    connect(ui->pushButtonDirectory, &QPushButton::clicked,
            this, &SAKLogUi::onPushButtonDirectoryClicked);
    connect(ui->checkBoxPause, &QCheckBox::clicked,
            this, &SAKLogUi::onCheckBoxPauseClicked);
}

SAKLogUi::~SAKLogUi()
{
    delete ui;
}

void SAKLogUi::onPushButtonClearClicked()
{
    QMessageBox::information(this, tr("Clear Log Outputted"),
                             tr("The log outputted will be empty,"
                                " but the log file will not!"));
    SAKLog::instance()->clear();
}

void SAKLogUi::onPushButtonDirectoryClicked()
{
    QDesktopServices::openUrl(QUrl(SAKLog::instance()->logPath()));
}

void SAKLogUi::onComboBoxLevelCurrentIndexChanged()
{
    int level = ui->comboBoxLevel->currentData().toInt();
    SAKLog::instance()->setLogLevel(level);
}

void SAKLogUi::onComboBoxLifeCycleCurrentIndexChanged()
{
    int lefeCycle = ui->comboBoxLifeCycle->currentText().toInt();
    SAKLog::instance()->setLogLifeCycle(lefeCycle);
}

void SAKLogUi::onCheckBoxPauseClicked()
{
    bool paused = ui->checkBoxPause->isChecked();
    SAKLog::instance()->setIsPaused(paused);
}
