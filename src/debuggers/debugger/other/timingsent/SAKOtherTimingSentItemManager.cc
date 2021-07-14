/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>

#include "SAKDebugPage.hh"
#include "SAKApplication.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKOtherTimingSentItem.hh"
#include "SAKOtherTimingSentItemManager.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#include "ui_SAKOtherTimingSentItemManager.h"

SAKOtherTimingSentItemManager::SAKOtherTimingSentItemManager(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,mUi(new Ui::SAKOtherTimingSentItemManager)
{
    mUi->setupUi(this);

    mItemListWidget = mUi->itemListWidget;
    mOutportPushButton = mUi->outportPushButton;
    mImportPushButton = mUi->importPushButton;
    mDeletePushButton = mUi->deletePushButton;
    mAddPushButton = mUi->addPushButton;
    mMessageLabel = mUi->messageLabel;

    mClearMessageTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearMessageTimer, &QTimer::timeout, this, [&](){
        mClearMessageTimer.stop();
        mMessageLabel->clear();
    });

    mDatabaseInterface = mDebugPage->databaseInterface();
    mTableName = mDebugPage->tableNameTimingSendingTable();
    readinRecord();
}

SAKOtherTimingSentItemManager::~SAKOtherTimingSentItemManager()
{
    delete mUi;
}

SAKOtherTimingSentItem *innerCreateItem(SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem &var, SAKDebugPage *debugPage, QListWidget *listWidget)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    SAKOtherTimingSentItem *itemWidget = new SAKOtherTimingSentItem(debugPage,
                                                                    var.id,
                                                                    var.interval,
                                                                    var.format,
                                                                    var.comment,
                                                                    var.data,
                                                                    Q_NULLPTR);
    item->setSizeHint(itemWidget->size());
    listWidget->setItemWidget(item, itemWidget);
    return itemWidget;
}

void SAKOtherTimingSentItemManager::readinRecord()
{
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem> itemList = mDatabaseInterface->selectTimingSentItem();
    if (itemList.isEmpty()){
        return;
    }

    for (auto &var : itemList){
        SAKOtherTimingSentItem *item = innerCreateItem(var, mDebugPage, mItemListWidget);
        initializingItem(item);
    }
}

bool SAKOtherTimingSentItemManager::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < mItemListWidget->count(); i++){
        QListWidgetItem *item = mItemListWidget->item(i);
        QWidget *w = mItemListWidget->itemWidget(item);
        SAKOtherTimingSentItem *itemWidget = reinterpret_cast<SAKOtherTimingSentItem*>(w);
        if (itemWidget->itemID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
}

void SAKOtherTimingSentItemManager::outputMessage(QString msg, bool isError)
{
    QString color = "black";
    if (isError){
        color = "red";
        QApplication::beep();
    }
    mMessageLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));
    mMessageLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    mClearMessageTimer.start();
}

void SAKOtherTimingSentItemManager::initializingItem(SAKOtherTimingSentItem *item)
{
    if (item){
        connect(item, &SAKOtherTimingSentItem::intervalChanged, this, &SAKOtherTimingSentItemManager::changeInterval);
        connect(item, &SAKOtherTimingSentItem::formatChanged, this, &SAKOtherTimingSentItemManager::changeFormat);
        connect(item, &SAKOtherTimingSentItem::descriptionChanged, this, &SAKOtherTimingSentItemManager::changeDescription);
        connect(item, &SAKOtherTimingSentItem::inputTextChanged, this, &SAKOtherTimingSentItemManager::changeInputText);
    }
}

SAKOtherTimingSentItem *SAKOtherTimingSentItemManager::sender2item(QObject *sender)
{
    SAKOtherTimingSentItem *item = Q_NULLPTR;
    if (sender){
        if (sender->inherits("SAKOtherTimingSentItem")){
            item = qobject_cast<SAKOtherTimingSentItem*>(sender);
        }
    }

    return item;
}

void SAKOtherTimingSentItemManager::changeInterval(int interval)
{
    SAKOtherTimingSentItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::TimingSendingTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.interval, QVariant::fromValue(interval), id, false);
    }
}

void SAKOtherTimingSentItemManager::changeFormat(int format)
{
    SAKOtherTimingSentItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::TimingSendingTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.format, QVariant::fromValue(format), id, false);
    }
}

