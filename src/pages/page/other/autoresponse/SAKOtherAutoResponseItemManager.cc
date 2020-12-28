/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QJsonArray>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QJsonObject>
#include <QApplication>
#include <QJsonDocument>
#include <QStandardPaths>

#include "SAKDebugPage.hh"
#include "SAKApplication.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKOtherAutoResponseItem.hh"
#include "SAKOtherAutoResponseItemManager.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#include "ui_SAKOtherAutoResponseItemManager.h"

SAKOtherAutoResponseItemManager::SAKOtherAutoResponseItemManager(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,mDebugPage (debugPage)
    ,mUi (new Ui::SAKOtherAutoResponseItemManager)
{
    mUi->setupUi(this);
    mListWidget = mUi->listWidget;
    mForbidAllCheckBox = mUi->forbidAllCheckBox;
    mDeleteItemPushButton = mUi->deleteItemPushButton;
    mAddItemPushButton = mUi->addItemPushButton;
    mOutportPushButton = mUi->outportPushButton;
    mImportPushButton = mUi->importPushButton;
    mMsgLabel = mUi->msgLabel;

    mClearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearMessageInfoTimer, &QTimer::timeout, this, &SAKOtherAutoResponseItemManager::clearMessage);

    // Read in record from database
    mDatabaseInterface = mDebugPage->databaseInterface();
    mTableName = mDebugPage->tableNameAutoResponseTable();
    readInRecord();
}

SAKOtherAutoResponseItemManager::~SAKOtherAutoResponseItemManager()
{
    delete mUi;
}

SAKOtherAutoResponseItem *innerCreateItem(SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem &var, SAKDebugPage *debugPage, QListWidget *listWidget)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    SAKOtherAutoResponseItem *itemWidget = new SAKOtherAutoResponseItem(debugPage,
                                                                        var.id,
                                                                        var.name,
                                                                        var.referenceData,
                                                                        var.responseData,
                                                                        var.enable,
                                                                        var.referenceFormat,
                                                                        var.responseFormat,
                                                                        var.option,
                                                                        var.delay,
                                                                        var.interval,
                                                                        listWidget);
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);
    return itemWidget;
}

void SAKOtherAutoResponseItemManager::outputMessage(QString msg, bool isInfo)
{
    QString color = "black";
    if (!isInfo){
        color = "red";
        QApplication::beep();
    }
    mMsgLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    mMsgLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    mClearMessageInfoTimer.start();
}

void SAKOtherAutoResponseItemManager::clearMessage()
{
    mClearMessageInfoTimer.stop();
    mMsgLabel->clear();
}

void SAKOtherAutoResponseItemManager::readInRecord()
{
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem> itemList = mDatabaseInterface->selectAutoResponseItem();
    for (auto &var : itemList){
        SAKOtherAutoResponseItem *item = innerCreateItem(var, mDebugPage, mListWidget);
        initializingItem(item);
    }
}

bool SAKOtherAutoResponseItemManager::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *w = mListWidget->itemWidget(item);
        SAKOtherAutoResponseItem *itemWidget = reinterpret_cast<SAKOtherAutoResponseItem*>(w);
        if (itemWidget->itemID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
}

void SAKOtherAutoResponseItemManager::initializingItem(SAKOtherAutoResponseItem *item)
{
    if (item){
        connect(item, &SAKOtherAutoResponseItem::descriptionChanged, this, &SAKOtherAutoResponseItemManager::changeDescription);
        connect(item, &SAKOtherAutoResponseItem::referenceTextChanged, this, &SAKOtherAutoResponseItemManager::changeReferenceText);
        connect(item, &SAKOtherAutoResponseItem::responseTextChanged, this, &SAKOtherAutoResponseItemManager::changeResponseText);
        connect(item, &SAKOtherAutoResponseItem::enableChanged, this, &SAKOtherAutoResponseItemManager::changeDelay);
        connect(item, &SAKOtherAutoResponseItem::optionChanged, this, &SAKOtherAutoResponseItemManager::changeOption);
        connect(item, &SAKOtherAutoResponseItem::referenceFormatChanged, this, &SAKOtherAutoResponseItemManager::changeReferenceFormat);
        connect(item, &SAKOtherAutoResponseItem::responseFromatChanged, this, &SAKOtherAutoResponseItemManager::changeResponseFromat);
        connect(item, &SAKOtherAutoResponseItem::delayChanged, this, &SAKOtherAutoResponseItemManager::changeDelay);
        connect(item, &SAKOtherAutoResponseItem::intervalChanged, this, &SAKOtherAutoResponseItemManager::changeInterval);
    }
}

