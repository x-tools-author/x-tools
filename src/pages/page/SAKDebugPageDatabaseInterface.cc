/*
 * Copyright 2020 Qter. All rights reserved.
 *
 * This file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project, which is an open source project. you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 * or
 *     https://gitee.com/qsak/QtSwissArmyKnife
 * ----------------------------------------------------------------------------
 * QQ Group     : 952218522
 * Bug Report   : qsak@foxmail.com
 * Official Web : https://qsak.pro
 */
#include <QDebug>
#include <QMetaEnum>
#include <QSqlError>
#include <QDateTime>
#include <QCoreApplication>

#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKDebugPageDatabaseInterface.hh"

SAKDebugPageDatabaseInterface *SAKDebugPageDatabaseInterface::instancePtr = Q_NULLPTR;
SAKDebugPageDatabaseInterface::SAKDebugPageDatabaseInterface(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;

    /// @brief 初始化数据库
    databaseName = SAKSettings::instance()->fileName();
    QStringList strList = databaseName.split('/');
    databaseName = databaseName.remove(strList.last());
    databaseName.append(QString("QSAKDatabase.sqlite3"));
    qInfo() << "The name of database:" << databaseName;
    initDatabase();
}

SAKDebugPageDatabaseInterface::~SAKDebugPageDatabaseInterface()
{
    if (sakDatabase.isOpen()){
        sakDatabase.close();
    }
}

