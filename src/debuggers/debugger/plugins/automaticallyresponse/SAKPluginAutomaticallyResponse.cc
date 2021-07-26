/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
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
#include "SAKPluginAutomaticallyResponseItem.hh"
#include "SAKPluginAutomaticallyResponse.hh"

#include "ui_SAKPluginAutomaticallyResponse.h"

SAKPluginAutomaticallyResponse::SAKPluginAutomaticallyResponse(
        QSettings *settings,
        QString settingsGroup,
        QSqlDatabase *sqlDatabase,
        QWidget *parent
        )
    :QWidget (parent)
    ,mSettiings(settings)
    ,mSqlDatabase(sqlDatabase)
    ,mSqlQuery(*sqlDatabase)
    ,mForbidAll(false)
    ,mUi (new Ui::SAKPluginAutomaticallyResponse)
{
    mUi->setupUi(this);
    mSettingsKeyCtx.disableAutomaticallyResponse =
            settingsGroup + "/disableAllAutoResponse";
    mSqlDatabaseTableCtx.tableName = settingsGroup + "AutomaticallyResponse";


    mClearMessageInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    mClearMessageInfoTimer.setSingleShot(true);
    connect(&mClearMessageInfoTimer, &QTimer::timeout,
            mUi->msgLabel, &QLabel::clear);


    bool forbidAll =
            settings->value(mSettingsKeyCtx.disableAutomaticallyResponse).toBool();
    mForbidAll = forbidAll;
    mUi->forbidAllCheckBox->setChecked(mForbidAll);
    connect(mUi->forbidAllCheckBox, &QCheckBox::clicked, this, [&](){
        mForbidAll = mUi->forbidAllCheckBox->isChecked();
        mSettiings->setValue(mSettingsKeyCtx.disableAutomaticallyResponse, mForbidAll);
    });


    connect(mUi->outportPushButton, &QPushButton::clicked,
            this, &SAKPluginAutomaticallyResponse::exportItems);
    connect(mUi->importPushButton, &QPushButton::clicked,
            this, &SAKPluginAutomaticallyResponse::importItems);
    connect(mUi->deleteItemPushButton, &QPushButton::clicked,
            this, &SAKPluginAutomaticallyResponse::deleteItem);
    connect(mUi->addItemPushButton, &QPushButton::clicked,
            this, &SAKPluginAutomaticallyResponse::addItemWidthoutParameters);


    createSqlDatabaseTable();
    readInRecord();
}

SAKPluginAutomaticallyResponse::~SAKPluginAutomaticallyResponse()
{
    delete mUi;
}

void SAKPluginAutomaticallyResponse::onBytesRead(QByteArray bytes)
{
    if (!mForbidAll) {
        emit bytesRead(bytes);
    }
}

void SAKPluginAutomaticallyResponse::outputMessage(QString msg, bool isInfo)
{
    QString color = isInfo ? "black" : "red";
    mUi->msgLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));
    mUi->msgLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);

    mClearMessageInfoTimer.start();
    if (!isInfo){
        QApplication::beep();
    }
}

void SAKPluginAutomaticallyResponse::readInRecord()
{
    const QString queryString = QString("SELECT * FROM %1")
            .arg(mSqlDatabaseTableCtx.tableName);
    if (mSqlQuery.exec(queryString)) {
        SAKPluginAutomaticallyResponseItem::ITEM_CTX item;
        while (mSqlQuery.next()) {
            QString column;
            QVariant valueVariant;

            column = mSqlDatabaseTableCtx.columns.id;
            valueVariant = mSqlQuery.value(column);
            item.id = valueVariant.toULongLong();

            column = mSqlDatabaseTableCtx.columns.name;
            valueVariant = mSqlQuery.value(column);
            item.name = valueVariant.toString();

            column = mSqlDatabaseTableCtx.columns.referenceData;
            valueVariant = mSqlQuery.value(column);
            item.referenceData = valueVariant.toString();

            column = mSqlDatabaseTableCtx.columns.responseData;
            valueVariant = mSqlQuery.value(column);
            item.responseData =valueVariant.toString();

            column = mSqlDatabaseTableCtx.columns.enable;
            valueVariant = mSqlQuery.value(column);
            item.enable = valueVariant.toBool();

            column = mSqlDatabaseTableCtx.columns.referenceFormat;
            valueVariant = mSqlQuery.value(column);
            item.referenceFormat = valueVariant.toUInt();

            column = mSqlDatabaseTableCtx.columns.responseFormat;
            valueVariant = mSqlQuery.value(column);
            item.responseFormat = valueVariant.toUInt();

            column = mSqlDatabaseTableCtx.columns.option;
            valueVariant = mSqlQuery.value(column);
            item.option = valueVariant.toUInt();

            column = mSqlDatabaseTableCtx.columns.delay;
            valueVariant = mSqlQuery.value(column);
            item.delay = valueVariant.toBool();

            column = mSqlDatabaseTableCtx.columns.interval;
            valueVariant = mSqlQuery.value(column);
            item.interval = valueVariant.toInt();

            addItem(item);
        }
    } else {
        qWarning() << "Select record form "
                   << mSqlDatabaseTableCtx.tableName
                   << " table failed: "
                   << mSqlQuery.lastError().text();
    }
}