void SAKOtherAutoResponseItemManager::changeDescription(const QString &description)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.description, QVariant::fromValue(description), id, true);
    }
}

void SAKOtherAutoResponseItemManager::changeReferenceText(const QString &text)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.referenceText, QVariant::fromValue(text), id, true);
    }
}

void SAKOtherAutoResponseItemManager::changeResponseText(const QString &text)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.responseText, QVariant::fromValue(text), id, true);
    }
}

void SAKOtherAutoResponseItemManager::changeEnable(bool enable)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.enable, QVariant::fromValue(enable), id, false);
    }
}

void SAKOtherAutoResponseItemManager::changeOption(int option)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.option, QVariant::fromValue(option), id, false);
    }
}

void SAKOtherAutoResponseItemManager::changeReferenceFormat(int format)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.referenceFormat, QVariant::fromValue(format), id, false);
    }
}

void SAKOtherAutoResponseItemManager::changeResponseFromat(int format)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.responseFormat, QVariant::fromValue(format), id, false);
    }
}

void SAKOtherAutoResponseItemManager::changeDelay(bool delay)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.delay, QVariant::fromValue(delay), id, false);
    }
}

void SAKOtherAutoResponseItemManager::changeInterval(int interval)
{
    SAKOtherAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.interval, QVariant::fromValue(interval), id, false);
    }
}

SAKOtherAutoResponseItem *SAKOtherAutoResponseItemManager::sender2item(QObject *sender)
{
    SAKOtherAutoResponseItem *item = Q_NULLPTR;
    if (sender){
        if (sender->inherits("SAKOtherAutoResponseItem")){
            item = qobject_cast<SAKOtherAutoResponseItem*>(sender);
        }
    }
    return item;
}

QList<SAKOtherAutoResponseItem *> SAKOtherAutoResponseItemManager::items()
{
    QList<SAKOtherAutoResponseItem *> itemList;
    for (int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        SAKOtherAutoResponseItem *itemWidget = qobject_cast<SAKOtherAutoResponseItem *>(mListWidget->itemWidget(item));
        itemList.append(itemWidget);
    }

    return itemList;
}

void SAKOtherAutoResponseItemManager::on_forbidAllCheckBox_clicked()
{
    for(int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *widget = mListWidget->itemWidget(item);
        bool disAble = mForbidAllCheckBox->isChecked();
        reinterpret_cast<SAKOtherAutoResponseItem*>(widget)->setAllAutoResponseDisable(disAble);
    }
}

void SAKOtherAutoResponseItemManager::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item){
        outputMessage(tr("Please select an item first!"), false);
        return;
    }

    // Delete record from database
    SAKOtherAutoResponseItem *w = reinterpret_cast<SAKOtherAutoResponseItem*>(mListWidget->itemWidget(item));
    quint64 id = w->itemID();
    mDatabaseInterface->deleteRecord(mTableName, id);

    mListWidget->removeItemWidget(item);
    delete item;
}

void SAKOtherAutoResponseItemManager::on_addItemPushButton_clicked()
{
    // Check length fo item
    if (mListWidget->count() >= SAK_MAX_AUTO_RESPONSE_COUNT){
        outputMessage(tr("Items are too many, operation will be ignored!"), false);
        return;
    }

    QListWidgetItem *item = new QListWidgetItem(mListWidget);
    mListWidget->addItem(item);
    SAKOtherAutoResponseItem *itemWidget = new SAKOtherAutoResponseItem(mDebugPage, mListWidget);
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    mListWidget->setItemWidget(item, itemWidget);
    initializingItem(itemWidget);

    // Add record
    SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem dataItem;
    dataItem.id = itemWidget->itemID();
    dataItem.name = itemWidget->itemDescription();
    dataItem.enable = itemWidget->itemEnable();
    dataItem.responseData = itemWidget->itemResponseText();
    dataItem.referenceData = itemWidget->itemRefernceText();
    dataItem.responseFormat = itemWidget->itemResponseFormat();
    dataItem.referenceFormat = itemWidget->itemReferenceFormat();
    dataItem.option = itemWidget->itemOption();
    dataItem.delay = itemWidget->delay();
    dataItem.interval = itemWidget->interval();
    mDatabaseInterface->insertAutoResponseItem(dataItem);
}

