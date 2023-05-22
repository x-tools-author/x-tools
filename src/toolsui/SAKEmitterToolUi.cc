/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTableView>
#include <QHeaderView>
#include <QJsonDocument>
#include <QStandardItemModel>

#include "SAKEmitterTool.hh"
#include "SAKEmitterToolUi.hh"
#include "ui_SAKEmitterToolUi.h"

SAKEmitterToolUi::SAKEmitterToolUi(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::SAKEmitterToolUi)
{
    ui->setupUi(this);
}

SAKEmitterToolUi::~SAKEmitterToolUi()
{
    delete ui;
}

void SAKEmitterToolUi::setupEmitterTool(SAKEmitterTool *tool)
{
    if (tool == nullptr) {
        return;
    }

    if (!tool->inherits("SAKEmitterTool")) {
        return;
    }

    mTool = qobject_cast<SAKEmitterTool*>(tool);

    SAKEmitterTableModel *dataModel = mTool->getModel();
    QTableView *tableView = ui->tableView;
    QHeaderView *headerView = tableView->horizontalHeader();
    QStringList headers = mTool->getHeaders();
    QStandardItemModel *headerViewModel = new QStandardItemModel(headerView);

    tableView->setHorizontalHeader(headerView);
    tableView->setModel(dataModel);

    headerViewModel->setColumnCount(headers.count());
    headerViewModel->setHorizontalHeaderLabels(headers);
    headerView->setSectionResizeMode(11, QHeaderView::Stretch);
    //headerView->setSectionResizeMode(QHeaderView::Stretch);
    headerView->setModel(headerViewModel);
    headerView->setDefaultAlignment(Qt::AlignLeft);
#if 1
    //tableView->hideColumn(0);
    //tableView->hideColumn(1);
    tableView->hideColumn(2);
    tableView->hideColumn(3);
    //tableView->hideColumn(4);
    tableView->hideColumn(5);
    tableView->hideColumn(6);
    tableView->hideColumn(7);
    tableView->hideColumn(8);
    tableView->hideColumn(9);
    tableView->hideColumn(10);
#endif

    for (int i = 0; i < 100; i++) {
        QVariant var = mTool->itemContext(-1);
        QJsonDocument jsonDoc;
        jsonDoc.setObject(var.toJsonObject());
        mTool->addItem(QString::fromUtf8(jsonDoc.toJson()));
    }


    qDebug() << headers.count() << headers;
}
