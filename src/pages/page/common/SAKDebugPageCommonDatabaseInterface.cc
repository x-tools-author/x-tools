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

#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKSqlDatabase.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

SAKDebugPageCommonDatabaseInterface *SAKDebugPageCommonDatabaseInterface::instancePtr = Q_NULLPTR;
SAKDebugPageCommonDatabaseInterface::SAKDebugPageCommonDatabaseInterface(QSqlDatabase *sqlDatabase, QObject *parent)
    :QObject(parent)
    ,mSqlDatabase(sqlDatabase)
{
    instancePtr = this;

    mSqlQuery = QSqlQuery(*mSqlDatabase);
    if (mSqlDatabase->open()){
        createTables();
    }else{
        qWarning() << __FUNCTION__ << "QSAKDatabase.sqlite3 open failed: " << mSqlDatabase->lastError().text();
    }
}

SAKDebugPageCommonDatabaseInterface::~SAKDebugPageCommonDatabaseInterface()
{
    instancePtr = Q_NULLPTR;
}

SAKDebugPageCommonDatabaseInterface* SAKDebugPageCommonDatabaseInterface::instance()
{
    if (!instancePtr){
        new SAKDebugPageCommonDatabaseInterface(SAKSqlDatabase::instance()->sqlDatabase(), reinterpret_cast<QObject*>(qApp));
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Oh, a null pointer");
    return instancePtr;
}

void SAKDebugPageCommonDatabaseInterface::insertAutoResponseItem(QString tableName, SAKCommonDataStructure::SAKStructAutoResponseItem item)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
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
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

QList<SAKCommonDataStructure::SAKStructAutoResponseItem> SAKDebugPageCommonDatabaseInterface::selectAutoResponseItem(QString tableName)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKCommonDataStructure::SAKStructAutoResponseItem> itemList;
    if (ret){
        SAKCommonDataStructure::SAKStructAutoResponseItem item;
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
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::insertTimingSentItem(QString tableName, SAKCommonDataStructure::SAKStructTimingSentItem item)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
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
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

QList<SAKCommonDataStructure::SAKStructTimingSentItem> SAKDebugPageCommonDatabaseInterface::selectTimingSentItem(QString tableName)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKCommonDataStructure::SAKStructTimingSentItem> itemList;
    if (ret){
        SAKCommonDataStructure::SAKStructTimingSentItem item;
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

void SAKDebugPageCommonDatabaseInterface::insertDataPresetItem(QString tableName, SAKCommonDataStructure::SAKStructPresettingDataItem item)
{
    DataPresetItemTable table = tableNameToPresettingDataTable(tableName);
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

QList<SAKCommonDataStructure::SAKStructPresettingDataItem> SAKDebugPageCommonDatabaseInterface::selectDataPresetItem(QString tableName)
{
    DataPresetItemTable table = tableNameToPresettingDataTable(tableName);
    bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKCommonDataStructure::SAKStructPresettingDataItem> itemList;
    if (ret){
        SAKCommonDataStructure::SAKStructPresettingDataItem item;
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
#ifdef SAK_IMPORT_SQL_MODULE
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
        qWarning() << __FUNCTION__ << QString("Can not update record(%1):%2").arg(columnName).arg(mSqlQuery.lastError().text());
#ifdef QT_DEBUG
        qDebug() << __FUNCTION__ << queryString;
#endif
    }
#else
    Q_UNUSED(tableName);
    Q_UNUSED(columnName);
    Q_UNUSED(value);
    Q_UNUSED(recordID);
    Q_UNUSED(valueIsString);
#endif
}

void SAKDebugPageCommonDatabaseInterface::deleteRecord(QString tableName, quint64 recordID)
{
    bool ret = mSqlQuery.exec(QString("DELETE FROM %1 WHERE ID=%2")
                                     .arg(tableName)
                                     .arg(recordID));
    if (!ret){
        qWarning() << __FUNCTION__ << "delete record form " << tableName << " table failed: " << mSqlQuery.lastError().text();
    }
}

bool SAKDebugPageCommonDatabaseInterface::isTableExist(QString tableName)
{
   bool ret = mSqlQuery.exec(QString("SELECT * FROM %1").arg(tableName));
   return ret;
}

void SAKDebugPageCommonDatabaseInterface::createTables()
{
    createAutoResponseTables();
    createTimingSendingTables();
    createPresettingDataTables();
}

void SAKDebugPageCommonDatabaseInterface::createAutoResponseTables()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    AutoResponseTable autoResponseTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        autoResponseTable.tableName = SAKCommonDataStructure::autoResponseTableName(i);
        mAutoResponseTableList.append(autoResponseTable);
    }

    for (auto var : mAutoResponseTableList){
        if (isTableExist(var.tableName)){
            continue;
        }

        if (!createAutoResponseTable(var)){
            qWarning() << QString("Carete table failed:%1").arg(mSqlQuery.lastError().text());
        }
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

void SAKDebugPageCommonDatabaseInterface::createTimingSendingTables()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    TimingSendingTable timingSendingTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        timingSendingTable.tableName = SAKCommonDataStructure::timingSendingTableName(i);
        mTimingSendingTableList.append(timingSendingTable);
    }

    for (auto var : mTimingSendingTableList){
        if (isTableExist(var.tableName)){
            continue;
        }

        if (!createTimingSendingTable(var)){
            qWarning() << QString("Carete table(%1) failed:%2").arg(var.tableName).arg(mSqlQuery.lastError().text());
        }
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

void SAKDebugPageCommonDatabaseInterface::createPresettingDataTables()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    DataPresetItemTable presettingDataTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        presettingDataTable.tableName = SAKCommonDataStructure::dataPresetTableName(i);
        mPresettingDataTableList.append(presettingDataTable);
    }

    for (auto var : mPresettingDataTableList){
        if (isTableExist(var.tableName)){
            continue;
        }

        if (!createPresettingDataTable(var)){
            qWarning() << QString("Carete table(%1) failed:%2").arg(var.tableName).arg(mSqlQuery.lastError().text());
        }
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

SAKDebugPageCommonDatabaseInterface::AutoResponseTable SAKDebugPageCommonDatabaseInterface::tableNmaeToAutoResponseTable(QString tableName)
{
    AutoResponseTable table;
    for (auto var : mAutoResponseTableList){
        if (tableName.compare(var.tableName) == 0){
            table.tableName = var.tableName;
            break;
        }
    }
    return table;
}

SAKDebugPageCommonDatabaseInterface::TimingSendingTable SAKDebugPageCommonDatabaseInterface::tableNameToTimingSendingTable(QString tableName)
{
    TimingSendingTable table;
    for(auto var : mTimingSendingTableList){
        if (tableName.compare(var.tableName) == 0){
            table.tableName = var.tableName;
            break;
        }
    }
    return table;
}

SAKDebugPageCommonDatabaseInterface::DataPresetItemTable SAKDebugPageCommonDatabaseInterface::tableNameToPresettingDataTable(QString tableName)
{
    DataPresetItemTable table;
    for(auto var : mPresettingDataTableList){
        if (tableName.compare(var.tableName) == 0){
            table.tableName = var.tableName;
            break;
        }
    }
    return table;
}
