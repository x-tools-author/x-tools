/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
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

#include "SAKDebugger.hh"
#include "SAKApplication.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKPluginAutoResponseItem.hh"
#include "SAKPluginAutoResponse.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#include "ui_SAKPluginAutoResponse.h"

SAKPluginAutoResponse::SAKPluginAutoResponse(QWidget *parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKPluginAutoResponse)
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
    connect(&mClearMessageInfoTimer, &QTimer::timeout, this, &SAKPluginAutoResponse::clearMessage);
#if 0
    // Read in record from database
    mDatabaseInterface = mDebugPage->databaseInterface();
    mTableName = mDebugPage->tableNameAutoResponseTable();
    readInRecord();
#endif
}

SAKPluginAutoResponse::~SAKPluginAutoResponse()
{
    delete mUi;
}

SAKPluginAutoResponseItem *innerCreateItem(SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem &var, SAKDebugger *debugPage, QListWidget *listWidget)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    SAKPluginAutoResponseItem *itemWidget = new SAKPluginAutoResponseItem(debugPage,
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

void SAKPluginAutoResponse::outputMessage(QString msg, bool isInfo)
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

void SAKPluginAutoResponse::clearMessage()
{
    mClearMessageInfoTimer.stop();
    mMsgLabel->clear();
}

void SAKPluginAutoResponse::readInRecord()
{
#if 0
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem> itemList = mDatabaseInterface->selectAutoResponseItem();
    for (auto &var : itemList){
        SAKOtherAutoResponseItem *item = innerCreateItem(var, mDebugPage, mListWidget);
        initializingItem(item);
    }
#endif
}

bool SAKPluginAutoResponse::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *w = mListWidget->itemWidget(item);
        SAKPluginAutoResponseItem *itemWidget = reinterpret_cast<SAKPluginAutoResponseItem*>(w);
        if (itemWidget->itemID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
}

void SAKPluginAutoResponse::initializingItem(SAKPluginAutoResponseItem *item)
{
    if (item){
        connect(item, &SAKPluginAutoResponseItem::descriptionChanged, this, &SAKPluginAutoResponse::changeDescription);
        connect(item, &SAKPluginAutoResponseItem::referenceTextChanged, this, &SAKPluginAutoResponse::changeReferenceText);
        connect(item, &SAKPluginAutoResponseItem::responseTextChanged, this, &SAKPluginAutoResponse::changeResponseText);
        connect(item, &SAKPluginAutoResponseItem::enableChanged, this, &SAKPluginAutoResponse::changeDelay);
        connect(item, &SAKPluginAutoResponseItem::optionChanged, this, &SAKPluginAutoResponse::changeOption);
        connect(item, &SAKPluginAutoResponseItem::referenceFormatChanged, this, &SAKPluginAutoResponse::changeReferenceFormat);
        connect(item, &SAKPluginAutoResponseItem::responseFromatChanged, this, &SAKPluginAutoResponse::changeResponseFromat);
        connect(item, &SAKPluginAutoResponseItem::delayChanged, this, &SAKPluginAutoResponse::changeDelay);
        connect(item, &SAKPluginAutoResponseItem::intervalChanged, this, &SAKPluginAutoResponse::changeInterval);
    }
}

void SAKPluginAutoResponse::changeDescription(const QString &description)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.description, QVariant::fromValue(description), id, true);
    }
}

void SAKPluginAutoResponse::changeReferenceText(const QString &text)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.referenceText, QVariant::fromValue(text), id, true);
    }
}

void SAKPluginAutoResponse::changeResponseText(const QString &text)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.responseText, QVariant::fromValue(text), id, true);
    }
}

void SAKPluginAutoResponse::changeEnable(bool enable)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.enable, QVariant::fromValue(enable), id, false);
    }
}

void SAKPluginAutoResponse::changeOption(int option)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.option, QVariant::fromValue(option), id, false);
    }
}

void SAKPluginAutoResponse::changeReferenceFormat(int format)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.referenceFormat, QVariant::fromValue(format), id, false);
    }
}

void SAKPluginAutoResponse::changeResponseFromat(int format)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.responseFormat, QVariant::fromValue(format), id, false);
    }
}

void SAKPluginAutoResponse::changeDelay(bool delay)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.delay, QVariant::fromValue(delay), id, false);
    }
}

void SAKPluginAutoResponse::changeInterval(int interval)
{
    SAKPluginAutoResponseItem *item = sender2item(sender());
    if (item){
        SAKDebugPageCommonDatabaseInterface::AutoResponseTable table;
        quint64 id = item->itemID();
        mDatabaseInterface->updateRecord(mTableName, table.columns.interval, QVariant::fromValue(interval), id, false);
    }
}

SAKPluginAutoResponseItem *SAKPluginAutoResponse::sender2item(QObject *sender)
{
    SAKPluginAutoResponseItem *item = Q_NULLPTR;
    if (sender){
        if (sender->inherits("SAKOtherAutoResponseItem")){
            item = qobject_cast<SAKPluginAutoResponseItem*>(sender);
        }
    }
    return item;
}

QList<SAKPluginAutoResponseItem *> SAKPluginAutoResponse::items()
{
    QList<SAKPluginAutoResponseItem *> itemList;
    for (int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        SAKPluginAutoResponseItem *itemWidget = qobject_cast<SAKPluginAutoResponseItem *>(mListWidget->itemWidget(item));
        itemList.append(itemWidget);
    }

    return itemList;
}

void SAKPluginAutoResponse::on_forbidAllCheckBox_clicked()
{
    for(int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *widget = mListWidget->itemWidget(item);
        bool disAble = mForbidAllCheckBox->isChecked();
        reinterpret_cast<SAKPluginAutoResponseItem*>(widget)->setAllAutoResponseDisable(disAble);
    }
}

void SAKPluginAutoResponse::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item){
        outputMessage(tr("Please select an item first!"), false);
        return;
    }

    // Delete record from database
    SAKPluginAutoResponseItem *w = reinterpret_cast<SAKPluginAutoResponseItem*>(mListWidget->itemWidget(item));
    quint64 id = w->itemID();
    mDatabaseInterface->deleteRecord(mTableName, id);

    mListWidget->removeItemWidget(item);
    delete item;
}

void SAKPluginAutoResponse::on_addItemPushButton_clicked()
{
    // Check length fo item
    if (mListWidget->count() >= SAK_MAX_AUTO_RESPONSE_COUNT){
        outputMessage(tr("Items are too many, operation will be ignored!"), false);
        return;
    }
#if 0
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
#endif
}

void SAKPluginAutoResponse::on_outportPushButton_clicked()
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

void SAKPluginAutoResponse::on_importPushButton_clicked()
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
#if 0
                /// @brief If the item is not exist, new a new item.
                if (!contains(responseItem.id)){
                    SAKOtherAutoResponseItem *item = innerCreateItem(responseItem, mDebugPage, mListWidget);
                    initializingItem(item);
                    mDatabaseInterface->insertAutoResponseItem(responseItem);
                }
#endif
            }
        }
    }else{
        outputMessage(file.errorString(), false);
    }
}
