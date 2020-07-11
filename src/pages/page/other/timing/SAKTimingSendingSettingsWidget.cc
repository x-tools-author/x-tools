/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKTimingSendingItemWidget.hh"
#include "SAKDebugPageDatabaseInterface.hh"
#include "SAKTimingSendingSettingsWidget.hh"

#include "ui_SAKTimingSendingSettingsWidget.h"

SAKTimingSendingSettingsWidget::SAKTimingSendingSettingsWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,debugPage(debugPage)
    ,ui(new Ui::SAKTimingSendingSettingsWidget)
{
    ui->setupUi(this);

    itemListWidget = ui->itemListWidget;
    outportPushButton = ui->outportPushButton;
    importPushButton = ui->importPushButton;
    deletePushButton = ui->deletePushButton;
    addPushButton = ui->addPushButton;
    messageLabel = ui->messageLabel;

    clearMessageTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&clearMessageTimer, &QTimer::timeout, this, [&](){
        clearMessageTimer.stop();
        messageLabel->clear();
    });

    databaseInterface = SAKDebugPageDatabaseInterface::instance();
    tableName = SAKDataStruct::timingSendingTableName(this->debugPage->pageType());
    readinRecord();
}

SAKTimingSendingSettingsWidget::~SAKTimingSendingSettingsWidget()
{
    delete ui;
}

void innerCreateItem(SAKDataStruct::SAKStructTimingSendingItem &var, SAKDebugPage *debugPage, QListWidget *listWidget)
{
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    SAKTimingSendingItemWidget *itemWidget = new SAKTimingSendingItemWidget(debugPage,
                                                                            var.id,
                                                                            var.interval,
                                                                            var.format,
                                                                            var.comment,
                                                                            var.data,
                                                                            Q_NULLPTR);
    item->setSizeHint(itemWidget->size());
    listWidget->setItemWidget(item, itemWidget);
}

void SAKTimingSendingSettingsWidget::readinRecord()
{
    QList<SAKDataStruct::SAKStructTimingSendingItem> itemList = databaseInterface->selectTimingSendingItem(tableName);
    if (itemList.isEmpty()){
        return;
    }

    for (auto var : itemList){
        innerCreateItem(var, debugPage, itemListWidget);
    }
}

bool SAKTimingSendingSettingsWidget::contains(quint64 paraID)
{
    bool contain = false;
    for (int i = 0; i < itemListWidget->count(); i++){
        QListWidgetItem *item = itemListWidget->item(i);
        QWidget *w = itemListWidget->itemWidget(item);
        SAKTimingSendingItemWidget *itemWidget = reinterpret_cast<SAKTimingSendingItemWidget*>(w);
        if (itemWidget->parameterID() == paraID){
            contain = true;
            break;
        }
    }

    return contain;
}

void SAKTimingSendingSettingsWidget::outputMessage(QString msg, bool isError)
{
    QString color = "black";
    if (isError){
        color = "red";
        QApplication::beep();
    }
    messageLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));
    messageLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    clearMessageTimer.start();
}

void SAKTimingSendingSettingsWidget::on_outportPushButton_clicked()
{
    QList<SAKDataStruct::SAKStructTimingSendingItem> itemList = databaseInterface->selectTimingSendingItem(tableName);
    if (itemList.isEmpty()){
        return;
    }

    QJsonArray jsonArray;
    TimingSendingItemKey itemKey;
    for (auto var : itemList){
        QJsonObject obj;
        obj.insert(itemKey.id, QVariant::fromValue(var.id).toJsonValue());
        obj.insert(itemKey.data, QVariant::fromValue(var.data).toJsonValue());
        obj.insert(itemKey.format, QVariant::fromValue(var.format).toJsonValue());
        obj.insert(itemKey.comment, QVariant::fromValue(var.comment).toJsonValue());
        obj.insert(itemKey.interval, QVariant::fromValue(var.interval).toJsonValue());
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
    }
}

void SAKTimingSendingSettingsWidget::on_importPushButton_clicked()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("导出数据"), defaultPath, QString("json (*.json)"));
    QFile file(fileName);
    if (file.open(QFile::ReadWrite)){
        QByteArray array = file.readAll();
        file.close();

        QJsonDocument jsc = QJsonDocument::fromJson(array);
        if (!jsc.isArray()){
            outputMessage(tr("文件数据格式有误"), true);
            return;
        }

        QJsonArray jsa = jsc.array();
        for (int i = 0; i < jsa.count(); i++){
            if (jsa.at(i).isObject()){
                QJsonObject jso = jsa.at(i).toObject();
                TimingSendingItemKey itemKey;
                SAKDataStruct::SAKStructTimingSendingItem responseItem;
                responseItem.id = jso.value(itemKey.id).toVariant().toULongLong();
                responseItem.data = jso.value(itemKey.data).toVariant().toString();
                responseItem.format = jso.value(itemKey.format).toVariant().toUInt();
                responseItem.comment = jso.value(itemKey.comment).toVariant().toString();
                responseItem.interval = jso.value(itemKey.interval).toVariant().toUInt();

                /// @brief 不存在则新建
                if (!contains(responseItem.id)){
                    innerCreateItem(responseItem, debugPage, itemListWidget);
                    databaseInterface->insertTimingSendingItem(tableName, responseItem);
                }
            }
        }
    }else{
        outputMessage(file.errorString(), true);
    }
}

void SAKTimingSendingSettingsWidget::on_deletePushButton_clicked()
{
    QListWidgetItem *currentItem = itemListWidget->currentItem();
    if (currentItem){
        SAKTimingSendingItemWidget *w = reinterpret_cast<SAKTimingSendingItemWidget*>(itemListWidget->itemWidget(currentItem));
        SAKDataStruct::SAKStructTimingSendingItem sendingItem;
        sendingItem.id = w->parameterID();
        databaseInterface->deleteTimingSendingItem(tableName, sendingItem);

        itemListWidget->removeItemWidget(currentItem);
        delete currentItem;
    }
}

void SAKTimingSendingSettingsWidget::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(itemListWidget);
    itemListWidget->addItem(item);

    SAKTimingSendingItemWidget *itemWidget = new SAKTimingSendingItemWidget(debugPage);
    item->setSizeHint(itemWidget->size());
    itemListWidget->setItemWidget(item, itemWidget);

    /// @brief 插入定时发送记录
    SAKDataStruct::SAKStructTimingSendingItem sendingItem;
    sendingItem.id = itemWidget->parameterID();
    sendingItem.data = itemWidget->parameterData();
    sendingItem.format = itemWidget->parameterFormat();
    sendingItem.comment = itemWidget->parameterComment();
    sendingItem.interval = itemWidget->parameterInterval();
    databaseInterface->insertTimingSendingItem(tableName, sendingItem);
}
