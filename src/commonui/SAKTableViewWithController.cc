/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>

#include "SAKTableViewWithController.hh"
#include "ui_SAKTableViewWithController.h"

SAKTableViewWithController::SAKTableViewWithController(const char *lg, QWidget *parent)
    : QWidget{parent}
    , mLoggingCategory{lg}
    , ui(new Ui::SAKTableViewWithController)
{
    ui->setupUi(this);

    connect(ui->pushButtonEdit, &QPushButton::clicked,
            this, &SAKTableViewWithController::onPushButtonEditClicked);
    connect(ui->pushButtonClear, &QPushButton::clicked,
            this, &SAKTableViewWithController::onPushButtonClearClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked,
            this, &SAKTableViewWithController::onPushButtonDeleteClicked);
    connect(ui->pushButtonImport, &QPushButton::clicked,
            this, &SAKTableViewWithController::onPushButtonImportClicked);
    connect(ui->pushButtonExport, &QPushButton::clicked,
            this, &SAKTableViewWithController::onPushButtonExportClicked);
    connect(ui->pushButtonAppend, &QPushButton::clicked,
            this, &SAKTableViewWithController::onPushButtonAppendClicked);
}

SAKTableViewWithController::~SAKTableViewWithController()
{
    delete ui;
}

void SAKTableViewWithController::setupTableModel(QAbstractTableModel *tableModel)
{
    if (!tableModel) {
        qCWarning(mLoggingCategory) << "The value of tableModel is nullptr!";
        return;
    }

    mTableModel = tableModel;
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

    headerViewModel->setColumnCount(headers.count());
    headerViewModel->setHorizontalHeaderLabels(headers);
    //headerView->setSectionResizeMode(11, QHeaderView::Stretch);
    //headerView->setSectionResizeMode(QHeaderView::Stretch);
    headerView->setModel(headerViewModel);
    headerView->setDefaultAlignment(Qt::AlignLeft);

    mMenu = new QMenu(ui->pushButtonVisible);
    ui->pushButtonVisible->setMenu(mMenu);
    for (int i = 0; i < headers.count(); i++) {
        QAction *ret = mMenu->addAction(headers.at(i));
        connect(ret, &QAction::triggered, this, [=](){
            if (ret->isChecked()) {
                tableView->showColumn(i);
            } else {
                tableView->hideColumn(i);
            }
        });
        ret->setCheckable(true);
        ret->setChecked(true);
    }
}

QModelIndex SAKTableViewWithController::currentIndex()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this,
                             tr("Please Select an Item"),
                             tr("Please select an tiem first, then try again!"));
    }
    return index;
}

void SAKTableViewWithController::onPushButtonEditClicked()
{
    QModelIndex index = currentIndex();
    if (index.isValid()) {
        edit(index);
    }
}

void SAKTableViewWithController::onPushButtonClearClicked()
{
    int ret = QMessageBox::warning(this,
                                   tr("Clear Data"),
                                   tr("The data will be empty from settings file, "
                                      "please confrim the operation!"),
                                   QMessageBox::Cancel|QMessageBox::Ok);
    if (ret == QMessageBox::Ok) {
        clear();
    }
}

void SAKTableViewWithController::onPushButtonDeleteClicked()
{
    QModelIndex index = currentIndex();
    if (index.isValid()) {
        remove(index);
    }
}

void SAKTableViewWithController::onPushButtonImportClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*)"));
    if (!fileName.isEmpty()) {
        importFromFile(fileName);
    }
}

void SAKTableViewWithController::onPushButtonExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Import data"),
                                                    ".",
                                                    tr("JSON (*.json);;All (*); "));
    if (!fileName.isEmpty()) {
        exportToFile(fileName);
    }
}

void SAKTableViewWithController::onPushButtonAppendClicked()
{
    append();
}