void SAKOtherAutoResponseItemManager::on_outportPushButton_clicked()
{
    /// @brief Read record.
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem> itemList;
    if (itemList.isEmpty()){
        return;
    }

    QJsonArray jsonArray;
    AutoResponseItemKey itemKey;
    for (auto &var : itemList){
        QJsonObject obj;
        obj.insert(itemKey.id, QVariant::fromValue(var.id).toJsonValue());
        obj.insert(itemKey.description, QVariant::fromValue(var.name).toJsonValue());
        obj.insert(itemKey.enable, QVariant::fromValue(var.enable).toJsonValue());
        obj.insert(itemKey.option, QVariant::fromValue(var.option).toJsonValue());
        obj.insert(itemKey.responseText, QVariant::fromValue(var.responseData).toJsonValue());
        obj.insert(itemKey.referenceText, QVariant::fromValue(var.referenceData).toJsonValue());
        obj.insert(itemKey.responseFormat, QVariant::fromValue(var.responseFormat).toJsonValue());
        obj.insert(itemKey.referenceFormat, QVariant::fromValue(var.referenceFormat).toJsonValue());
        jsonArray.append(QJsonValue(obj));
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);

    /// @brief 打开文件，导出的数据将保存至该文件
    QString defaultName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    defaultName.append(QString("/"));
    defaultName.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    defaultName.append(".json");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Outport data"), defaultName, QString("json (*.json)"));
    if (fileName.isEmpty()){
        return;
    }

    /// @brief 保存至文件
    QFile file(fileName);
    if (file.open(QFile::ReadWrite)){
        file.write(jsonDoc.toJson());
        file.close();
    }else{
        outputMessage(file.errorString(), false);
    }
}

void SAKOtherAutoResponseItemManager::on_importPushButton_clicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import file"), defaultPath, QString("json (*.json)"));
    QFile file(fileName);
    if (file.open(QFile::ReadWrite)){
        QByteArray array = file.readAll();
        file.close();

        QJsonDocument jsc = QJsonDocument::fromJson(array);
        if (!jsc.isArray()){
            outputMessage(QString("QJsonDocument is not json array"), false);
            return;
        }

        QJsonArray jsa = jsc.array();
        for (int i = 0; i < jsa.count(); i++){
            if (jsa.at(i).isObject()){
                QJsonObject jso = jsa.at(i).toObject();
                AutoResponseItemKey itemKey;
                SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem responseItem;
                responseItem.id = jso.value(itemKey.id).toVariant().toULongLong();
                responseItem.name = jso.value(itemKey.description).toVariant().toString();
                responseItem.enable = jso.value(itemKey.enable).toVariant().toBool();
                responseItem.option = jso.value(itemKey.option).toVariant().toUInt();
                responseItem.responseData = jso.value(itemKey.responseText).toVariant().toString();
                responseItem.referenceData = jso.value(itemKey.referenceText).toVariant().toString();
                responseItem.responseFormat = jso.value(itemKey.responseFormat).toVariant().toUInt();
                responseItem.referenceFormat = jso.value(itemKey.referenceFormat).toVariant().toUInt();

                /// @brief If the item is not exist, new a new item.
                if (!contains(responseItem.id)){
                    SAKOtherAutoResponseItem *item = innerCreateItem(responseItem, mDebugPage, mListWidget);
                    initializingItem(item);
                    mDatabaseInterface->insertAutoResponseItem(responseItem);
                }
            }
        }
    }else{
        outputMessage(file.errorString(), false);
    }
}
