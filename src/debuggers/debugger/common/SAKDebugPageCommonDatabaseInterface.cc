/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QMetaEnum>
#include <QSqlError>
#include <QDateTime>
#include <QCoreApplication>

#include "SAKDebugPage.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

SAKDebugPageCommonDatabaseInterface::SAKDebugPageCommonDatabaseInterface(SAKDebugPage *debugPage, QSqlDatabase *sqlDatabase, QObject *parent)
    :QObject(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mDebugPage(debugPage)
{
    mSqlQuery = QSqlQuery(*mSqlDatabase);
    if (mSqlDatabase->isOpen()){
        mTableNameAutoResponseTable = mDebugPage->tableNameAutoResponseTable();
        mTableNamePresettingDataTable = mDebugPage->tableNamePresettingDataTable();
        mTableNameTimingSendingTable = mDebugPage->tableNameTimingSendingTable();
        createAutoResponseTable(mTableNameAutoResponseTable);
        createPresettingDataTable(mTableNamePresettingDataTable);
        createTimingSendingTable(mTableNameTimingSendingTable);
    }else{
        qWarning() << "You should open the data base at first, or data base read-write function is no effective!";
    }
}

SAKDebugPageCommonDatabaseInterface::~SAKDebugPageCommonDatabaseInterface()
{

}

void SAKDebugPageCommonDatabaseInterface::insertAutoResponseItem(SAKStructAutoResponseItem item)
{
    AutoResponseTable table;
    table.tableName = mTableNameAutoResponseTable;
    bool ret = mSqlQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6,%7,%8,%9,%10,%11) VALUES(%12,'%13','%14','%15',%16,%17,%18,%19,%20,'%21')")
                              .arg(table.tableName)
                              .arg(table.columns.id)
                              .arg(table.columns.description)
                              .arg(table.columns.referenceText)
                              .arg(table.columns.responseText)
                              .arg(table.columns.enable)
                              .arg(table.columns.referenceFormat)
                              .arg(table.columns.responseFormat)
                              .arg(table.columns.option)
                              .arg(table.columns.delay)
                              .arg(table.columns.interval)
                              .arg(item.id)
                              .arg(item.name)
                              .arg(item.referenceData)
                              .arg(item.responseData)
                              .arg(item.enable)
                              .arg(item.referenceFormat)
                              .arg(item.responseFormat)
                              .arg(item.option)
                              .arg(item.delay)
                              .arg(item.interval));
    if (!ret){
        qWarning() << "Insert record to " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

QList<SAKDebugPageCommonDatabaseInterface::SAKStructAutoResponseItem> SAKDebugPageCommonDatabaseInterface::selectAutoResponseItem()
{
    AutoResponseTable table;
    table.tableName = mTableNameAutoResponseTable;
    bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKStructAutoResponseItem> itemList;
    if (ret){
        SAKStructAutoResponseItem item;
        while (mSqlQuery.next()) {
            item.id = mSqlQuery.value(table.columns.id).toULongLong();
            item.name = mSqlQuery.value(table.columns.description).toString();
            item.referenceData = mSqlQuery.value(table.columns.referenceText).toString();
            item.responseData = mSqlQuery.value(table.columns.responseText).toString();
            item.enable = mSqlQuery.value(table.columns.enable).toBool();
            item.referenceFormat = mSqlQuery.value(table.columns.referenceFormat).toUInt();
            item.responseFormat = mSqlQuery.value(table.columns.responseFormat).toUInt();
            item.option = mSqlQuery.value(table.columns.option).toUInt();
            item.delay = mSqlQuery.value(table.columns.delay).toBool();
            item.interval = mSqlQuery.value(table.columns.interval).toInt();

            itemList.append(item);
        }
    }else{
        qWarning() << "Select record form " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::insertTimingSentItem(SAKStructTimingSentItem item)
{
    TimingSendingTable table;
    table.tableName = mTableNameTimingSendingTable;
    bool ret = mSqlQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6) VALUES(%7,%8,%9,'%10','%11')")
                              .arg(table.tableName)
                              .arg(table.columns.id)
                              .arg(table.columns.interval)
                              .arg(table.columns.format)
                              .arg(table.columns.description)
                              .arg(table.columns.text)
                              .arg(item.id)
                              .arg(item.interval)
                              .arg(item.format)
                              .arg(item.comment)
                              .arg(item.data));
    if (!ret){
        qWarning() << "Insert record to " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

QList<SAKDebugPageCommonDatabaseInterface::SAKStructTimingSentItem> SAKDebugPageCommonDatabaseInterface::selectTimingSentItem()
{
    TimingSendingTable table;
    table.tableName = mTableNameTimingSendingTable;
    bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKStructTimingSentItem> itemList;
    if (ret){
        SAKStructTimingSentItem item;
        while (mSqlQuery.next()) {
            item.id = mSqlQuery.value(table.columns.id).toULongLong();
            item.interval = mSqlQuery.value(table.columns.interval).toUInt();
            item.format = mSqlQuery.value(table.columns.format).toUInt();
            item.comment = mSqlQuery.value(table.columns.description).toString();
            item.data = mSqlQuery.value(table.columns.text).toString();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::insertDataPresetItem(SAKStructPresettingDataItem item)
{
    DataPresetItemTable table;
    table.tableName = mTableNamePresettingDataTable;
    bool ret = mSqlQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5) VALUES(%6,%7,'%8','%9')")
                              .arg(table.tableName)
                              .arg(table.columns.id)
                              .arg(table.columns.format)
                              .arg(table.columns.description)
                              .arg(table.columns.text)
                              .arg(item.id)
                              .arg(item.format)
                              .arg(item.description)
                              .arg(item.text));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

QList<SAKDebugPageCommonDatabaseInterface::SAKStructPresettingDataItem> SAKDebugPageCommonDatabaseInterface::selectDataPresetItem()
{
    DataPresetItemTable table;
    table.tableName = mTableNamePresettingDataTable;
    bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKStructPresettingDataItem> itemList;
    if (ret){
        SAKStructPresettingDataItem item;
        while (mSqlQuery.next()) {
            item.id = mSqlQuery.value(table.columns.id).toULongLong();
            item.format = mSqlQuery.value(table.columns.format).toUInt();
            item.description = mSqlQuery.value(table.columns.description).toString();
            item.text = mSqlQuery.value(table.columns.text).toString();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::updateRecord(QString tableName, QString columnName, QVariant value, quint64 recordID, bool valueIsString)
{
    QString queryString;
    if (valueIsString){
        queryString = QString("UPDATE %1 SET %2='%3' WHERE ID=%4")
                .arg(tableName)
                .arg(columnName)
                .arg(value.toString())
                .arg(recordID);
    }else{
        queryString = QString("UPDATE %1 SET %2=%3 WHERE ID=%4")
                .arg(tableName)
                .arg(columnName)
                .arg(value.toInt())
                .arg(recordID);
    }

    if(!mSqlQuery.exec(queryString)){
        qWarning() << QString("Can not update record(%1):%2").arg(columnName).arg(mSqlQuery.lastError().text());
#ifdef QT_DEBUG
        qDebug() << queryString;
#endif
    }
}

void SAKDebugPageCommonDatabaseInterface::deleteRecord(QString tableName, quint64 recordID)
{
    bool ret = mSqlQuery.exec(QString("DELETE FROM %1 WHERE ID=%2")
                                     .arg(tableName)
                                     .arg(recordID));
    if (!ret){
        qWarning() << "Delete record form " << tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

bool SAKDebugPageCommonDatabaseInterface::isTableExist(QString tableName)
{
   bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(tableName));
   return ret;
}

void SAKDebugPageCommonDatabaseInterface::createAutoResponseTable(QString &tableName)
{
    AutoResponseTable autoResponseTable;
    autoResponseTable.tableName = tableName;
    if (!isTableExist(tableName)){
        if (!createAutoResponseTable(autoResponseTable)){
            qWarning() << QString("Carete table failed:%1").arg(mSqlQuery.lastError().text());
        };
    }
}

bool SAKDebugPageCommonDatabaseInterface::createAutoResponseTable(const AutoResponseTable &table)
{
    bool ret = mSqlQuery.exec(QString("CREATE TABLE %1 \
                                       ( \
                                       %2 INTEGER PRIMARY KEY NOT NULL, \
                                       %3 TEXT NOT NULL, \
                                       %4 TEXT NOT NULL, \
                                       %5 TEXT NOT NULL, \
                                       %6 BOOL NOT NULL, \
                                       %7 INTEGER NOT NULL, \
                                       %8 INTEGER NOT NULL, \
                                       %9 INTEGER NOT NULL, \
                                       %10 BOOL NOT NULL, \
                                       %11 INTEGER NOT NULL \
                                       )")
                                      .arg(table.tableName)
                                      .arg(table.columns.id)
                                      .arg(table.columns.description)
                                      .arg(table.columns.referenceText)
                                      .arg(table.columns.responseText)
                                      .arg(table.columns.enable)
                                      .arg(table.columns.referenceFormat)
                                      .arg(table.columns.responseFormat)
                                      .arg(table.columns.option)
                                      .arg(table.columns.delay)
                                      .arg(table.columns.interval));
    return ret;
}

void SAKDebugPageCommonDatabaseInterface::createTimingSendingTable(QString &tableName)
{
    TimingSendingTable timingSendingTable;
    timingSendingTable.tableName = tableName;
    if (!isTableExist(tableName)){
        if (!createTimingSendingTable(timingSendingTable)){
            qWarning() << QString("Carete table(%1) failed:%2").arg(tableName).arg(mSqlQuery.lastError().text());
        };
    }
}

bool SAKDebugPageCommonDatabaseInterface::createTimingSendingTable(const TimingSendingTable &table)
{
    bool ret = mSqlQuery.exec(QString("CREATE TABLE %1 \
                                       ( \
                                       %2 INTEGER PRIMARY KEY NOT NULL, \
                                       %3 INTEGER NOT NULL, \
                                       %4 INTEGER NOT NULL, \
                                       %5 TEXT NOT NULL, \
                                       %6 TEXT NOT NULL \
                                       )")
                                      .arg(table.tableName)
                                      .arg(table.columns.id)
                                      .arg(table.columns.interval)
                                      .arg(table.columns.format)
                                      .arg(table.columns.description)
                                      .arg(table.columns.text));
    return ret;
}

void SAKDebugPageCommonDatabaseInterface::createPresettingDataTable(QString &tableName)
{
    DataPresetItemTable presettingDataTable;
    presettingDataTable.tableName = tableName;
    if (!isTableExist(tableName)){
        if (!createPresettingDataTable(presettingDataTable)){
            qWarning() << QString("Carete table(%1) failed:%2").arg(tableName).arg(mSqlQuery.lastError().text());
        };
    }
}

bool SAKDebugPageCommonDatabaseInterface::createPresettingDataTable(const DataPresetItemTable &table)
{
    bool ret = mSqlQuery.exec(QString("CREATE TABLE %1 \
                                       ( \
                                       %2 INTEGER PRIMARY KEY NOT NULL, \
                                       %3 INTEGER NOT NULL, \
                                       %4 TEXT NOT NULL, \
                                       %5 TEXT NOT NULL \
                                       )")
                                      .arg(table.tableName)
                                      .arg(table.columns.id)
                                      .arg(table.columns.format)
                                      .arg(table.columns.description)
                                      .arg(table.columns.text));
    return ret;
}
