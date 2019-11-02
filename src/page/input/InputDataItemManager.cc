/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */

#include <QListWidgetItem>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "InputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "InputDataFactory.hh"
#include "InputDataItemManager.hh"

#include "ui_InputDataItemManager.h"

InputDataItemManager::InputDataItemManager(SAKDebugPage *debugPage, DebugPageInputManager *inputManager, QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::InputDataItemManager)
    ,debugPage (debugPage)
    ,crcInterface (new SAKCRCInterface)
    ,factory (new InputDataFactory)
    ,inputManager (inputManager)
{
    ui->setupUi(this);
    deletePushButton    = ui->deletePushButton;
    addPushButton       = ui->addPushButton;
    listWidget          = ui->itemListWidget;
    infoLabel           = ui->infoLabel;
}

InputDataItemManager::~InputDataItemManager()
{
    delete ui;
}

void InputDataItemManager::on_deletePushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (item){
        listWidget->removeItemWidget(item);
        delete item;
    }
}

void InputDataItemManager::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    InputDataItem *itemWidget = new InputDataItem(debugPage, inputManager, this);
    item->setSizeHint(itemWidget->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, itemWidget);
}
