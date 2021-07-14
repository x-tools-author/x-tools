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
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKDebugPage.hh"
#include "SAKApplication.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKInputDataPresetItemManager.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#include "ui_SAKInputDataPresetItemManager.h"

SAKInputDataPresetItemManager::SAKInputDataPresetItemManager(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,mUi(new Ui::SAKInputDataPresetItemManager)
{
    mUi->setupUi(this);
    mDeletePushButton = mUi->deletePushButton;
    mAddPushButton = mUi->addPushButton;
    mListWidget = mUi->itemListWidget;
    mInfoLabel = mUi->infoLabel;
    mOutportPushButton = mUi->outportPushButton;
    mImportPushButton = mUi->importPushButton;

    mClearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearMessageInfoTimer, &QTimer::timeout, this, [&](){
        mClearMessageInfoTimer.stop();
        mInfoLabel->clear();
    });

    mDatabaseInterface = mDebugPage->databaseInterface();
    mTableName = mDebugPage->tableNamePresettingDataTable();
    readinRecord();
}

SAKInputDataPresetItemManager::~SAKInputDataPresetItemManager()
{
    delete mUi;
}

QList<SAKInputDataPresetItem*> SAKInputDataPresetItemManager::itemList()
{
    QList<SAKInputDataPresetItem*> itemWidgetList;
    for (int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *itemWidget = mListWidget->itemWidget(item);
        itemWidgetList.append(qobject_cast<SAKInputDataPresetItem*>(itemWidget));
    }
    return itemWidgetList;
}

QWidget *innerCreateItem(SAKDebugPageCommonDatabaseInterface::SAKStructPresettingDataItem &var, QListWidget *listWidget)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    SAKInputDataPresetItem *itemWidget = new SAKInputDataPresetItem(var.id,
                                                                    var.format,
                                                                    var.description,
                                                                    var.text,
                                                                    Q_NULLPTR);
    item->setSizeHint(itemWidget->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, itemWidget);

    return itemWidget;
}

void SAKInputDataPresetItemManager::readinRecord()
{
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructPresettingDataItem> itemList = mDatabaseInterface->selectDataPresetItem();
    for (auto &var : itemList){
        QWidget *iw = innerCreateItem(var, mListWidget);
        appendDataPresetItem(iw);
    }
}

void SAKInputDataPresetItemManager::outputMessage(QString msg, bool isError)
{
    QString color = "black";
    if (isError){
        color = "red";
        QApplication::beep();
    }
    mInfoLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    mInfoLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    mClearMessageInfoTimer.start();
}

bool SAKInputDataPresetItemManager::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < mListWidget->count(); i++){
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *w = mListWidget->itemWidget(item);
        SAKInputDataPresetItem *itemWidget = reinterpret_cast<SAKInputDataPresetItem*>(w);
        if (itemWidget->itemID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
}

void SAKInputDataPresetItemManager::appendDataPresetItem(QWidget *iw)
{
    SAKInputDataPresetItem *item = reinterpret_cast<SAKInputDataPresetItem*>(iw);
    connect(item, &SAKInputDataPresetItem::formatChanged, this, &SAKInputDataPresetItemManager::updateFormat);
    connect(item, &SAKInputDataPresetItem::textChanged, this, &SAKInputDataPresetItemManager::updateText);
    connect(item, &SAKInputDataPresetItem::descriptionChanged, this, &SAKInputDataPresetItemManager::updateDescription);
    emit itemAdded(item);
}

void SAKInputDataPresetItemManager::updateFormat(int format)
{
    if (sender()){
        if (sender()->inherits("SAKInputDataPresetItem")){
            SAKInputDataPresetItem *item = qobject_cast<SAKInputDataPresetItem*>(sender());
            quint64 id = item->itemID();

            SAKDebugPageCommonDatabaseInterface::DataPresetItemTable table;
            mDatabaseInterface->updateRecord(mTableName, table.columns.format, QVariant::fromValue(format), id, false);
        }
    }
}

void SAKInputDataPresetItemManager::updateDescription(const QString &text)
{
    if (sender()){
        if (sender()->inherits("SAKInputDataPresetItem")){
            SAKInputDataPresetItem *item = qobject_cast<SAKInputDataPresetItem*>(sender());
            quint64 id = item->itemID();

            SAKDebugPageCommonDatabaseInterface::DataPresetItemTable table;
            mDatabaseInterface->updateRecord(mTableName, table.columns.description, QVariant::fromValue(text), id, true);
            emit descriptionChanged(item);
        }
    }
}

void SAKInputDataPresetItemManager::updateText(QString text)
{
    if (sender()){
        if (sender()->inherits("SAKInputDataPresetItem")){
            SAKInputDataPresetItem *item = qobject_cast<SAKInputDataPresetItem*>(sender());
            quint64 id = item->itemID();

            SAKDebugPageCommonDatabaseInterface::DataPresetItemTable table;
            mDatabaseInterface->updateRecord(mTableName, table.columns.text, QVariant::fromValue(text), id, true);
        }
    }
}

void SAKInputDataPresetItemManager::on_deletePushButton_clicked()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (item){
        SAKInputDataPresetItem *itemWidget = reinterpret_cast<SAKInputDataPresetItem*>(mListWidget->itemWidget(item));
        quint64 id = itemWidget->itemID();
        // delete record from database
        mDatabaseInterface->deleteRecord(mTableName, id);

        mListWidget->removeItemWidget(item);
        emit itemDeleted(itemWidget);
        delete item;
    }else{
        outputMessage(tr("Plese select an item first."));
    }
}

