/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKGlobal.hh"
#include "SAKTimingSendingItem.hh"
#include "SAKTimingSendingManager.hh"

#include "ui_SAKTimingSendingManager.h"

#include <QDebug>
#include <QDateTime>

SAKTimingSendingManager::SAKTimingSendingManager(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKTimingSendingManager)
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

SAKTimingSendingManager::~SAKTimingSendingManager()
{
    delete ui;
}

void SAKTimingSendingManager::on_savePushButton_clicked()
{

}

void SAKTimingSendingManager::on_importPushButton_clicked()
{

}

void SAKTimingSendingManager::on_deletePushButton_clicked()
{
    QListWidgetItem *currentItem = itemListWidget->currentItem();
    if (currentItem){
        itemListWidget->removeItemWidget(currentItem);
        delete currentItem;
    }
}

void SAKTimingSendingManager::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(itemListWidget);
    itemListWidget->addItem(item);

    SAKTimingSendingItem *itemWidget = new SAKTimingSendingItem(debugPage);
    item->setSizeHint(itemWidget->size());
    itemListWidget->setItemWidget(item, itemWidget);
}