SAKDebugPageDatabaseInterface* SAKDebugPageDatabaseInterface::instance()
{
    if (!instancePtr){
        new SAKDebugPageDatabaseInterface(reinterpret_cast<QObject*>(qApp));
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Oh, a null pointer");
    return instancePtr;
}

void SAKDebugPageDatabaseInterface::insertAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6,%7,%8,%9) VALUES(%10,%11,%12,%13,%14,%15,%16)")
                                     .arg(table.tableName)
                                     .arg(table.idColumn)
                                     .arg(table.nameColumn)
                                     .arg(table.referenceDataColumn)
                                     .arg(table.responseDataColumn)
                                     .arg(table.enableColumn)
                                     .arg(table.referenceFormatColumn)
                                     .arg(table.responseFormatColumn)
                                     .arg(item.id)
                                     .arg(item.name)
                                     .arg(item.referenceData)
                                     .arg(item.responseData)
                                     .arg(item.enabled)
                                     .arg(item.referenceFormat)
                                     .arg(item.responseFormat));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageDatabaseInterface::deleteAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    bool ret = sakDatabaseQuery.exec(QString("DELETE FROM %1 WHERE ID=%2")
                                     .arg(tableName)
                                     .arg(item.id));
    if (!ret){
        qWarning() << __FUNCTION__ << "delete record form " << tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageDatabaseInterface::updateAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE %1 SET %2=%3, %4=%5, %6=%7, %8=%9, %10=%11, %12=%13 ,%14=%15, WHERE ID=%17")
                                     .arg(table.tableName)
                                     .arg(table.idColumn)
                                     .arg(item.id)
                                     .arg(table.nameColumn)
                                     .arg(item.name)
                                     .arg(table.referenceDataColumn)
                                     .arg(item.referenceData)
                                     .arg(table.responseDataColumn)
                                     .arg(item.responseData)
                                     .arg(table.enableColumn)
                                     .arg(item.enabled)
                                     .arg(table.referenceFormatColumn)
                                     .arg(item.referenceFormat)
                                     .arg(table.responseFormatColumn)
                                     .arg(item.responseFormat));
    if (!ret){
        qWarning() << __FUNCTION__ << "Update record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

QList<SAKDataStruct::SAKStructAutoResponseItem> SAKDebugPageDatabaseInterface::selectAutoResponseItem(QString tableName)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKDataStruct::SAKStructAutoResponseItem> itemList;
    if (ret){
        SAKDataStruct::SAKStructAutoResponseItem item;
        while (sakDatabaseQuery.next()) {
            item.id = sakDatabaseQuery.value(table.idColumn).toULongLong();
            item.name = sakDatabaseQuery.value(table.idColumn).toString();
            item.referenceData = sakDatabaseQuery.value(table.idColumn).toString();
            item.responseData = sakDatabaseQuery.value(table.idColumn).toString();
            item.enabled = sakDatabaseQuery.value(table.idColumn).toBool();
            item.referenceFormat = sakDatabaseQuery.value(table.idColumn).toUInt();
            item.responseFormat = sakDatabaseQuery.value(table.idColumn).toUInt();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageDatabaseInterface::insertTimingSendingItem(QString tableName, SAKDataStruct::TimingSendingItem item)
{
    Q_UNUSED(item);
}

bool SAKDebugPageDatabaseInterface::isTableExist(QString tableName)
{
   bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(tableName));
   return ret;
}

void SAKDebugPageDatabaseInterface::initDatabase()
{
    /// @brief 连接数据库
    sakDatabase = QSqlDatabase::addDatabase("QSQLITE");
    sakDatabase.setDatabaseName(databaseName);
    qDebug() << __FUNCTION__ << databaseName;
    /// @brief 以下是可选设置选项，对sqlite数据库来说,以下选项是无效的
    sakDatabase.setHostName("localhost");
    sakDatabase.setUserName("Qter");
    sakDatabase.setPassword("QterPassword");

    /// @brief 关联查询，初始化数据表
    sakDatabaseQuery = QSqlQuery(sakDatabase);
    if (sakDatabase.open()){
        createTables();
    }else{
        qWarning() << __FUNCTION__ << "QSAKDatabase.sqlite3 open failed: " << sakDatabase.lastError().text();
    }
}

void SAKDebugPageDatabaseInterface::createTables()
{
    createAutoResponseTables();
    createTimingSendingTables();
}

void SAKDebugPageDatabaseInterface::createAutoResponseTables()
{
    /// @brief 自动回复数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    AutoResponseTable autoResponseTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        autoResponseTable.tableName = SAKDataStruct::autoResponseTableName(i);
        autoResponseTable.idColumn = QString("ID");
        autoResponseTable.nameColumn = QString("Name");
        autoResponseTable.referenceDataColumn = QString("ReferenceData");
        autoResponseTable.responseDataColumn = QString("ResponseData");
        autoResponseTable.enableColumn = QString("Enable");
        autoResponseTable.referenceFormatColumn = QString(QString("ReferenceFormat"));
        autoResponseTable.responseFormatColumn = QString(QString("ResponseFormat"));
        autoResponseTableList.append(autoResponseTable);
    }

    for (auto var : autoResponseTableList){
        if (isTableExist(var.tableName)){
            continue;
        }

        if (!createAutoResponseTable(var)){
            qWarning() << QString("Carete table failed:%1").arg(sakDatabaseQuery.lastError().text());
        }
    }
}

bool SAKDebugPageDatabaseInterface::createAutoResponseTable(const AutoResponseTable &table)
{
    bool ret = sakDatabaseQuery.exec(QString("CREATE TABLE %1 \
                                              ( \
                                              %2 INTEGER PRIMARY KEY NOT NULL, \
                                              %3 TEXT NOT NULL, \
                                              %4 TEXT NOT NULL, \
                                              %5 TEXT NOT NULL, \
                                              %6 BOOL NOT NULL, \
                                              %7 INTEGER NOT NULL, \
                                              %8 INTEGER NOT NULL \
                                              )")
                                             .arg(table.tableName)
                                             .arg(table.idColumn)
                                             .arg(table.nameColumn)
                                             .arg(table.referenceDataColumn)
                                             .arg(table.responseDataColumn)
                                             .arg(table.enableColumn)
                                             .arg(table.referenceFormatColumn)
                                             .arg(table.responseFormatColumn));
    return ret;
}

void SAKDebugPageDatabaseInterface::createTimingSendingTables()
{
    /// @brief 定时发送数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    TimingSendingTable timingSendingTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        timingSendingTable.tableName = SAKDataStruct::timingSendingTableName(i);
        timingSendingTable.column.id = QString("ID");
        timingSendingTable.column.interval = QString("Interval");
        timingSendingTable.column.format = QString("Format");
        timingSendingTable.column.comment = QString("Comment");
        timingSendingTable.column.data = QString("Data");

        timingSendingTableList.append(timingSendingTable);
    }

    for (auto var : timingSendingTableList){
        if (isTableExist(var.tableName)){
            continue;
        }

        if (!createTimingSendingTable(var)){
            qWarning() << QString("Carete table(%1) failed:%2").arg(var.tableName).arg(sakDatabaseQuery.lastError().text());
        }
    }
}

bool SAKDebugPageDatabaseInterface::createTimingSendingTable(const TimingSendingTable &table)
{
    bool ret = sakDatabaseQuery.exec(QString("CREATE TABLE %1 \
                                              ( \
                                              %2 INTEGER PRIMARY KEY NOT NULL, \
                                              %3 TEXT NOT NULL, \
                                              %4 TEXT NOT NULL, \
                                              %5 TEXT NOT NULL \
                                              )")
                                             .arg(table.tableName)
                                             .arg(table.column.id)
                                             .arg(table.column.interval)
                                             .arg(table.column.format)
                                             .arg(table.column.data));
    return ret;
}

SAKDebugPageDatabaseInterface::AutoResponseTable SAKDebugPageDatabaseInterface::tableNmaeToAutoResponseTable(QString tableName)
{
    AutoResponseTable table;
    for (auto var : autoResponseTableList){
        if (tableName.compare(var.tableName) == 0){
            table = var;
            break;
        }
    }
    return table;
}