void SAKOtherTimingSentItemManager::changeDescription(QString description)
{
    SAKOtherTimingSentItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::TimingSendingTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.description, QVariant::fromValue(description), id, true);
    }
}

void SAKOtherTimingSentItemManager::changeInputText(QString text)
{
    SAKOtherTimingSentItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::TimingSendingTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.text, QVariant::fromValue(text), id, true);
    }
}

void SAKOtherTimingSentItemManager::on_outportPushButton_clicked()
{
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem> itemList = mDatabaseInterface->selectTimingSentItem();
    if (itemList.isEmpty()){
        return;
    }

    QJsonArray jsonArray;
    TimingSendingItemKey itemKey;
    for (auto &var : itemList){
        QJsonObject obj;
        obj.insert(itemKey.id, QVariant::fromValue(var.id).toJsonValue());
        obj.insert(itemKey.text, QVariant::fromValue(var.data).toJsonValue());
        obj.insert(itemKey.format, QVariant::fromValue(var.format).toJsonValue());
        obj.insert(itemKey.description, QVariant::fromValue(var.comment).toJsonValue());
        obj.insert(itemKey.interval, QVariant::fromValue(var.interval).toJsonValue());
        jsonArray.append(QJsonValue(obj));
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);

    // Open file
    QString defaultName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    defaultName.append(QString("/"));
    defaultName.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    defaultName.append(".json");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Outport data"), defaultName, QString("json (*.json)"));
    if (fileName.isEmpty()){
        return;
    }

    // Write data to file
    QFile file(fileName);
    if (file.open(QFile::ReadWrite)){
        file.write(jsonDoc.toJson());
        file.close();
    }
}

void SAKOtherTimingSentItemManager::on_importPushButton_clicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import data"), defaultPath, QString("json (*.json)"));
    QFile file(fileName);
    if (file.open(QFile::ReadWrite)){
        QByteArray array = file.readAll();
        file.close();

        QJsonDocument jsc = QJsonDocument::fromJson(array);
        if (!jsc.isArray()){
            outputMessage(tr("Format error!"), true);
            return;
        }

        QJsonArray jsa = jsc.array();
        for (int i = 0; i < jsa.count(); i++){
            if (jsa.at(i).isObject()){
                QJsonObject jso = jsa.at(i).toObject();
                TimingSendingItemKey itemKey;
                SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem responseItem;
                responseItem.id = jso.value(itemKey.id).toVariant().toULongLong();
                responseItem.data = jso.value(itemKey.text).toVariant().toString();
                responseItem.format = jso.value(itemKey.format).toVariant().toUInt();
                responseItem.comment = jso.value(itemKey.description).toVariant().toString();
                responseItem.interval = jso.value(itemKey.interval).toVariant().toUInt();

                // If item is not exist, creating an item
                if (!contains(responseItem.id)){
                    SAKOtherTimingSentItem *item = innerCreateItem(responseItem, mDebugPage, mItemListWidget);
                    initializingItem(item);
                    mDatabaseInterface->insertTimingSentItem(responseItem);
                }
            }
        }
    }else{
        outputMessage(file.errorString(), true);
    }
}

void SAKOtherTimingSentItemManager::on_deletePushButton_clicked()
{
    QListWidgetItem *currentItem = mItemListWidget->currentItem();
    if (currentItem){
        SAKOtherTimingSentItem *w = reinterpret_cast<SAKOtherTimingSentItem*>(mItemListWidget->itemWidget(currentItem));
        SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem sendingItem;
        sendingItem.id = w->itemID();
        mDatabaseInterface->deleteRecord(mTableName, sendingItem.id);

        mItemListWidget->removeItemWidget(currentItem);
        delete currentItem;
    }
}

void SAKOtherTimingSentItemManager::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(mItemListWidget);
    mItemListWidget->addItem(item);

    SAKOtherTimingSentItem *itemWidget = new SAKOtherTimingSentItem(mDebugPage);
    item->setSizeHint(itemWidget->size());
    mItemListWidget->setItemWidget(item, itemWidget);
    initializingItem(itemWidget);

    // Insert record to database
    SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem sendingItem;
    sendingItem.id = itemWidget->itemID();
    sendingItem.data = itemWidget->itemText();
    sendingItem.format = itemWidget->itemFormat();
    sendingItem.comment = itemWidget->itemDescription();
    sendingItem.interval = itemWidget->itemInterval();
    mDatabaseInterface->insertTimingSentItem(sendingItem);
}