void SAKInputDataPresetItemManager::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(mListWidget);
    SAKInputDataPresetItem *itemWidget = new SAKInputDataPresetItem(this);
    item->setSizeHint(itemWidget->sizeHint());
    mListWidget->addItem(item);
    mListWidget->setItemWidget(item, itemWidget);
    appendDataPresetItem(itemWidget);

    // Add item to database
    SAKDebugPageCommonDatabaseInterface::SAKStructPresettingDataItem dataItem;
    dataItem.id = itemWidget->itemID();
    dataItem.text = itemWidget->itemText();
    dataItem.format = itemWidget->itemTextFromat();
    dataItem.description = itemWidget->itemDescription();
    mDatabaseInterface->insertDataPresetItem(dataItem);
}

void SAKInputDataPresetItemManager::on_outportPushButton_clicked()
{
    // Read in records from database
    QList<SAKDebugPageCommonDatabaseInterface::SAKStructPresettingDataItem> itemList = mDatabaseInterface->selectDataPresetItem();
    if (itemList.isEmpty()){
        return;
    }

    QJsonArray jsonArray;
    DataPresetItemContext itemKey;
    for (auto &var : itemList){
        QJsonObject obj;
        obj.insert(itemKey.id, QVariant::fromValue(var.id).toJsonValue());
        obj.insert(itemKey.format, QVariant::fromValue(var.format).toJsonValue());
        obj.insert(itemKey.description, QVariant::fromValue(var.description).toJsonValue());
        obj.insert(itemKey.text, QVariant::fromValue(var.text).toJsonValue());
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
    }else{
        outputMessage(file.errorString(), false);
    }
}

void SAKInputDataPresetItemManager::on_importPushButton_clicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import data"), defaultPath, QString("json (*.json)"));
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
                DataPresetItemContext itemKey;
                SAKDebugPageCommonDatabaseInterface::SAKStructPresettingDataItem responseItem;
                responseItem.id = jso.value(itemKey.id).toVariant().toULongLong();
                responseItem.format = jso.value(itemKey.format).toVariant().toUInt();
                responseItem.description = jso.value(itemKey.description).toVariant().toString();
                responseItem.text = jso.value(itemKey.text).toVariant().toString();

                // If item is not exist, creating an item
                if (!contains(responseItem.id)){
                    QWidget *iw = innerCreateItem(responseItem, mListWidget);
                    appendDataPresetItem(iw);
                    // Insert record to database
                    mDatabaseInterface->insertDataPresetItem(responseItem);
                }
            }
        }
    }else{
        outputMessage(file.errorString(), false);
    }
}