void SAKPluginAutomaticallyResponse::
insertRecord(SAKPluginAutomaticallyResponseItem::ITEM_CTX itemCtx)
{
    auto table = mSqlDatabaseTableCtx;
    const QString queryString = QString("INSERT INTO %1\
                                         (%2,%3,%4,%5,%6,%7,%8,%9,%10,%11)\
                                         VALUES(%12,'%13','%14','%15',%16,%17,%18,%19,\
                                         %20,'%21')")
            .arg(table.tableName)
            .arg(table.columns.id)
            .arg(table.columns.name)
            .arg(table.columns.referenceData)
            .arg(table.columns.responseData)
            .arg(table.columns.enable)
            .arg(table.columns.referenceFormat)
            .arg(table.columns.responseFormat)
            .arg(table.columns.option)
            .arg(table.columns.delay)
            .arg(table.columns.interval)
            .arg(itemCtx.id)
            .arg(itemCtx.name)
            .arg(itemCtx.referenceData)
            .arg(itemCtx.responseData)
            .arg(itemCtx.enable)
            .arg(itemCtx.referenceFormat)
            .arg(itemCtx.responseFormat)
            .arg(itemCtx.option)
            .arg(itemCtx.delay)
            .arg(itemCtx.interval);
    if (mSqlQuery.exec(queryString)){
        qWarning() << "Insert record to " << table.tableName
                   << " table failed: " << mSqlQuery.lastError().text();
    }
}

void SAKPluginAutomaticallyResponse::
addItem(SAKPluginAutomaticallyResponseItem::ITEM_CTX itemCtx)
{
    QListWidget *listWidget = mUi->listWidget;
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    auto *itemWidget = new SAKPluginAutomaticallyResponseItem(
                itemCtx.id,
                itemCtx.name,
                itemCtx.referenceData,
                itemCtx.responseData,
                itemCtx.enable,
                itemCtx.referenceFormat,
                itemCtx.responseFormat,
                itemCtx.option,
                itemCtx.delay,
                itemCtx.interval,
                listWidget
                );
    setupResponseItem(itemWidget);
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);
    insertRecord(itemCtx);
}

void SAKPluginAutomaticallyResponse::exportItems()
{
    SAKStructAutomaticallyResponseJsonKeyContext itemKey;
    QListWidget *listWidget = mUi->listWidget;
    QJsonArray jsonArray;
    for (int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *itemWidget = listWidget->itemWidget(item);
        auto cookedItemWidget
                = qobject_cast<SAKPluginAutomaticallyResponseItem*>(itemWidget);
        auto itemCtx = cookedItemWidget->context();
        QJsonObject obj;
        obj.insert(itemKey.id,
                   QVariant::fromValue(itemCtx.id).toJsonValue());
        obj.insert(itemKey.name,
                   QVariant::fromValue(itemCtx.name).toJsonValue());
        obj.insert(itemKey.enable,
                   QVariant::fromValue(itemCtx.enable).toJsonValue());
        obj.insert(itemKey.option,
                   QVariant::fromValue(itemCtx.option).toJsonValue());
        obj.insert(itemKey.responseData,
                   QVariant::fromValue(itemCtx.responseData).toJsonValue());
        obj.insert(itemKey.referenceData,
                   QVariant::fromValue(itemCtx.referenceData).toJsonValue());
        obj.insert(itemKey.responseFormat,
                   QVariant::fromValue(itemCtx.responseFormat).toJsonValue());
        obj.insert(itemKey.referenceFormat,
                   QVariant::fromValue(itemCtx.referenceFormat).toJsonValue());
        jsonArray.append(QJsonValue(obj));
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);


    auto location = QStandardPaths::DesktopLocation;
    QString defaultName = QStandardPaths::writableLocation(location);
    defaultName.append(QString("/"));
    defaultName.append(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    defaultName.append(".json");
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Outport data"),
                                                    defaultName,
                                                    QString("json (*.json)"));
    if (fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadWrite)) {
            file.write(jsonDoc.toJson());
            file.close();
        } else {
            outputMessage(file.errorString(), false);
        }
    }
}

