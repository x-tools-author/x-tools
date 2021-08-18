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

SAKDebuggerPluginAutoResponse::SAKDebuggerPluginAutoResponse(
        QSettings *settings,
        QString settingsGroup,
        QSqlDatabase *sqlDatabase,
        QString tableNameSuffix,
        QWidget *parent
        )
    :SAKBaseListWidget(sqlDatabase,
                       settings,
                       settingsGroup,
                       tableNameSuffix,
                       parent)
{
    mTableCtx.tableName = mTableName;
    initialize();
}

SAKDebuggerPluginAutoResponse::~SAKDebuggerPluginAutoResponse()
{

}

QString SAKDebuggerPluginAutoResponse::sqlInsert(const QString &tableName,
                                                 QWidget *itemWidget)
{
    auto cookedItemWidget
            = qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
    if (!cookedItemWidget) {
        return QString();
    }

    auto itemCtx = cookedItemWidget->context();
    auto tableCtx = mTableCtx;
    QString queryString = QString("INSERT INTO %1(").arg(tableName);
    queryString.append(tableCtx.columns.id).append(",");
    queryString.append(tableCtx.columns.description).append(",");
    queryString.append(tableCtx.columns.referenceData).append(",");
    queryString.append(tableCtx.columns.responseData).append(",");
    queryString.append(tableCtx.columns.enable).append(",");
    queryString.append(tableCtx.columns.referenceFormat).append(",");
    queryString.append(tableCtx.columns.responseFormat).append(",");
    queryString.append(tableCtx.columns.option).append(",");
    queryString.append(tableCtx.columns.enableDelay).append(",");
    queryString.append(tableCtx.columns.delayTime).append(")");
    queryString.append("VALUES(");
    queryString.append(QString("%1,").arg(itemCtx.id));
    queryString.append(QString("'%1',").arg(itemCtx.description));
    queryString.append(QString("'%1',").arg(itemCtx.referenceData));
    queryString.append(QString("'%1',").arg(itemCtx.responseData));
    queryString.append(QString("%1,").arg(itemCtx.enable));
    queryString.append(QString("%1,").arg(itemCtx.referenceFormat));
    queryString.append(QString("%1,").arg(itemCtx.responseFormat));
    queryString.append(QString("%1,").arg(itemCtx.option));
    queryString.append(QString("%1,").arg(itemCtx.enableDelay));
    queryString.append(QString("%1)").arg(itemCtx.delayTime));

    return queryString;
}

QWidget *SAKDebuggerPluginAutoResponse::createItemFromParameters(
        const QJsonObject &jsonObj
        )
{
    if (jsonObj.isEmpty()) {
        return new SAKDebuggerPluginAutoResponseItem;
    }

    SAKDebuggerPluginAutoResponseItem::SAKStructItemContext itemCtx;
    itemCtx.id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
    itemCtx.description = jsonObj.value(mTableCtx.columns.description).toString();
    itemCtx.referenceData = jsonObj.value(mTableCtx.columns.referenceData).toString();
    itemCtx.responseData = jsonObj.value(mTableCtx.columns.responseData).toString();
    itemCtx.enable = jsonObj.value(mTableCtx.columns.enable).toBool();
    itemCtx.referenceFormat = jsonObj.value(mTableCtx.columns.referenceFormat).toInt();
    itemCtx.responseFormat = jsonObj.value(mTableCtx.columns.responseFormat).toInt();
    itemCtx.option = jsonObj.value(mTableCtx.columns.option).toInt();
    itemCtx.enableDelay = jsonObj.value(mTableCtx.columns.enableDelay).toBool();
    itemCtx.delayTime = jsonObj.value(mTableCtx.columns.delayTime).toInt();
    return new SAKDebuggerPluginAutoResponseItem(itemCtx);
}

