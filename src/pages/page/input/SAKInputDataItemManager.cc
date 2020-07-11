/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
#include "SAKCRCInterface.hh"
#include "SAKInputDataItem.hh"
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
    databaseInterface = SAKDebugPageDatabaseInterface::instance();
    readinRecord();
}

SAKInputDataItemManager::~SAKInputDataItemManager()
{
    delete crcInterface;
    delete ui;
    delete factory;
}

void innerCreateItem(SAKDataStruct::SAKStructPresettingDataItem &var, SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QListWidget *listWidget, SAKInputDataItemManager *itemManager)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    SAKInputDataItem *itemWidget = new SAKInputDataItem(var.id,
                                                        var.format,
                                                        var.comment,
                                                        var.classify,
                                                        var.data,
                                                        debugPage,
                                                        inputManager,
                                                        itemManager);
    item->setSizeHint(itemWidget->sizeHint());
    listWidget->addItem(item);
    listWidget->setItemWidget(item, itemWidget);
}

void SAKInputDataItemManager::readinRecord()
{
    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList = databaseInterface->selectPresettingDataItem(tableName);
    for (auto var : itemList){
        innerCreateItem(var, debugPage, inputManager, listWidget, this);
    }
}

void SAKInputDataItemManager::outputMessage(QString msg, bool isError)
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

bool SAKInputDataItemManager::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *w = listWidget->itemWidget(item);
        SAKInputDataItem *itemWidget = reinterpret_cast<SAKInputDataItem*>(w);
        if (itemWidget->parameterID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
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

void SAKInputDataItemManager::on_outportPushButton_clicked()
{
    /// @brief 从数据库中读入记录
    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList = databaseInterface->selectPresettingDataItem(tableName);
    if (itemList.isEmpty()){
        return;
    }

    QJsonArray jsonArray;
    InputDataItemKey itemKey;
    for (auto var : itemList){
        QJsonObject obj;
        obj.insert(itemKey.id, QVariant::fromValue(var.id).toJsonValue());
        obj.insert(itemKey.format, QVariant::fromValue(var.format).toJsonValue());
        obj.insert(itemKey.comment, QVariant::fromValue(var.comment).toJsonValue());
        obj.insert(itemKey.classify, QVariant::fromValue(var.classify).toJsonValue());
        obj.insert(itemKey.data, QVariant::fromValue(var.data).toJsonValue());
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

void SAKInputDataItemManager::on_importPushButton_clicked()
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
                InputDataItemKey itemKey;
                SAKDataStruct::SAKStructPresettingDataItem responseItem;
                responseItem.id = jso.value(itemKey.id).toVariant().toULongLong();
                responseItem.format = jso.value(itemKey.format).toVariant().toUInt();
                responseItem.comment = jso.value(itemKey.comment).toVariant().toString();
                responseItem.classify = jso.value(itemKey.classify).toVariant().toUInt();
                responseItem.data = jso.value(itemKey.data).toVariant().toString();

                /// @brief 不存在则新建
                if (!contains(responseItem.id)){
                    innerCreateItem(responseItem, debugPage, inputManager, listWidget, this);
                    databaseInterface->insertPresettingDataItem(tableName, responseItem);
                }
            }
        }
    }else{
        outputMessage(file.errorString(), false);
    }
}
