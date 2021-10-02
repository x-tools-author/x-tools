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
    mTableContext.tableName = mTableName;
    initialize();

    setContentsMargins(4, 4, 4, 4);
}

void SAKDebuggerInputDataPreset::onDescriptionChanged(quint64 id,
                                                   const QString &description)
{
    updateRecord(id,
                 mTableContext.columns.description,
                 QVariant::fromValue(description));
}

void SAKDebuggerInputDataPreset::onFormatChanged(quint64 id, int format)
{
    updateRecord(id, mTableContext.columns.format, QVariant::fromValue(format));
}

void SAKDebuggerInputDataPreset::onDataChanged(quint64 id, const QString &text)
{
    updateRecord(id, mTableContext.columns.data, QVariant::fromValue(text));
}

QString SAKDebuggerInputDataPreset::sqlCreate(const QString &tableName)
{
    QString queryString = QString("CREATE TABLE '%1' (").arg(tableName);
    queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL, ")
                       .arg(mTableContext.columns.id));
    queryString.append(QString("%1 TEXT NOT NULL, ")
                       .arg(mTableContext.columns.description));
    queryString.append(QString("%1 INTEGER NOT NULL, ")
                       .arg(mTableContext.columns.format));
    queryString.append(QString("%1 TEXT NOT NULL)")
                       .arg(mTableContext.columns.data));
    return queryString;
}


QString SAKDebuggerInputDataPreset::sqlInsert(const QString &tableName,
                                              QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    if (cookedItemWidget) {
        auto ctx = cookedItemWidget->context();
        SAKStructTableContext tableCtx;
        QString queryString = QString("INSERT INTO %1(%2,%3,%4,%5)"
                                      " VALUES(%6,%7,'%8','%9')")
                .arg(tableName,
                     tableCtx.columns.id,
                     tableCtx.columns.format,
                     tableCtx.columns.description,
                     tableCtx.columns.data,
                     QString::number(cookedItemWidget->id()),
                     QString::number(ctx.format),
                     ctx.description,
                     ctx.data);
        return queryString;
    }

    return QString();
}

QWidget *SAKDebuggerInputDataPreset::createItemFromParameters(
        const QJsonObject &jsonObj
        )
{
    if (jsonObj.isEmpty()) {
        return new SAKDebuggerInputDataPresetItem();
    } else {
        SAKDebuggerInputDataPresetItem::SAKStructItemContext itemCtx;
        QVariant idVariant = jsonObj.value(mTableContext.columns.id);
        itemCtx.id = idVariant.toULongLong();

        QVariant formatVariant = jsonObj.value(mTableContext.columns.format);
        itemCtx.format = formatVariant.toUInt();

        QVariant desVariant = jsonObj.value(mTableContext.columns.description);
        itemCtx.description = desVariant.toString();

        QVariant textVariant = jsonObj.value(mTableContext.columns.data);
        itemCtx.data = textVariant.toString();

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
        auto ctx = cookedItemWidget->context();
        key = mTableContext.columns.id;
        value = QVariant::fromValue(cookedItemWidget->id()).toJsonValue();
        obj.insert(key, value);

        key = mTableContext.columns.format;
        value = QVariant::fromValue(ctx.format).toJsonValue();
        obj.insert(key, value);

        key = mTableContext.columns.description;
        value = QVariant::fromValue(ctx.description).toJsonValue();
        obj.insert(key, value);

        key = mTableContext.columns.data;
        value = QVariant::fromValue(ctx.data).toJsonValue();
        obj.insert(key, value);
    }

    return obj;
}

QJsonObject SAKDebuggerInputDataPreset::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    jsonObj.insert(mTableContext.columns.id,
                   sqlQuery.value(mTableContext.columns.id).toLongLong());
    jsonObj.insert(mTableContext.columns.format,
                   sqlQuery.value(mTableContext.columns.format).toInt());
    jsonObj.insert(mTableContext.columns.description,
                   sqlQuery.value(mTableContext.columns.description).toString());
    jsonObj.insert(mTableContext.columns.data,
                   sqlQuery.value(mTableContext.columns.data).toString());
    return jsonObj;
}

quint64 SAKDebuggerInputDataPreset::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->id();
    }

    return 0;
}

void SAKDebuggerInputDataPreset::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKDebuggerInputDataPresetItem*>(itemWidget);
    if (!cookedItemWidget) {
        return;
    }

    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::formatChanged,
            this,
            &SAKDebuggerInputDataPreset::onFormatChanged);
    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::dataChanged,
            this,
            &SAKDebuggerInputDataPreset::onDataChanged);
    connect(cookedItemWidget,
            &SAKDebuggerInputDataPresetItem::descriptionChanged,
            this,
            &SAKDebuggerInputDataPreset::onDescriptionChanged);


    auto ctx = cookedItemWidget->context();
    QAction *action = mItemsMenu->addAction(ctx.description, this, [=](){
        QString rawData = ctx.data;
        int format = ctx.format;
        emit this->invokeWriteString(rawData, format);
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
