/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    initDatabase();
}

SAKDebugPageDatabaseInterface::~SAKDebugPageDatabaseInterface()
{
    if (sakDatabase.isOpen()){
        sakDatabase.close();
        instancePtr = Q_NULLPTR;
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
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6,%7,%8,%9) VALUES(%10,'%11','%12','%13',%14,%15,%16,%17)")
                                     .arg(table.tableName)
                                     .arg(table.column.id)
                                     .arg(table.column.name)
                                     .arg(table.column.referenceData)
                                     .arg(table.column.responseData)
                                     .arg(table.column.enable)
                                     .arg(table.column.referenceFormat)
                                     .arg(table.column.responseFormat)
                                     .arg(table.column.option)
                                     .arg(item.id)
                                     .arg(item.name)
                                     .arg(item.referenceData)
                                     .arg(item.responseData)
                                     .arg(item.enable)
                                     .arg(item.referenceFormat)
                                     .arg(item.responseFormat)
                                     .arg(item.option));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageDatabaseInterface::deleteAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    deleteItemFromTable(tableName, item.id);
}

void SAKDebugPageDatabaseInterface::updateAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE %1 SET %2=%3,%4='%5',%6='%7',%8='%9',%10=%11,%12=%13,%14=%15,%16=%17 WHERE ID=%18")
                                     .arg(table.tableName)
                                     .arg(table.column.id)
                                     .arg(item.id)
                                     .arg(table.column.name)
                                     .arg(item.name)
                                     .arg(table.column.referenceData)
                                     .arg(item.referenceData)
                                     .arg(table.column.responseData)
                                     .arg(item.responseData)
                                     .arg(table.column.enable)
                                     .arg(item.enable)
                                     .arg(table.column.referenceFormat)
                                     .arg(item.referenceFormat)
                                     .arg(table.column.responseFormat)
                                     .arg(item.responseFormat)
                                     .arg(table.column.option)
                                     .arg(item.option)
                                     .arg(item.id));
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
            item.id = sakDatabaseQuery.value(table.column.id).toULongLong();
            item.name = sakDatabaseQuery.value(table.column.name).toString();
            item.referenceData = sakDatabaseQuery.value(table.column.referenceData).toString();
            item.responseData = sakDatabaseQuery.value(table.column.responseData).toString();
            item.enable = sakDatabaseQuery.value(table.column.enable).toBool();
            item.referenceFormat = sakDatabaseQuery.value(table.column.referenceFormat).toUInt();
            item.responseFormat = sakDatabaseQuery.value(table.column.responseFormat).toUInt();
            item.option = sakDatabaseQuery.value(table.column.option).toUInt();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageDatabaseInterface::insertTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6) VALUES(%7,%8,%9,'%10','%11')")
                                     .arg(table.tableName)
                                     .arg(table.column.id)
                                     .arg(table.column.interval)
                                     .arg(table.column.format)
                                     .arg(table.column.comment)
                                     .arg(table.column.data)
                                     .arg(item.id)
                                     .arg(item.interval)
                                     .arg(item.format)
                                     .arg(item.comment)
                                     .arg(item.data));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageDatabaseInterface::deleteTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item)
{
    deleteItemFromTable(tableName, item.id);
}

void SAKDebugPageDatabaseInterface::updateTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE '%1' SET %2=%3,%4=%5,%6=%7,%8='%9',%10='%11' WHERE ID=%12")
                                     .arg(table.tableName)
                                     .arg(table.column.id)
                                     .arg(item.id)
                                     .arg(table.column.interval)
                                     .arg(item.interval)
                                     .arg(table.column.format)
                                     .arg(item.format)
                                     .arg(table.column.comment)
                                     .arg(item.comment)
                                     .arg(table.column.data)
                                     .arg(item.data)
                                     .arg(item.id));
    if (!ret){
        qWarning() << __FUNCTION__ << "Update record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

QList<SAKDataStruct::SAKStructTimingSendingItem> SAKDebugPageDatabaseInterface::selectTimingSendingItem(QString tableName)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKDataStruct::SAKStructTimingSendingItem> itemList;
    if (ret){
        SAKDataStruct::SAKStructTimingSendingItem item;
        while (sakDatabaseQuery.next()) {
            item.id = sakDatabaseQuery.value(table.column.id).toULongLong();
            item.interval = sakDatabaseQuery.value(table.column.interval).toUInt();
            item.format = sakDatabaseQuery.value(table.column.format).toUInt();
            item.comment = sakDatabaseQuery.value(table.column.comment).toString();
            item.data = sakDatabaseQuery.value(table.column.data).toString();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageDatabaseInterface::insertPresettingDataItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item)
{
    PresettingDataTable table = tableNameToPresettingDataTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6) VALUES(%7,%8,%9,%10,%11)")
                                     .arg(table.tableName)
                                     .arg(table.column.id)
                                     .arg(table.column.format)
                                     .arg(table.column.comment)
                                     .arg(table.column.data)
                                     .arg(item.id)
                                     .arg(item.format)
                                     .arg(item.comment)
                                     .arg(item.classify)
                                     .arg(item.data));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageDatabaseInterface::deletePresettingDataItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item)
{
    deleteItemFromTable(tableName, item.id);
}

