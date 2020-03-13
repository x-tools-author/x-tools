/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QListWidgetItem>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKInputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataItemManager.hh"

#include "ui_SAKInputDataItemManager.h"

SAKInputDataItemManager::SAKInputDataItemManager(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKInputDataItemManager)
    ,debugPage (debugPage)
    ,crcInterface (new SAKCRCInterface)
    ,factory (new SAKInputDataFactory)
    ,inputManager (inputManager)
{
    ui->setupUi(this);
    deletePushButton    = ui->deletePushButton;
    addPushButton       = ui->addPushButton;
    listWidget          = ui->itemListWidget;
    infoLabel           = ui->infoLabel;
}

SAKInputDataItemManager::~SAKInputDataItemManager()
{
    delete ui;
}

void SAKInputDataItemManager::on_deletePushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (item){
        listWidget->removeItemWidget(item);
        delete item;
    }
}

void SAKInputDataItemManager::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    SAKInputDataItem *itemWidget = new SAKInputDataItem(debugPage, inputManager, this);
    item->setSizeHint(itemWidget->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, itemWidget);
}
