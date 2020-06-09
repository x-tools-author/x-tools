/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QListWidgetItem>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKInputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataItemManager.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKInputDataItemManager.h"

SAKInputDataItemManager::SAKInputDataItemManager(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,crcInterface(new SAKCRCInterface)
    ,factory(new SAKInputDataFactory)
    ,inputManager(inputManager)
    ,ui(new Ui::SAKInputDataItemManager)
{
    ui->setupUi(this);
    deletePushButton    = ui->deletePushButton;
    addPushButton       = ui->addPushButton;
    listWidget          = ui->itemListWidget;
    infoLabel           = ui->infoLabel;

    tableName = SAKDataStruct::presettingDataTableName(debugPage->pageType());
    databaseInterface = SAKDebugPageDatabaseInterface::instance();
    readinRecord();
}

SAKInputDataItemManager::~SAKInputDataItemManager()
{
    delete crcInterface;
    delete ui;
}

void innerCreateItem(SAKDataStruct::SAKStructPresettingDataItem &var, SAKDebugPage *debugPage, QListWidget *listWidget)
{

}

void SAKInputDataItemManager::readinRecord()
{
    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList = databaseInterface->selectPresettingDataItem(tableName);
    for (auto var : itemList){
        innerCreateItem(var, debugPage, listWidget);
    }
}

void SAKInputDataItemManager::on_deletePushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (item){
        SAKInputDataItem *itemWidget = reinterpret_cast<SAKInputDataItem*>(listWidget->itemWidget(item));
        SAKDataStruct::SAKStructPresettingDataItem dataItem;
        dataItem.id = itemWidget->parameterID();
        databaseInterface->deletePresettingDataItem(tableName, dataItem);

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

    /// @brief 添加记录至数据库
    SAKDataStruct::SAKStructPresettingDataItem dataItem;
    dataItem.id = itemWidget->parameterID();
    dataItem.data = itemWidget->parameterData();
    dataItem.format = itemWidget->parameterFormat();
    dataItem.comment = itemWidget->parameterComment();
    dataItem.classify = itemWidget->parameterClassify();
    databaseInterface->insertPresettingDataItem(tableName, dataItem);
}
