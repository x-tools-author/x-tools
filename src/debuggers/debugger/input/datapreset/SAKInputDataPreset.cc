/******************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QTime>
#include <QDebug>
#include <QSqlError>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKDebugger.hh"
#include "SAKInputDataPreset.hh"
#include "SAKInputDataPresetItem.hh"

#include "ui_SAKInputDataPreset.h"

SAKInputDataPreset::SAKInputDataPreset(QSqlDatabase *sqlDatabase,
                                       QSettings *settings,
                                       QString settingsGroup,
                                       QMenu *itemsMenu,
                                       QWidget *parent)
    :QWidget(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
    ,mItemsMenu(itemsMenu)
    ,mUi(new Ui::SAKInputDataPreset)
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



    mTableContext.tableName = settingsGroup + QString("PresetItems");
    QString queryString = QString("CREATE TABLE '%1' (")
            .arg(mTableContext.tableName);
    queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL, ")
                       .arg(mTableContext.columns.id));
    queryString.append(QString("%1 INTEGER NOT NULL, ")
                       .arg(mTableContext.columns.format));
    queryString.append(QString("%1 TEXT NOT NULL, ")
                       .arg(mTableContext.columns.description));
    queryString.append(QString("%1 TEXT NOT NULL)")
                       .arg(mTableContext.columns.text));
    // Try to create table
    mSqlQuery = QSqlQuery(*sqlDatabase);
    if (!sqlDatabase->tables().contains(mTableContext.tableName)) {
        if (!mSqlQuery.exec(queryString)) {
            QString errorString = mSqlQuery.lastError().text();
            outputMessage(errorString, true);
            qWarning() << "Create table("
                       << mTableContext.tableName
                       << ")failed:"
                       << errorString;
            qInfo() << queryString;
        }
    }
    readinRecord();
}

SAKInputDataPreset::~SAKInputDataPreset()
{
    delete mUi;
}

void SAKInputDataPreset::readinRecord()
{
    const QString queryString = QString("SELECT * FROM %1")
            .arg(mTableContext.tableName);
    if (mSqlQuery.exec(queryString)) {
        while (mSqlQuery.next()) {
            SAKInputDataPresetItem::SAKStructDataPresetItemContext itemContext;
            QVariant tempVariant = mSqlQuery.value(mTableContext.columns.id);
            itemContext.id = tempVariant.toULongLong();

            tempVariant = mSqlQuery.value(mTableContext.columns.format);
            itemContext.format = tempVariant.toUInt();

            tempVariant = mSqlQuery.value(mTableContext.columns.description);
            itemContext.description = tempVariant.toString();

            tempVariant = mSqlQuery.value(mTableContext.columns.text);
            itemContext.text = tempVariant.toString();

            QListWidgetItem *item = new QListWidgetItem();
            auto *itemWidget = new SAKInputDataPresetItem(itemContext);
            setItemWidget(item, itemWidget);
        }
    } else {
        qWarning() << "Can not exec query command:"
                   << qPrintable(queryString);
    }
}

void SAKInputDataPreset::outputMessage(QString msg, bool isError)
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

void SAKInputDataPreset::updateFormat(quint64 id, int format)
{
    SAKDebugger::commonSqlApiUpdateRecord(&mSqlQuery,
                                          mTableContext.tableName,
                                          mTableContext.columns.format,
                                          QVariant::fromValue(format),
                                          id,
                                          true);
}

void SAKInputDataPreset::updateDescription(quint64 id,
                                           const QString &description)
{
    SAKDebugger::commonSqlApiUpdateRecord(&mSqlQuery,
                                          mTableContext.tableName,
                                          mTableContext.columns.description,
                                          QVariant::fromValue(description),
                                          id,
                                          true);
}

void SAKInputDataPreset::updateText(quint64 id, const QString &text)
{
    SAKDebugger::commonSqlApiUpdateRecord(&mSqlQuery,
                                          mTableContext.tableName,
                                          mTableContext.columns.text,
                                          QVariant::fromValue(text),
                                          id,
                                          true);
}

void SAKInputDataPreset::insertRecord(const QString &tableName,
                                      SAKInputDataPresetItem *itemWidget)
{
    SAKStructDataPresetItemTableContext tableCtx;
    QString queryString = QString("INSERT INTO %1(%2,%3,%4,%5)"
                                  " VALUES(%6,%7,'%8','%9')")
            .arg(tableName)
            .arg(tableCtx.columns.id)
            .arg(tableCtx.columns.format)
            .arg(tableCtx.columns.description)
            .arg(tableCtx.columns.text)
            .arg(itemWidget->itemID())
            .arg(itemWidget->itemTextFromat())
            .arg(itemWidget->itemDescription())
            .arg(itemWidget->itemText());
    if (!mSqlQuery.exec(queryString)) {
#if 0
        qWarning() << "Insert record to("
                   << tableName
                   << ") table failed: "
                   << mSqlQuery.lastError().text();
        qInfo() << queryString;
#endif
    }
}

void SAKInputDataPreset::setItemWidget(QListWidgetItem *item,
                                       SAKInputDataPresetItem *itemWidget)
{
    bool contain = false;
    for (int i = 0; i < mListWidget->count(); i++) {
        QListWidgetItem *item = mListWidget->item(i);
        QWidget *w = mListWidget->itemWidget(item);
        auto *iw = qobject_cast<SAKInputDataPresetItem*>(w);
        if (iw->itemID() == itemWidget->itemID()) {
            contain = true;
            break;
        }
    }

    if (!contain) {
        item->setSizeHint(itemWidget->sizeHint());
        mListWidget->addItem(item);
        mListWidget->setItemWidget(item, itemWidget);

        connect(itemWidget, &SAKInputDataPresetItem::formatChanged,
                this, &SAKInputDataPreset::updateFormat);
        connect(itemWidget, &SAKInputDataPresetItem::textChanged,
                this, &SAKInputDataPreset::updateText);
        connect(itemWidget, &SAKInputDataPresetItem::descriptionChanged,
                this, &SAKInputDataPreset::updateDescription);

        QAction *action = mItemsMenu->addAction(itemWidget->itemText(),
                                                this,
                                                [=](){
            QString rawData = itemWidget->itemText();
            int format = itemWidget->itemTextFromat();
            emit this->invokeWriteBytes(rawData, format);
        });
        // The action will be deleted after item widget is destroyed.
        connect(itemWidget, &SAKInputDataPresetItem::destroyed,
                action, &QAction::deleteLater);
        connect(itemWidget, &SAKInputDataPresetItem::descriptionChanged,
                this, [=](quint64 id, const QString text){
            Q_UNUSED(id);
            action->setText(text);
        });
    }
}

void SAKInputDataPreset::on_deletePushButton_clicked()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (item) {
        QWidget *itemWidget = mListWidget->itemWidget(item);
        auto *iw = reinterpret_cast<SAKInputDataPresetItem*>(itemWidget);
        quint64 id = iw->itemID();
        QString tableName = mTableContext.tableName;

        // Delete record from database.
        mListWidget->removeItemWidget(item);
        delete item;
        SAKDebugger::commonSqlApiDeleteRecord(&mSqlQuery,
                                              mTableContext.tableName,
                                              id);
    } else {
        outputMessage(tr("Plese select an item first."));
    }
}

void SAKInputDataPreset::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem();
    SAKInputDataPresetItem *itemWidget = new SAKInputDataPresetItem();
    setItemWidget(item, itemWidget);
    insertRecord(mTableContext.tableName, itemWidget);
}

void SAKInputDataPreset::on_outportPushButton_clicked()
{
    QJsonArray jsonArray;
    for (int i = 0; i < mListWidget->count(); i++) {
        auto *itemWidget = mListWidget->itemWidget(mListWidget->item(i));
        auto *iw = qobject_cast<SAKInputDataPresetItem*>(itemWidget);
        if (iw) {
            QJsonObject obj;
            QString key;
            QJsonValue value;


            key = mTableContext.columns.id;
            value = QVariant::fromValue(iw->itemID()).toJsonValue();
            obj.insert(key, value);

            key = mTableContext.columns.format;
            value = QVariant::fromValue(iw->itemTextFromat()).toJsonValue();
            obj.insert(key, value);

            key = mTableContext.columns.description;
            value = QVariant::fromValue(iw->itemDescription()).toJsonValue();
            obj.insert(key, value);

            key = mTableContext.columns.text;
            value = QVariant::fromValue(iw->itemText()).toJsonValue();
            obj.insert(key, value);


            jsonArray.append(QJsonValue(obj));
        }
    }
    QJsonDocument jsonDoc;
    jsonDoc.setArray(jsonArray);


    // Open file
    auto desktopLocation = QStandardPaths::DesktopLocation;
    QString defaultName = QStandardPaths::writableLocation(desktopLocation);
    defaultName.append(QString("/"));
    defaultName.append(QDateTime::currentDateTime()
                       .toString("yyyyMMddhhmmss"));
    defaultName.append(".json");
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Outport data"),
                                                    defaultName,
                                                    QString("json (*.json)"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if (file.open(QFile::ReadWrite)){
            file.write(jsonDoc.toJson());
            file.close();
        }else{
            outputMessage(file.errorString(), false);
        }
    }
}

void SAKInputDataPreset::on_importPushButton_clicked()
{
    auto writableLocation = QStandardPaths::DesktopLocation;
    QString defaultPath = QStandardPaths::writableLocation(writableLocation);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import data"),
                                                    defaultPath,
                                                    QString("json (*.json)"));
    QFile file(fileName);
    QByteArray jsonArray;
    if (file.open(QFile::ReadWrite)) {
        jsonArray = file.readAll();
        file.close();
    } else {
        outputMessage(file.errorString(), false);
        return;
    }

    QJsonDocument jsc = QJsonDocument::fromJson(jsonArray);
    if (!jsc.isArray()){
        outputMessage(QString("QJsonDocument is not json array"), false);
        return;
    }

    QJsonArray jsa = jsc.array();
    for (int i = 0; i < jsa.count(); i++) {
        if (!jsa.at(i).isObject()) {
            continue;
        }

        QJsonObject jso = jsa.at(i).toObject();
        SAKInputDataPresetItem::SAKStructDataPresetItemContext itemCtx;
        QVariant idVariant = jso.value(mTableContext.columns.id);
        itemCtx.id = idVariant.toULongLong();

        QVariant formatVariant = jso.value(mTableContext.columns.format);
        itemCtx.format = formatVariant.toUInt();

        QVariant desVariant = jso.value(mTableContext.columns.description);
        itemCtx.description = desVariant.toString();

        QVariant textVariant = jso.value(mTableContext.columns.text);
        itemCtx.text = textVariant.toString();

        QListWidgetItem *item = new QListWidgetItem();
        auto itemWidget = new SAKInputDataPresetItem(itemCtx);
        setItemWidget(item, itemWidget);
        insertRecord(mTableContext.tableName,
                     qobject_cast<SAKInputDataPresetItem*>(itemWidget));
    }
}
