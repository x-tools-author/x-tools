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
#include <QTime>
#include <QDebug>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QSqlError>
#include <QJsonArray>
#include <QListWidget>
#include <QPushButton>
#include <QJsonObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QStandardPaths>

#include "SAKDebugger.hh"
#include "SAKDebuggerInputDataPreset.hh"
#include "SAKDebuggerInputDataPresetItem.hh"

SAKDebuggerInputDataPreset::SAKDebuggerInputDataPreset(
        QSqlDatabase *sqlDatabase,
        QSettings *settings,
        QString settingsGroup,
        QMenu *itemsMenu,
        QWidget *parent)
    :SAKBaseListWidget(sqlDatabase,
                       settings,
                       settingsGroup,
                       QString("PresetData"),
                       parent)
    ,mItemsMenu(itemsMenu)
{
    mTableContext.tableName = settingsGroup + QString("PresetData");
    initialize();
}

void SAKDebuggerInputDataPreset::updateFormat(quint64 id, int format)
{
    updateRecord(id, mTableContext.columns.format, QVariant::fromValue(format));
}

void SAKDebuggerInputDataPreset::updateDescription(quint64 id,
                                                   const QString &description)
{
    updateRecord(id,
                 mTableContext.columns.description,
                 QVariant::fromValue(description));
}

void SAKDebuggerInputDataPreset::updateText(quint64 id, const QString &text)
{
    updateRecord(id, mTableContext.columns.text, QVariant::fromValue(text));
}

void SAKDebuggerInputDataPreset::insertRecord(const QString &tableName,
                                              QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    if (itemWidget) {
        SAKStructDataPresetItemTableContext tableCtx;
        QString queryString = QString("INSERT INTO %1(%2,%3,%4,%5)"
                                      " VALUES(%6,%7,'%8','%9')")
                .arg(tableName,
                     tableCtx.columns.id,
                     tableCtx.columns.format,
                     tableCtx.columns.description,
                     tableCtx.columns.text,
                     QString::number(cookedItemWidget->itemID()),
                     QString::number(cookedItemWidget->itemTextFromat()),
                     cookedItemWidget->itemDescription(),
                     cookedItemWidget->itemText());
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
}

QWidget *SAKDebuggerInputDataPreset::createItemFromParameters(
        const QJsonObject &jsonObj
        )
{
    if (jsonObj.isEmpty()) {
        return new SAKDebuggerInputDataPresetItem();
    } else {
        SAKDebuggerInputDataPresetItem::SAKStructDataPresetItemContext itemCtx;
        QVariant idVariant = jsonObj.value(mTableContext.columns.id);
        itemCtx.id = idVariant.toULongLong();

        QVariant formatVariant = jsonObj.value(mTableContext.columns.format);
        itemCtx.format = formatVariant.toUInt();

        QVariant desVariant = jsonObj.value(mTableContext.columns.description);
        itemCtx.description = desVariant.toString();

        QVariant textVariant = jsonObj.value(mTableContext.columns.text);
        itemCtx.text = textVariant.toString();

        auto itemWidget = new SAKDebuggerInputDataPresetItem(itemCtx);
        return itemWidget;
    }
}

QJsonObject SAKDebuggerInputDataPreset::toJsonObject(QWidget *itemWidget)
{
    QJsonObject obj;
    QString key;
    QJsonValue value;

    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    if (cookedItemWidget) {
        key = mTableContext.columns.id;
        value = QVariant::fromValue(cookedItemWidget->itemID()).toJsonValue();
        obj.insert(key, value);

        key = mTableContext.columns.format;
        value = QVariant::fromValue(cookedItemWidget->itemTextFromat()).toJsonValue();
        obj.insert(key, value);

        key = mTableContext.columns.description;
        value = QVariant::fromValue(cookedItemWidget->itemDescription()).toJsonValue();
        obj.insert(key, value);

        key = mTableContext.columns.text;
        value = QVariant::fromValue(cookedItemWidget->itemText()).toJsonValue();
        obj.insert(key, value);
    }

    return obj;
}

QJsonObject SAKDebuggerInputDataPreset::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    jsonObj.insert(mTableContext.columns.id,
                   mSqlQuery.value(mTableContext.columns.id).toLongLong());
    jsonObj.insert(mTableContext.columns.format,
                   mSqlQuery.value(mTableContext.columns.format).toInt());
    jsonObj.insert(mTableContext.columns.description,
                   mSqlQuery.value(mTableContext.columns.description).toString());
    jsonObj.insert(mTableContext.columns.text,
                   mSqlQuery.value(mTableContext.columns.text).toString());
    return jsonObj;
}

quint64 SAKDebuggerInputDataPreset::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->itemID();
    }

    return 0;
}

void SAKDebuggerInputDataPreset::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::formatChanged,
            this,
            &SAKDebuggerInputDataPreset::updateFormat);
    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::textChanged,
            this,
            &SAKDebuggerInputDataPreset::updateText);
    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::descriptionChanged,
            this,
            &SAKDebuggerInputDataPreset::updateDescription);

    QAction *action = mItemsMenu->addAction(cookedItemWidget->itemDescription(),
                                            this,
                                            [=](){
        QString rawData = cookedItemWidget->itemText();
        int format = cookedItemWidget->itemTextFromat();
        emit this->invokeWriteBytes(rawData, format);
    });

    // The action will be deleted after item widget is destroyed.
    connect(cookedItemWidget, &SAKDebuggerInputDataPresetItem::destroyed,
            action, &QAction::deleteLater);
    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::descriptionChanged,
            this,
            [=](quint64 id, const QString text){
        Q_UNUSED(id);
        action->setText(text);
    });
}

void SAKDebuggerInputDataPreset::createDatabaseTable(QString tableName)
{
    QString queryString = QString("CREATE TABLE '%1' (").arg(tableName);
    queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL, ")
                       .arg(mTableContext.columns.id));
    queryString.append(QString("%1 INTEGER NOT NULL, ")
                       .arg(mTableContext.columns.format));
    queryString.append(QString("%1 TEXT NOT NULL, ")
                       .arg(mTableContext.columns.description));
    queryString.append(QString("%1 TEXT NOT NULL)")
                       .arg(mTableContext.columns.text));
    // Try to create table
    if (!mSqlQuery.exec(queryString)) {
        QString errorString = mSqlQuery.lastError().text();
        //outputMessage(errorString, true);
        qWarning() << "Create table("
                   << tableName
                   << ")failed:"
                   << errorString;
        qInfo() << queryString;
    }
}