void SAKDebugPageDatabaseInterface::updatePresettingDataItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item)
{
    PresettingDataTable table = tableNameToPresettingDataTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE %1 SET %2=%3, %4=%5, %6=%7, %8=%9, %10=%11, WHERE ID=%12")
                                     .arg(table.tableName)
                                     .arg(table.column.id)
                                     .arg(item.id)
                                     .arg(table.column.format)
                                     .arg(item.format)
                                     .arg(table.column.comment)
                                     .arg(item.comment)
                                     .arg(table.column.classify)
                                     .arg(item.classify)
                                     .arg(table.column.data)
                                     .arg(item.data));
    if (!ret){
        qWarning() << __FUNCTION__ << "Update record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

QList<SAKDataStruct::SAKStructPresettingDataItem> SAKDebugPageDatabaseInterface::selectPresettingDataItem(QString tableName)
{
    PresettingDataTable table = tableNameToPresettingDataTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList;
    if (ret){
        SAKDataStruct::SAKStructPresettingDataItem item;
        while (sakDatabaseQuery.next()) {
            item.id = sakDatabaseQuery.value(table.column.id).toULongLong();
            item.format = sakDatabaseQuery.value(table.column.format).toUInt();
            item.comment = sakDatabaseQuery.value(table.column.format).toString();
            item.classify = sakDatabaseQuery.value(table.column.comment).toUInt();
            item.data = sakDatabaseQuery.value(table.column.data).toString();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
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
    createPresettingDataTables();
}

void SAKDebugPageDatabaseInterface::createAutoResponseTables()
{
    /// @brief 自动回复数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    AutoResponseTable autoResponseTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        autoResponseTable.tableName = SAKDataStruct::autoResponseTableName(i);
        autoResponseTable.column.id = QString("ID");
        autoResponseTable.column.name = QString("Name");
        autoResponseTable.column.referenceData = QString("ReferenceData");
        autoResponseTable.column.responseData = QString("ResponseData");
        autoResponseTable.column.enable = QString("Enable");
        autoResponseTable.column.referenceFormat = QString("ReferenceFormat");
        autoResponseTable.column.responseFormat = QString("ResponseFormat");
        autoResponseTable.column.option = QString("Option");
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
                                              %8 INTEGER NOT NULL, \
                                              %9 INTEGER NOT NULL \
                                              )")
                                             .arg(table.tableName)
                                             .arg(table.column.id)
                                             .arg(table.column.name)
                                             .arg(table.column.referenceData)
                                             .arg(table.column.responseData)
                                             .arg(table.column.enable)
                                             .arg(table.column.referenceFormat)
                                             .arg(table.column.responseFormat)
                                             .arg(table.column.option));
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
                                              %3 INTEGER NOT NULL, \
                                              %4 INTEGER NOT NULL, \
                                              %5 TEXT NOT NULL, \
                                              %6 TEXT NOT NULL \
                                              )")
                                             .arg(table.tableName)
                                             .arg(table.column.id)
                                             .arg(table.column.interval)
                                             .arg(table.column.format)
                                             .arg(table.column.comment)
                                             .arg(table.column.data));
    return ret;
}

void SAKDebugPageDatabaseInterface::createPresettingDataTables()
{
    /// @brief 定时发送数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    PresettingDataTable presettingDataTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        presettingDataTable.tableName = SAKDataStruct::presettingDataTableName(i);
        presettingDataTable.column.id = QString("ID");
        presettingDataTable.column.format = QString("Format");
        presettingDataTable.column.comment = QString("Comment");
        presettingDataTable.column.classify = QString("Classify");
        presettingDataTable.column.data = QString("Data");

        presettingDataTableList.append(presettingDataTable);
    }

    for (auto var : presettingDataTableList){
        if (isTableExist(var.tableName)){
            continue;
        }

        if (!createPresettingDataTable(var)){
            qWarning() << QString("Carete table(%1) failed:%2").arg(var.tableName).arg(sakDatabaseQuery.lastError().text());
        }
    }
}

bool SAKDebugPageDatabaseInterface::createPresettingDataTable(const PresettingDataTable &table)
{
    bool ret = sakDatabaseQuery.exec(QString("CREATE TABLE %1 \
                                              ( \
                                              %2 INTEGER PRIMARY KEY NOT NULL, \
                                              %3 INTEGER NOT NULL, \
                                              %4 TEXT NOT NULL, \
                                              %5 INTEGER NOT NULL, \
                                              %6 TEXT NOT NULL \
                                              )")
                                             .arg(table.tableName)
                                             .arg(table.column.id)
                                             .arg(table.column.format)
                                             .arg(table.column.comment)
                                             .arg(table.column.classify)
                                             .arg(table.column.data));
    return ret;
}

void SAKDebugPageDatabaseInterface::deleteItemFromTable(QString tableName, quint64 id)
{
    bool ret = sakDatabaseQuery.exec(QString("DELETE FROM %1 WHERE ID=%2")
                                     .arg(tableName)
                                     .arg(id));
    if (!ret){
        qWarning() << __FUNCTION__ << "delete record form " << tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
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

SAKDebugPageDatabaseInterface::TimingSendingTable SAKDebugPageDatabaseInterface::tableNameToTimingSendingTable(QString tableName)
{
    TimingSendingTable table;
    for(auto var : timingSendingTableList){
        if (tableName.compare(var.tableName) == 0){
            table = var;
            break;
        }
    }
    return table;
}

SAKDebugPageDatabaseInterface::PresettingDataTable SAKDebugPageDatabaseInterface::tableNameToPresettingDataTable(QString tableName)
{
    PresettingDataTable table;
    for(auto var : presettingDataTableList){
        if (tableName.compare(var.tableName) == 0){
            table = var;
            break;
        }
    }
    return table;
}
