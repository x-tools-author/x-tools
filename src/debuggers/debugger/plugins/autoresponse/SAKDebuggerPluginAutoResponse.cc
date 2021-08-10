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
#include "SAKDebuggerPluginAutoResponse.hh"

#include "ui_SAKPluginAutomaticallyResponse.h"

SAKDebuggerPluginAutoResponse::SAKDebuggerPluginAutoResponse(
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
            this, &SAKDebuggerPluginAutoResponse::exportItems);
    connect(mUi->importPushButton, &QPushButton::clicked,
            this, &SAKDebuggerPluginAutoResponse::importItems);
    connect(mUi->deleteItemPushButton, &QPushButton::clicked,
            this, &SAKDebuggerPluginAutoResponse::deleteItem);
    connect(mUi->addItemPushButton, &QPushButton::clicked,
            this, &SAKDebuggerPluginAutoResponse::addItemWidthoutParameters);
    connect(mUi->clearItemsPushButton, &QPushButton::clicked,
            this, &SAKDebuggerPluginAutoResponse::clearItems);


    createSqlDatabaseTable();
    readInRecord();
}

SAKDebuggerPluginAutoResponse::~SAKDebuggerPluginAutoResponse()
{
    delete mUi;
}

void SAKDebuggerPluginAutoResponse::onBytesRead(const QByteArray &bytes)
{
    if (!mForbidAll) {
        emit bytesRead(bytes);
    }
}

void SAKDebuggerPluginAutoResponse::outputMessage(QString msg, bool isInfo)
{
    QString color = isInfo ? "black" : "red";
    mUi->msgLabel->setStyleSheet(QString("QLabel{color:%1}").arg(color));
    mUi->msgLabel->setText(QTime::currentTime().toString("hh:mm:ss ") + msg);

    mClearMessageInfoTimer.start();
    if (!isInfo){
        QApplication::beep();
    }
}

