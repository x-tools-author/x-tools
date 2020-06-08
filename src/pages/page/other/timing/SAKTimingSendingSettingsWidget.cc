/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDateTime>

#include "SAKGlobal.hh"
#include "SAKTimingSendingItemWidget.hh"
#include "SAKTimingSendingSettingsWidget.hh"

#include "ui_SAKTimingSendingSettingsWidget.h"

SAKTimingSendingSettingsWidget::SAKTimingSendingSettingsWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKTimingSendingSettingsWidget)
    ,debugPage (debugPage)
{
    ui->setupUi(this);

    itemListWidget      = ui->itemListWidget;
    savePushButton      = ui->savePushButton;
    importPushButton    = ui->importPushButton;
    deletePushButton    = ui->deletePushButton;
    addPushButton       = ui->addPushButton;

    on_addPushButton_clicked();
}

SAKTimingSendingSettingsWidget::~SAKTimingSendingSettingsWidget()
{
    delete ui;
}

void SAKTimingSendingSettingsWidget::on_savePushButton_clicked()
{

}

void SAKTimingSendingSettingsWidget::on_importPushButton_clicked()
{

}

void SAKTimingSendingSettingsWidget::on_deletePushButton_clicked()
{
    QListWidgetItem *currentItem = itemListWidget->currentItem();
    if (currentItem){
        itemListWidget->removeItemWidget(currentItem);
        delete currentItem;
    }
}

void SAKTimingSendingSettingsWidget::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(itemListWidget);
    itemListWidget->addItem(item);

    SAKTimingSendingItemWidget *itemWidget = new SAKTimingSendingItemWidget(debugPage);
    item->setSizeHint(itemWidget->size());
    itemListWidget->setItemWidget(item, itemWidget);
}
