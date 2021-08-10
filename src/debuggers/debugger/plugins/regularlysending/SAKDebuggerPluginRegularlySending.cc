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
#include "SAKDebuggerPluginRegularlySending.hh"
#include "SAKDebuggerPluginRegularlySendingItem.hh"

SAKDebuggerPluginRegularlySending::SAKDebuggerPluginRegularlySending(
        QSqlDatabase *sqlDatabase,
        QSettings *settings,
        QString settingsGroup,
        QString tableNameSuffix,
        QWidget *parent)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix, parent)
{
    mTableCtx.tableName = mTableName;


    if (!mSqlDatabase->tables().contains(mTableName)) {
        createSqlDatabaseTable();
    }
    readinRecord();
}

SAKDebuggerPluginRegularlySending::~SAKDebuggerPluginRegularlySending()
{

}

void SAKDebuggerPluginRegularlySending::insertRecord(const QString &tableName,
                                                     QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    QString queryString = QString("INSERT INTO %1").arg(tableName);
    queryString.append(QString("(%1,%2,%3,%4,%5) VALUES(")
                       .arg(mTableCtx.columns.id,
                            mTableCtx.columns.interval,
                            mTableCtx.columns.format,
                            mTableCtx.columns.description,
                            mTableCtx.columns.data));
    queryString.append(QString("%1,").arg(cookedItemWidget->itemID()));
    queryString.append(QString("%1,").arg(cookedItemWidget->itemInterval()));
    queryString.append(QString("%1,").arg(cookedItemWidget->itemFormat()));
    queryString.append(QString("'%1',").arg(cookedItemWidget->itemDescription()));
    queryString.append(QString("'%1')").arg(cookedItemWidget->itemText()));
    if (!mSqlQuery.exec(queryString)) {
        qInfo() << queryString;
        qWarning() << "Insert record to " << mTableCtx.tableName
                   << " table failed: " << mSqlQuery.lastError().text();
    }
}

QWidget *SAKDebuggerPluginRegularlySending::createItemFromParameters(
        const QJsonObject &jsonObj
        )
{
    if (jsonObj.isEmpty()) {
        return new SAKDebuggerPluginRegularlySendingItem();
    } else {
        SAKDebuggerPluginRegularlySendingItem::SAKStructItemContext ctx;
        ctx.id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
        ctx.data = jsonObj.value(mTableCtx.columns.data).toString();
        ctx.format = jsonObj.value(mTableCtx.columns.data).toInt();
        ctx.interval = jsonObj.value(mTableCtx.columns.data).toInt();
        ctx.description = jsonObj.value(mTableCtx.columns.data).toString();

        auto itemWidget = new SAKDebuggerPluginRegularlySendingItem(ctx);
        return itemWidget;
    }
}

QJsonObject SAKDebuggerPluginRegularlySending::toJsonObject(QWidget *itemWidget)
{
    QJsonObject jsonObj;
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    if (cookedItemWidget) {
        QString key;
        QJsonValue value;

        key = mTableCtx.columns.id;
        value = QJsonValue(qint64(cookedItemWidget->itemID()));
        jsonObj.insert(key, value);

        key = mTableCtx.columns.data;
        value = QJsonValue(cookedItemWidget->itemText());
        jsonObj.insert(key, value);

        key = mTableCtx.columns.format;
        value = QJsonValue(int(cookedItemWidget->itemFormat()));
        jsonObj.insert(key, value);

        key = mTableCtx.columns.interval;
        value = QJsonValue(int(cookedItemWidget->itemInterval()));
        jsonObj.insert(key, value);

        key = mTableCtx.columns.description;
        value = QJsonValue(cookedItemWidget->itemDescription());
        jsonObj.insert(key, value);
    }

    return jsonObj;
}

quint64 SAKDebuggerPluginRegularlySending::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->itemID();
    } else {
        return 0;
    }
}

void SAKDebuggerPluginRegularlySending::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SendingItem*>(itemWidget);
    connect(cookedItemWidget,
            &SAKDebuggerPluginRegularlySendingItem::intervalChanged,
            this,
            &SAKDebuggerPluginRegularlySending::changeInterval);
    connect(cookedItemWidget,
            &SAKDebuggerPluginRegularlySendingItem::formatChanged,
            this,
            &SAKDebuggerPluginRegularlySending::changeFormat);

    connect(cookedItemWidget,
            &SAKDebuggerPluginRegularlySendingItem::descriptionChanged,
            this,
            &SAKDebuggerPluginRegularlySending::changeDescription);

    connect(cookedItemWidget,
            &SAKDebuggerPluginRegularlySendingItem::inputTextChanged,
            this,
            &SAKDebuggerPluginRegularlySending::changeInputText);
#if 0
    connect(cookedItemWidget,
            &SAKDebuggerPluginRegularlySendingItem::invokeWriteBytes,
            this,
            &SAKDebuggerPluginRegularlySending::invokeWriteBytes);
#endif
}

void SAKDebuggerPluginRegularlySending::readinRecord()
{
    if (mSqlQuery.exec(QString("SELECT * FROM %1").arg(mTableName))) {
        SendingItem::SAKStructItemContext itemCtx;
        while (mSqlQuery.next()) {
            itemCtx.id = mSqlQuery.value(mTableCtx.columns.id).toULongLong();
            itemCtx.interval = mSqlQuery.value(mTableCtx.columns.interval).toUInt();
            itemCtx.format = mSqlQuery.value(mTableCtx.columns.format).toUInt();
            itemCtx.description =
                    mSqlQuery.value(mTableCtx.columns.description).toString();
            itemCtx.data = mSqlQuery.value(mTableCtx.columns.data).toString();

            auto item = new QListWidgetItem(mListWidget);
            auto itemWidget = new SAKDebuggerPluginRegularlySendingItem(itemCtx);
            setupItemWidget(item, itemWidget);
        }
    }else{
        qWarning() << "Select record form " << mTableName
                   << " table failed: " << mSqlQuery.lastError().text();
    }
}

void SAKDebuggerPluginRegularlySending::createSqlDatabaseTable()
{
    QString queryString = QString("CREATE TABLE %1(")
            .arg(mTableName);
    queryString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                       .arg(mTableCtx.columns.id));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.interval));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.format));
    queryString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.description));
    queryString.append(QString("%1 INTEGER NOT NULL)")
                       .arg(mTableCtx.columns.data));

    if (!mSqlQuery.exec(queryString)) {
        qInfo() << queryString;
        qWarning() << QString("Carete table(%1) failed:%2")
                      .arg(mTableName, mSqlQuery.lastError().text());
    }
}

void SAKDebuggerPluginRegularlySending::changeInterval(quint64 id, int interval)
{
    updateRecord(id, mTableCtx.columns.interval, interval);
}

void SAKDebuggerPluginRegularlySending::changeFormat(quint64 id, int format)
{
    updateRecord(id, mTableCtx.columns.format, format);
}

void SAKDebuggerPluginRegularlySending::changeDescription(quint64 id, QString description)
{
    updateRecord(id, mTableCtx.columns.description, description);
}

void SAKDebuggerPluginRegularlySending::changeInputText(quint64 id, QString text)
{
    updateRecord(id, mTableCtx.columns.data, text);
}
