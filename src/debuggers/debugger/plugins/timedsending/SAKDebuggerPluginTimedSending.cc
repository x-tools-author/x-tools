/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>

#include "SAKDebugger.hh"
#include "SAKApplication.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginTimedSending.hh"
#include "SAKDebuggerPluginTimedSendingItem.hh"

SAKDebuggerPluginTimedSending::SAKDebuggerPluginTimedSending(
        QSqlDatabase *sqlDatabase,
        QSettings *settings,
        QString settingsGroup,
        QString tableNameSuffix,
        QWidget *parent)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix, parent)
{
    mTableCtx.tableName = mTableName;
    initialize();
}

SAKDebuggerPluginTimedSending::~SAKDebuggerPluginTimedSending()
{

}

QString SAKDebuggerPluginTimedSending::sqlInsert(const QString &tableName,
                                                     QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    if (cookedItemWidget) {
        QString queryString = QString("INSERT INTO %1").arg(tableName);
        queryString.append(QString("(%1,%2,%3,%4,%5,%6,%7) VALUES(")
                           .arg(mTableCtx.columns.id,
                                mTableCtx.columns.enable,
                                mTableCtx.columns.description,
                                mTableCtx.columns.interval,
                                mTableCtx.columns.format,
                                mTableCtx.columns.suffix,
                                mTableCtx.columns.data));
        auto ctx = cookedItemWidget->context();
        queryString.append(QString("%1,").arg(cookedItemWidget->id()));
        queryString.append(QString("%1,").arg(ctx.enable));
        queryString.append(QString("'%1',").arg(ctx.description));
        queryString.append(QString("%1,").arg(ctx.interval));
        queryString.append(QString("%1,").arg(ctx.format));
        queryString.append(QString("%1,").arg(ctx.suffix));
        queryString.append(QString("'%1')").arg(ctx.data));
        return queryString;
    }

    return QString();
}

QString SAKDebuggerPluginTimedSending::sqlCreate(const QString &tableName)
{
    QString queryString = QString("CREATE TABLE %1(")
            .arg(tableName);
    queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                       .arg(mTableCtx.columns.id));
    queryString.append(QString("%1 BOOL NOT NULL,")
                       .arg(mTableCtx.columns.enable));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.description));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.interval));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.format));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.suffix));
    queryString.append(QString("%1 TEXT NOT NULL)")
                       .arg(mTableCtx.columns.data));

    return queryString;
}

QWidget *SAKDebuggerPluginTimedSending::createItemFromParameters(
        const QJsonObject &jsonObj
        )
{
    if (jsonObj.isEmpty()) {
        return new SAKDebuggerPluginTimedSendingItem();
    } else {
        SAKDebuggerPluginTimedSendingItem::SAKStructItemContext ctx;
        ctx.id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
        ctx.enable = jsonObj.value(mTableCtx.columns.enable).toBool();
        ctx.description = jsonObj.value(mTableCtx.columns.description).toString();
        ctx.interval = jsonObj.value(mTableCtx.columns.interval).toInt();
        ctx.format = jsonObj.value(mTableCtx.columns.format).toInt();
        ctx.suffix = jsonObj.value(mTableCtx.columns.suffix).toInt();
        ctx.data = jsonObj.value(mTableCtx.columns.data).toString();


        auto itemWidget = new SAKDebuggerPluginTimedSendingItem(ctx);
        return itemWidget;
    }
}

QJsonObject SAKDebuggerPluginTimedSending::toJsonObject(QWidget *itemWidget)
{
    QJsonObject jsonObj;
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    if (cookedItemWidget) {
        QString key;
        QJsonValue value;
        auto ctx = cookedItemWidget->context();

        key = mTableCtx.columns.id;
        value = QJsonValue(qint64(cookedItemWidget->id()));
        jsonObj.insert(key, value);

        key = mTableCtx.columns.enable;
        value = QJsonValue(ctx.enable);
        jsonObj.insert(key, value);

        key = mTableCtx.columns.description;
        value = QJsonValue(ctx.description);
        jsonObj.insert(key, value);

        key = mTableCtx.columns.interval;
        value = QJsonValue(ctx.interval);
        jsonObj.insert(key, value);

        key = mTableCtx.columns.format;
        value = QJsonValue(ctx.format);
        jsonObj.insert(key, value);

        key = mTableCtx.columns.suffix;
        value = QJsonValue(ctx.suffix);
        jsonObj.insert(key, value);

        key = mTableCtx.columns.data;
        value = QJsonValue(ctx.data);
        jsonObj.insert(key, value);
    }

    return jsonObj;
}

QJsonObject SAKDebuggerPluginTimedSending::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject parameters;
    parameters.insert(mTableCtx.columns.id,
                      sqlQuery.value(mTableCtx.columns.id).toLongLong());
    parameters.insert(mTableCtx.columns.interval,
                      sqlQuery.value(mTableCtx.columns.interval).toInt());
    parameters.insert(mTableCtx.columns.format,
                      sqlQuery.value(mTableCtx.columns.format).toInt());
    parameters.insert(mTableCtx.columns.description,
                      sqlQuery.value(mTableCtx.columns.description).toString());
    parameters.insert(mTableCtx.columns.data,
                      sqlQuery.value(mTableCtx.columns.data).toString());

    return parameters;
}

quint64 SAKDebuggerPluginTimedSending::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->id();
    } else {
        return 0;
    }
}

void SAKDebuggerPluginTimedSending::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);

    connect(cookedItemWidget,
            &SAKDebuggerPluginTimedSendingItem::enableChanged,
            this,
            &SAKDebuggerPluginTimedSending::onEnableChanged);

    connect(cookedItemWidget,
            &SAKDebuggerPluginTimedSendingItem::descriptionChanged,
            this,
            &SAKDebuggerPluginTimedSending::onDescriptionChanged);

    connect(cookedItemWidget,
            &SAKDebuggerPluginTimedSendingItem::intervalChanged,
            this,
            &SAKDebuggerPluginTimedSending::onIntervalChanged);

    connect(cookedItemWidget,
            &SAKDebuggerPluginTimedSendingItem::formatChanged,
            this,
            &SAKDebuggerPluginTimedSending::onFormatChanged);

    connect(cookedItemWidget,
            &SAKDebuggerPluginTimedSendingItem::suffixChanged,
            this,
            &SAKDebuggerPluginTimedSending::onSuffixChanged);

    connect(cookedItemWidget,
            &SAKDebuggerPluginTimedSendingItem::dataChanged,
            this,
            &SAKDebuggerPluginTimedSending::onDataChanged);
}

void SAKDebuggerPluginTimedSending::onEnableChanged(quint64 id, bool enable)
{
    updateRecord(id, mTableCtx.columns.enable, enable);
}

void SAKDebuggerPluginTimedSending::onDescriptionChanged(quint64 id, QString description)
{
    updateRecord(id, mTableCtx.columns.description, description);
}

void SAKDebuggerPluginTimedSending::onIntervalChanged(quint64 id, int interval)
{
    updateRecord(id, mTableCtx.columns.interval, interval);
}

void SAKDebuggerPluginTimedSending::onFormatChanged(quint64 id, int format)
{
    updateRecord(id, mTableCtx.columns.format, format);
}

void SAKDebuggerPluginTimedSending::onSuffixChanged(quint64 id, int format)
{
    updateRecord(id, mTableCtx.columns.suffix, format);
}

void SAKDebuggerPluginTimedSending::onDataChanged(quint64 id, QString text)
{
    updateRecord(id, mTableCtx.columns.data, text);
}