void SAKPluginAutomaticallyResponse::importItems()
{
    auto location = QStandardPaths::DesktopLocation;
    QString defaultPath = QStandardPaths::writableLocation(location);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import file"),
                                                    defaultPath,
                                                    QString("json (*.json)"));
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
        SAKStructAutomaticallyResponseJsonKeyContext itemKey;
        SAKPluginAutomaticallyResponseItem::ITEM_CTX itemCtx;
        for (int i = 0; i < jsa.count(); i++){
            QJsonObject jso = jsa.at(i).toObject();
            if (!jso.isEmpty()) {
                continue;
            }

            itemCtx.id = jso.value(itemKey.id).toVariant().toULongLong();
            itemCtx.name = jso.value(itemKey.name).toVariant().toString();
            itemCtx.enable = jso.value(itemKey.enable).toVariant().toBool();
            itemCtx.option = jso.value(itemKey.option).toVariant().toUInt();
            itemCtx.responseData =
                    jso.value(itemKey.responseData).toVariant().toString();
            itemCtx.referenceData =
                    jso.value(itemKey.referenceData).toVariant().toString();
            itemCtx.responseFormat =
                    jso.value(itemKey.responseFormat).toVariant().toUInt();
            itemCtx.referenceFormat =
                    jso.value(itemKey.referenceFormat).toVariant().toUInt();


            /// @brief If the item is not exist, new a new item.
            if (!itemIsExisted(itemCtx.id)){
                addItem(itemCtx);
            }
        }
    }else{
        outputMessage(file.errorString(), false);
    }
}

bool SAKPluginAutomaticallyResponse::itemIsExisted(quint64 id)
{
    QListWidget *listWidget = mUi->listWidget;
    QJsonArray jsonArray;
    for (int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *itemWidget = listWidget->itemWidget(item);
        auto cookedItemWidget
                = qobject_cast<SAKPluginAutomaticallyResponseItem*>(itemWidget);
        auto itemCtx = cookedItemWidget->context();

        if (itemCtx.id == id) {
            return true;
        }
    }

    return false;
}

void SAKPluginAutomaticallyResponse::deleteItem()
{
    QListWidget *listWidget = mUi->listWidget;
    QListWidgetItem *item = listWidget->currentItem();
    if (item) {
        outputMessage(tr("Please select an item first!"), false);
        return;
    }

    QWidget *itemWidget = listWidget->itemWidget(item);
    auto cookedItemWidget =
            qobject_cast<SAKPluginAutomaticallyResponseItem*>(itemWidget);
    quint64 id = cookedItemWidget->context().id;
    SAKDebugger::commonSqlApiDeleteRecord(&mSqlQuery,
                                          mSqlDatabaseTableCtx.tableName,
                                          id);
    cookedItemWidget->deleteLater();
    delete item;
}

void SAKPluginAutomaticallyResponse::addItemWidthoutParameters()
{
    QListWidget *listWidget = mUi->listWidget;
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    auto *itemWidget = new SAKPluginAutomaticallyResponseItem(listWidget);
    setupResponseItem(itemWidget);
    item->setSizeHint(QSize(itemWidget->width(), itemWidget->height()));
    listWidget->setItemWidget(item, itemWidget);

    auto itemCtx = itemWidget->context();
    insertRecord(itemCtx);
}

void SAKPluginAutomaticallyResponse::setupResponseItem(
        SAKPluginAutomaticallyResponseItem* item
        )
{
    connect(this, &SAKPluginAutomaticallyResponse::bytesRead,
            item, &SAKPluginAutomaticallyResponseItem::onBytesRead);

    connect(item, &SAKPluginAutomaticallyResponseItem::responseBytes,
            this, &SAKPluginAutomaticallyResponse::writeBytes);
}

void SAKPluginAutomaticallyResponse::createSqlDatabaseTable()
{
    if (!mSqlDatabase->tables().contains(mSqlDatabaseTableCtx.tableName)) {
        QString queryString;
        queryString.append(QString("CREATE TABLE %1(")
                           .arg(mSqlDatabaseTableCtx.tableName));
        queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.id));

        queryString.append(QString("%1 TEXT NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.name));

        queryString.append(QString("%1 TEXT NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.referenceData));

        queryString.append(QString("%1 TEXT NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.responseData));

        queryString.append(QString("%1 INTEGER NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.enable));

        queryString.append(QString("%1 INTEGER NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.referenceFormat));

        queryString.append(QString("%1 INTEGER NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.responseFormat));

        queryString.append(QString("%1 INTEGER NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.option));

        queryString.append(QString("%1 INTEGER NOT NULL,")
                           .arg(mSqlDatabaseTableCtx.columns.delay));

        queryString.append(QString("%1 INTEGER NOT NULL)")
                           .arg(mSqlDatabaseTableCtx.columns.interval));

        if (!mSqlQuery.exec(queryString)) {
            qInfo() << queryString;
            qWarning() << mSqlQuery.lastError().text();
        }
    }
}