void SAKDebuggerPluginAutoResponse::readInRecord()
{
    const QString queryString = QString("SELECT * FROM %1")
            .arg(mSqlDatabaseTableCtx.tableName);
    if (mSqlQuery.exec(queryString)) {
        SAKDebuggerPluginAutoResponseItem::ITEM_CTX item;
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

void SAKDebuggerPluginAutoResponse::
insertRecord(SAKDebuggerPluginAutoResponseItem::ITEM_CTX itemCtx)
{
    auto tableCtx = mSqlDatabaseTableCtx;
    QString queryString = QString("INSERT INTO %1(").arg(tableCtx.tableName);
    queryString.append(tableCtx.columns.id).append(",");
    queryString.append(tableCtx.columns.name).append(",");
    queryString.append(tableCtx.columns.referenceData).append(",");
    queryString.append(tableCtx.columns.responseData).append(",");
    queryString.append(tableCtx.columns.enable).append(",");
    queryString.append(tableCtx.columns.referenceFormat).append(",");
    queryString.append(tableCtx.columns.responseFormat).append(",");
    queryString.append(tableCtx.columns.option).append(",");
    queryString.append(tableCtx.columns.delay).append(",");
    queryString.append(tableCtx.columns.interval).append(")");
    queryString.append("VALUES(");
    queryString.append(QString("%1,").arg(itemCtx.id));
    queryString.append(QString("'%1',").arg(itemCtx.name));
    queryString.append(QString("'%1',").arg(itemCtx.referenceData));
    queryString.append(QString("'%1',").arg(itemCtx.responseData));
    queryString.append(QString("%1,").arg(itemCtx.enable));
    queryString.append(QString("%1,").arg(itemCtx.referenceFormat));
    queryString.append(QString("%1,").arg(itemCtx.responseFormat));
    queryString.append(QString("%1,").arg(itemCtx.option));
    queryString.append(QString("%1,").arg(itemCtx.delay));
    queryString.append(QString("%1)").arg(itemCtx.interval));
    if (!mSqlQuery.exec(queryString)) {
        qInfo() << queryString;
        qWarning() << "Insert record to " << tableCtx.tableName
                   << " table failed: " << mSqlQuery.lastError().text();
    }
}

void SAKDebuggerPluginAutoResponse::
addItem(SAKDebuggerPluginAutoResponseItem::ITEM_CTX itemCtx)
{
    QListWidget *listWidget = mUi->listWidget;
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    auto *itemWidget = new SAKDebuggerPluginAutoResponseItem(
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
    setItemWidget(item, itemWidget, listWidget);
}

void SAKDebuggerPluginAutoResponse::exportItems()
{
    SAKStructAutomaticallyResponseJsonKeyContext itemKey;
    QListWidget *listWidget = mUi->listWidget;
    QJsonArray jsonArray;
    for (int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *itemWidget = listWidget->itemWidget(item);
        auto cookedItemWidget
                = qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
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
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadWrite)) {
            file.write(jsonDoc.toJson());
            file.close();
        } else {
            outputMessage(file.errorString(), false);
        }
    }
}

void SAKDebuggerPluginAutoResponse::importItems()
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
        SAKDebuggerPluginAutoResponseItem::ITEM_CTX itemCtx;
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

bool SAKDebuggerPluginAutoResponse::itemIsExisted(quint64 id)
{
    QListWidget *listWidget = mUi->listWidget;
    for (int i = 0; i < listWidget->count(); i++){
        QListWidgetItem *item = listWidget->item(i);
        QWidget *itemWidget = listWidget->itemWidget(item);
        auto cookedItemWidget
                = qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
        auto itemCtx = cookedItemWidget->context();

        if (itemCtx.id == id) {
            return true;
        }
    }

    return false;
}

void SAKDebuggerPluginAutoResponse::deleteItem()
{
    QListWidget *listWidget = mUi->listWidget;
    QListWidgetItem *item = listWidget->currentItem();
    if (!item) {
        outputMessage(tr("Please select an item first!"), false);
        return;
    }

    QWidget *itemWidget = listWidget->itemWidget(item);
    auto cookedItemWidget =
            qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
    quint64 id = cookedItemWidget->context().id;
    SAKDebugger::commonSqlApiDeleteRecord(&mSqlQuery,
                                          mSqlDatabaseTableCtx.tableName,
                                          id);
    cookedItemWidget->deleteLater();
    delete item;
}

void SAKDebuggerPluginAutoResponse::addItemWidthoutParameters()
{
    QListWidget *listWidget = mUi->listWidget;
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    listWidget->addItem(item);
    auto *itemWidget = new SAKDebuggerPluginAutoResponseItem(listWidget);
    setItemWidget(item, itemWidget, listWidget);

    auto itemCtx = itemWidget->context();
    insertRecord(itemCtx);
}

void SAKDebuggerPluginAutoResponse::createSqlDatabaseTable()
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

void SAKDebuggerPluginAutoResponse::clearItems()
{
    int ret = SAKDebugger::clearDataMessageBox();
    if (ret == QMessageBox::Ok) {
        QListWidget *listWidget = mUi->listWidget;
        while (listWidget->count()) {
            auto item = listWidget->item(0);
            auto itemWidget =
                    qobject_cast<SAKDebuggerPluginAutoResponseItem*>(
                        listWidget->itemWidget(item)
                        );
            auto id = itemWidget->context().id;
            delete item;
            delete itemWidget;
            SAKDebugger::commonSqlApiDeleteRecord(&mSqlQuery,
                                                  mSqlDatabaseTableCtx.tableName,
                                                  id);
        }
    }
}

void SAKDebuggerPluginAutoResponse::setItemWidget(QListWidgetItem *item,
                        SAKDebuggerPluginAutoResponseItem *itemWidget,
                        QListWidget *listWidget)
{
    item->setSizeHint(itemWidget->sizeHint());
    listWidget->setItemWidget(item, itemWidget);

    connect(this, &SAKDebuggerPluginAutoResponse::bytesRead,
            itemWidget, &SAKDebuggerPluginAutoResponseItem::onBytesRead);

    connect(itemWidget, &SAKDebuggerPluginAutoResponseItem::responseBytes,
            this, &SAKDebuggerPluginAutoResponse::writeBytes);
}