QJsonObject SAKDebuggerPluginAutoResponse::toJsonObject(QWidget *itemWidget)
{
    auto cookedItemWidget =
            qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
    auto itemCtx = cookedItemWidget->context();
    QJsonObject jsonObj;
    jsonObj.insert(mTableCtx.columns.id, qint64(itemCtx.id));
    jsonObj.insert(mTableCtx.columns.description, itemCtx.description);
    jsonObj.insert(mTableCtx.columns.referenceData, itemCtx.referenceData);
    jsonObj.insert(mTableCtx.columns.responseData, itemCtx.responseData);
    jsonObj.insert(mTableCtx.columns.enable, itemCtx.enable);
    jsonObj.insert(mTableCtx.columns.referenceFormat, itemCtx.referenceFormat);
    jsonObj.insert(mTableCtx.columns.responseFormat, itemCtx.responseFormat);
    jsonObj.insert(mTableCtx.columns.option, itemCtx.option);
    jsonObj.insert(mTableCtx.columns.enableDelay, itemCtx.enableDelay);
    jsonObj.insert(mTableCtx.columns.delayTime, qint64(itemCtx.delayTime));
    jsonObj.insert(mTableCtx.columns.id, qint64(itemCtx.id));

    return jsonObj;
}

QJsonObject SAKDebuggerPluginAutoResponse::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    QString column;
    QVariant valueVariant;

    column = mTableCtx.columns.id;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toLongLong());

    column = mTableCtx.columns.description;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.referenceData;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.responseData;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.enable;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toBool());

    column = mTableCtx.columns.referenceFormat;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.responseFormat;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.option;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.enableDelay;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toBool());

    column = mTableCtx.columns.delayTime;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    return jsonObj;
}

quint64 SAKDebuggerPluginAutoResponse::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget =
            qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
    return cookedItemWidget->context().id;
}

void SAKDebuggerPluginAutoResponse::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget =
            qobject_cast<SAKDebuggerPluginAutoResponseItem*>(itemWidget);
    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::descriptionChanged,
            this,
            [&](quint64 id, const QString &description){
        updateRecord(id,
                     mTableCtx.columns.description,
                     QVariant::fromValue(description));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::referenceTextChanged,
            this,
            [&](quint64 id, const QString &text){
        updateRecord(id,
                     mTableCtx.columns.referenceData,
                     QVariant::fromValue(text));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::responseTextChanged,
            this,
            [&](quint64 id, const QString &text){
        updateRecord(id,
                     mTableCtx.columns.responseData,
                     QVariant::fromValue(text));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::optionChanged,
            this,
            [&](quint64 id, int option){
        updateRecord(id,
                     mTableCtx.columns.option,
                     QVariant::fromValue(option));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::referenceFormatChanged,
            this,
            [&](quint64 id, int format){
        updateRecord(id,
                     mTableCtx.columns.referenceFormat,
                     QVariant::fromValue(format));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::responseFromatChanged,
            this,
            [&](quint64 id, int format){
        updateRecord(id,
                     mTableCtx.columns.responseFormat,
                     QVariant::fromValue(format));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::enableDelayChanged,
            this,
            [&](quint64 id, bool delay){
        updateRecord(id,
                     mTableCtx.columns.enableDelay,
                     QVariant::fromValue(delay));
    });

    connect(cookedItemWidget,
            &SAKDebuggerPluginAutoResponseItem::delayTimeChanged,
            this,
            [&](quint64 id, int interval){
        updateRecord(id,
                     mTableCtx.columns.delayTime,
                     QVariant::fromValue(interval));
    });
}

QString SAKDebuggerPluginAutoResponse::sqlCreate(const QString &tableName)
{
    QString queryString;
    queryString.append(QString("CREATE TABLE %1(")
                       .arg(tableName));
    queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                       .arg(mTableCtx.columns.id));

    queryString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.description));

    queryString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.referenceData));

    queryString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.responseData));

    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.enable));

    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.referenceFormat));

    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.responseFormat));

    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.option));

    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.enableDelay));

    queryString.append(QString("%1 INTEGER NOT NULL)")
                       .arg(mTableCtx.columns.delayTime));
    return queryString;
}
