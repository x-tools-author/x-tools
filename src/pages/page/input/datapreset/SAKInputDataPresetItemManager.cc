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

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKSqlDatabase.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKInputDataPresetItemManager.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#include "ui_SAKInputDataPresetItemManager.h"

SAKInputDataPresetItemManager::SAKInputDataPresetItemManager(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,ui(new Ui::SAKInputDataPresetItemManager)
{
    ui->setupUi(this);
    deletePushButton = ui->deletePushButton;
    addPushButton = ui->addPushButton;
    listWidget = ui->itemListWidget;
    infoLabel = ui->infoLabel;
    outportPushButton = ui->outportPushButton;
    importPushButton = ui->importPushButton;

    clearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&clearMessageInfoTimer, &QTimer::timeout, this, [&](){
        clearMessageInfoTimer.stop();
        infoLabel->clear();
    });

    tableName = SAKDataStruct::presettingDataTableName(debugPage->pageType());
    databaseInterface = SAKDebugPageCommonDatabaseInterface::instance();
    readinRecord();
}

SAKInputDataPresetItemManager::~SAKInputDataPresetItemManager()
{
    delete ui;
}

QWidget *innerCreateItem(SAKDataStruct::SAKStructPresettingDataItem &var, QListWidget *listWidget)
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
    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList = databaseInterface->selectDataPresetItem(tableName);
    for (auto var : itemList){
        innerCreateItem(var, listWidget);
    }
}

void SAKInputDataPresetItemManager::outputMessage(QString msg, bool isError)
{
    QString color = "black";
    if (isError){
        color = "red";
        QApplication::beep();
    }
    infoLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    infoLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    clearMessageInfoTimer.start();
}

bool SAKInputDataPresetItemManager::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *w = listWidget->itemWidget(item);
        SAKInputDataPresetItem *itemWidget = reinterpret_cast<SAKInputDataPresetItem*>(w);
        if (itemWidget->itemID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
}

void SAKInputDataPresetItemManager::updateFormat(int format)
{
    if (sender()){
        if (sender()->inherits("SAKInputDataPresetItem")){
            SAKInputDataPresetItem *item = qobject_cast<SAKInputDataPresetItem*>(sender());
            quint64 id = item->itemID();

            SAKDebugPageCommonDatabaseInterface::DataPresetItemTable table;
            databaseInterface->updateRecord(tableName, table.columns.format, QVariant::fromValue(format), id, false);
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
            databaseInterface->updateRecord(tableName, table.columns.description, QVariant::fromValue(text), id, true);
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
            databaseInterface->updateRecord(tableName, table.columns.text, QVariant::fromValue(text), id, true);
        }
    }
}

void SAKInputDataPresetItemManager::on_deletePushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (item){
        SAKInputDataPresetItem *itemWidget = reinterpret_cast<SAKInputDataPresetItem*>(listWidget->itemWidget(item));
        SAKDataStruct::SAKStructPresettingDataItem dataItem;
        dataItem.id = itemWidget->itemID();
//        databaseInterface->deletePresettingDataItem(tableName, dataItem);

        listWidget->removeItemWidget(item);
        delete item;
    }
}

void SAKInputDataPresetItemManager::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    SAKInputDataPresetItem *itemWidget = new SAKInputDataPresetItem(this);
    item->setSizeHint(itemWidget->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, itemWidget);

    // Add item to database
    SAKDataStruct::SAKStructPresettingDataItem dataItem;
    dataItem.id = itemWidget->itemID();
    dataItem.text = itemWidget->itemText();
    dataItem.format = itemWidget->itemTextFromat();
    dataItem.description = itemWidget->itemDescription();
    databaseInterface->insertDataPresetItem(tableName, dataItem);
}

void SAKInputDataPresetItemManager::on_outportPushButton_clicked()
{
    /// @brief 从数据库中读入记录
    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList = databaseInterface->selectDataPresetItem(tableName);
    if (itemList.isEmpty()){
        return;
    }

    QJsonArray jsonArray;
    DataPresetItemContext itemKey;
    for (auto var : itemList){
        QJsonObject obj;
        obj.insert(itemKey.id, QVariant::fromValue(var.id).toJsonValue());
        obj.insert(itemKey.format, QVariant::fromValue(var.format).toJsonValue());
        obj.insert(itemKey.description, QVariant::fromValue(var.description).toJsonValue());
        obj.insert(itemKey.text, QVariant::fromValue(var.text).toJsonValue());
        jsonArray.append(QJsonValue(obj));
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);

    /// @brief 打开文件，导出的数据将保存至该文件
    QString defaultName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    defaultName.append(QString("/"));
    defaultName.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    defaultName.append(".json");
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出数据"), defaultName, QString("json (*.json)"));
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

void SAKInputDataPresetItemManager::on_importPushButton_clicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("导出数据"), defaultPath, QString("json (*.json)"));
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
                SAKDataStruct::SAKStructPresettingDataItem responseItem;
                responseItem.id = jso.value(itemKey.id).toVariant().toULongLong();
                responseItem.format = jso.value(itemKey.format).toVariant().toUInt();
                responseItem.description = jso.value(itemKey.description).toVariant().toString();
                responseItem.classify = jso.value(itemKey.classify).toVariant().toUInt();
                responseItem.text = jso.value(itemKey.text).toVariant().toString();

                /// @brief 不存在则新建
                if (!contains(responseItem.id)){
                    innerCreateItem(responseItem, listWidget);
//                    databaseInterface->insertPresettingDataItem(tableName, responseItem);
                }
            }
        }
    }else{
        outputMessage(file.errorString(), false);
    }
}
