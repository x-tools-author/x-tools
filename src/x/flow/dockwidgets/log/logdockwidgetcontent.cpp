/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logdockwidgetcontent.h"
#include "ui_logdockwidgetcontent.h"

#include <QSettings>

#include "application.h"
#include "logdockwidgetcontentmodel.h"
#include "logdockwidgetcontentmodelfilter.h"

LogDockWidgetContent::LogDockWidgetContent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LogDockWidgetContent)
{
    ui->setupUi(this);

    LogDockWidgetContentModelFilter *filterModel = new LogDockWidgetContentModelFilter(this);
    filterModel->setSourceModel(&LogDockWidgetContentModel::singleton());
    ui->tableView->setModel(filterModel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    ui->comboBoxLogLevel->addItem(tr("Debug"), QtDebugMsg);
    ui->comboBoxLogLevel->addItem(tr("Information"), QtInfoMsg);
    ui->comboBoxLogLevel->addItem(tr("Warning"), QtWarningMsg);
    ui->comboBoxLogLevel->addItem(tr("Critical"), QtCriticalMsg);
    ui->comboBoxLogLevel->addItem(tr("Fatal"), QtFatalMsg);
    connect(ui->comboBoxLogLevel, &QComboBox::currentIndexChanged, [=]() {
        QtMsgType level = static_cast<QtMsgType>(ui->comboBoxLogLevel->currentData().toInt());
        filterModel->setMessagesLevel(level);

        QSettings *settings = xApp->settings();
        settings->setValue("Log/level", level);
    });
    connect(ui->pushButtonClear, &QPushButton::clicked, this, []() {
        LogDockWidgetContentModel::singleton().clear();
    });

    QSettings *settings = xApp->settings();
    int level = settings->value("Log/level", QtDebugMsg).toInt();
    int index = ui->comboBoxLogLevel->findData(level);
    ui->comboBoxLogLevel->setCurrentIndex(index < 0 ? 0 : index);
}

LogDockWidgetContent::~LogDockWidgetContent()
{
    delete ui;
}
