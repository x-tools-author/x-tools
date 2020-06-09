/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QDomElement>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

#include "SAKAutoResponseItemWidget.hh"
#include "SAKAutoResponseSettingsWidget.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKAutoResponseSettingsWidget.h"

SAKAutoResponseSettingsWidget::SAKAutoResponseSettingsWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget (parent)
    ,debugPage (debugPage)
    ,ui (new Ui::SAKAutoResponseSettingsWidget)
{
    ui->setupUi(this);
    listWidget = ui->listWidget;
    forbidAllCheckBox = ui->forbidAllCheckBox;
    deleteItemPushButton = ui->deleteItemPushButton;
    addItemPushButton = ui->addItemPushButton;
    msgLabel = ui->msgLabel;

    setWindowTitle(tr("自动回复设置"));
    databaseInterface = SAKDebugPageDatabaseInterface::instance();

    clearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&clearMessageInfoTimer, &QTimer::timeout, this, &SAKAutoResponseSettingsWidget::clearMessage);

    /// @brief 从数据库读入记录
    readInRecord();
}

SAKAutoResponseSettingsWidget::~SAKAutoResponseSettingsWidget()
{
    delete ui;
}

void SAKAutoResponseSettingsWidget::on_forbidAllCheckBox_clicked()
{
    for(int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *widget = listWidget->itemWidget(item);
        bool disAble = forbidAllCheckBox->isChecked();
        reinterpret_cast<SAKAutoResponseItemWidget*>(widget)->setAllAutoResponseDisable(disAble);
    }
}

void SAKAutoResponseSettingsWidget::on_deleteItemPushButton_clicked()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (!item){
        outputMessage(tr("未选择行，请选择要删除的行后重试"), false);
        return;
    }

    /// @brief 删除数据库记录
    QString tableName = SAKDataStruct::autoResponseTableName(debugPage->pageType());
    SAKDataStruct::SAKStructAutoResponseItem dataItem;
    SAKAutoResponseItemWidget *w = reinterpret_cast<SAKAutoResponseItemWidget*>(listWidget->itemWidget(item));
    dataItem.id = w->parameterID();
    databaseInterface->deleteAutoResponseItem(tableName, dataItem);

    listWidget->removeItemWidget(item);
    delete item;
}

void SAKAutoResponseSettingsWidget::on_addItemPushButton_clicked()
{
    /// @brief 限制数量
    if (listWidget->count() >= SAK_MAX_AUTO_RESPONSE_COUNT){
        outputMessage(tr("数据已达到上限，不能新建数据"), false);
        return;
    }

    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    SAKAutoResponseItemWidget *itemWidget = new SAKAutoResponseItemWidget(debugPage, listWidget);
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);

    /// @brief 添加数据库记录
    SAKDataStruct::SAKStructAutoResponseItem dataItem;
    dataItem.id = itemWidget->parameterID();
    dataItem.name = itemWidget->parameterName();
    dataItem.enabled = itemWidget->parameterEnable();
    dataItem.responseData = itemWidget->parameterResponseData();
    dataItem.referenceData = itemWidget->parameterRefernceData();
    dataItem.responseFormat = itemWidget->parameterResponseFormat();
    dataItem.referenceFormat = itemWidget->parameterReferenceFormat();
    QString tableName = SAKDataStruct::autoResponseTableName(debugPage->pageType());
    databaseInterface->insertAutoResponseItem(tableName, dataItem);
}

void SAKAutoResponseSettingsWidget::outputMessage(QString msg, bool isInfo)
{
    QString color = "black";
    if (!isInfo){
        color = "red";
        QApplication::beep();
    }
    msgLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));

    msgLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);
    clearMessageInfoTimer.start();
}

void SAKAutoResponseSettingsWidget::clearMessage()
{
    clearMessageInfoTimer.stop();
    msgLabel->clear();
}

void SAKAutoResponseSettingsWidget::readInRecord()
{
    QString tableName = SAKDataStruct::autoResponseTableName(debugPage->pageType());
    QList<SAKDataStruct::SAKStructAutoResponseItem> itemList = databaseInterface->selectAutoResponseItem(tableName);
    for (auto var : itemList){
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        listWidget->addItem(item);
        SAKAutoResponseItemWidget *itemWidget = new SAKAutoResponseItemWidget(debugPage,
                                                                              var.id,
                                                                              var.name,
                                                                              var.referenceData,
                                                                              var.responseData,
                                                                              var.enabled,
                                                                              var.referenceFormat,
                                                                              var.responseFormat,
                                                                              listWidget);
        item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
        listWidget->setItemWidget(item, itemWidget);
    }
}
