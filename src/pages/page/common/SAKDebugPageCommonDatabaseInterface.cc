/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
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
#include "SAKDebugPageCommonDatabaseInterface.hh"

SAKDebugPageCommonDatabaseInterface *SAKDebugPageCommonDatabaseInterface::instancePtr = Q_NULLPTR;
SAKDebugPageCommonDatabaseInterface::SAKDebugPageCommonDatabaseInterface(QObject *parent)
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

SAKDebugPageCommonDatabaseInterface::~SAKDebugPageCommonDatabaseInterface()
{
    if (sakDatabase.isOpen()){
        sakDatabase.close();
        instancePtr = Q_NULLPTR;
    }
}

SAKDebugPageCommonDatabaseInterface* SAKDebugPageCommonDatabaseInterface::instance()
{
    if (!instancePtr){
        new SAKDebugPageCommonDatabaseInterface(reinterpret_cast<QObject*>(qApp));
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Oh, a null pointer");
    return instancePtr;
}

void SAKDebugPageCommonDatabaseInterface::insertAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6,%7,%8,%9) VALUES(%10,'%11','%12','%13',%14,%15,%16,%17)")
                                     .arg(table.tableName)
                                     .arg(table.columns.id)
                                     .arg(table.columns.name)
                                     .arg(table.columns.referenceData)
                                     .arg(table.columns.responseData)
                                     .arg(table.columns.enable)
                                     .arg(table.columns.referenceFormat)
                                     .arg(table.columns.responseFormat)
                                     .arg(table.columns.option)
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

void SAKDebugPageCommonDatabaseInterface::deleteAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    deleteItemFromTable(tableName, item.id);
}

void SAKDebugPageCommonDatabaseInterface::updateAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE %1 SET %2=%3,%4='%5',%6='%7',%8='%9',%10=%11,%12=%13,%14=%15,%16=%17 WHERE ID=%18")
                                     .arg(table.tableName)
                                     .arg(table.columns.id)
                                     .arg(item.id)
                                     .arg(table.columns.name)
                                     .arg(item.name)
                                     .arg(table.columns.referenceData)
                                     .arg(item.referenceData)
                                     .arg(table.columns.responseData)
                                     .arg(item.responseData)
                                     .arg(table.columns.enable)
                                     .arg(item.enable)
                                     .arg(table.columns.referenceFormat)
                                     .arg(item.referenceFormat)
                                     .arg(table.columns.responseFormat)
                                     .arg(item.responseFormat)
                                     .arg(table.columns.option)
                                     .arg(item.option)
                                     .arg(item.id));
    if (!ret){
        qWarning() << __FUNCTION__ << "Update record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

QList<SAKDataStruct::SAKStructAutoResponseItem> SAKDebugPageCommonDatabaseInterface::selectAutoResponseItem(QString tableName)
{
    AutoResponseTable table = tableNmaeToAutoResponseTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKDataStruct::SAKStructAutoResponseItem> itemList;
    if (ret){
        SAKDataStruct::SAKStructAutoResponseItem item;
        while (sakDatabaseQuery.next()) {
            item.id = sakDatabaseQuery.value(table.columns.id).toULongLong();
            item.name = sakDatabaseQuery.value(table.columns.name).toString();
            item.referenceData = sakDatabaseQuery.value(table.columns.referenceData).toString();
            item.responseData = sakDatabaseQuery.value(table.columns.responseData).toString();
            item.enable = sakDatabaseQuery.value(table.columns.enable).toBool();
            item.referenceFormat = sakDatabaseQuery.value(table.columns.referenceFormat).toUInt();
            item.responseFormat = sakDatabaseQuery.value(table.columns.responseFormat).toUInt();
            item.option = sakDatabaseQuery.value(table.columns.option).toUInt();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::insertTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5,%6) VALUES(%7,%8,%9,'%10','%11')")
                                     .arg(table.tableName)
                                     .arg(table.columns.id)
                                     .arg(table.columns.interval)
                                     .arg(table.columns.format)
                                     .arg(table.columns.comment)
                                     .arg(table.columns.data)
                                     .arg(item.id)
                                     .arg(item.interval)
                                     .arg(item.format)
                                     .arg(item.comment)
                                     .arg(item.data));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageCommonDatabaseInterface::deleteTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item)
{
    deleteItemFromTable(tableName, item.id);
}

void SAKDebugPageCommonDatabaseInterface::updateTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE '%1' SET %2=%3,%4=%5,%6=%7,%8='%9',%10='%11' WHERE ID=%12")
                                     .arg(table.tableName)
                                     .arg(table.columns.id)
                                     .arg(item.id)
                                     .arg(table.columns.interval)
                                     .arg(item.interval)
                                     .arg(table.columns.format)
                                     .arg(item.format)
                                     .arg(table.columns.comment)
                                     .arg(item.comment)
                                     .arg(table.columns.data)
                                     .arg(item.data)
                                     .arg(item.id));
    if (!ret){
        qWarning() << __FUNCTION__ << "Update record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

QList<SAKDataStruct::SAKStructTimingSendingItem> SAKDebugPageCommonDatabaseInterface::selectTimingSendingItem(QString tableName)
{
    TimingSendingTable table = tableNameToTimingSendingTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKDataStruct::SAKStructTimingSendingItem> itemList;
    if (ret){
        SAKDataStruct::SAKStructTimingSendingItem item;
        while (sakDatabaseQuery.next()) {
            item.id = sakDatabaseQuery.value(table.columns.id).toULongLong();
            item.interval = sakDatabaseQuery.value(table.columns.interval).toUInt();
            item.format = sakDatabaseQuery.value(table.columns.format).toUInt();
            item.comment = sakDatabaseQuery.value(table.columns.comment).toString();
            item.data = sakDatabaseQuery.value(table.columns.data).toString();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::insertDataPresetItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item)
{
    DataPresetItemTable table = tableNameToPresettingDataTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("INSERT INTO %1(%2,%3,%4,%5) VALUES(%6, %7,%8,'%9','%10',)")
                                     .arg(table.tableName)
                                     .arg(table.columns.id)
                                     .arg(table.columns.format)
                                     .arg(table.columns.description)
                                     .arg(table.columns.text)
                                     .arg(item.id)
                                     .arg(item.format)
                                     .arg(item.comment)
                                     .arg(item.text));
    if (!ret){
        qWarning() << __FUNCTION__ << "Insert record to " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

void SAKDebugPageCommonDatabaseInterface::deleteDataPresetItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item)
{
    deleteItemFromTable(tableName, item.id);
}

void SAKDebugPageCommonDatabaseInterface::updateDataPresetItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item)
{
    DataPresetItemTable table = tableNameToPresettingDataTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("UPDATE %1 SET %2=%3, %4=%5, %6='%7', %8='%9', WHERE %10=%11")
                                     .arg(table.tableName)
                                     .arg(table.columns.id)
                                     .arg(item.id)
                                     .arg(table.columns.format)
                                     .arg(item.format)
                                     .arg(table.columns.description)
                                     .arg(item.comment)
                                     .arg(table.columns.text)
                                     .arg(item.text)
                                     .arg(table.columns.id)
                                     .arg(item.id));
    if (!ret){
        qWarning() << __FUNCTION__ << "Update record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

QList<SAKDataStruct::SAKStructPresettingDataItem> SAKDebugPageCommonDatabaseInterface::selectDataPresetItem(QString tableName)
{
    DataPresetItemTable table = tableNameToPresettingDataTable(tableName);
    bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(table.tableName));

    QList<SAKDataStruct::SAKStructPresettingDataItem> itemList;
    if (ret){
        SAKDataStruct::SAKStructPresettingDataItem item;
        while (sakDatabaseQuery.next()) {
            item.id = sakDatabaseQuery.value(table.columns.id).toULongLong();
            item.format = sakDatabaseQuery.value(table.columns.format).toUInt();
            item.comment = sakDatabaseQuery.value(table.columns.description).toString();
            item.text = sakDatabaseQuery.value(table.columns.text).toString();

            itemList.append(item);
        }
    }else{
        qWarning() << __FUNCTION__ << "Select record form " << table.tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }

    return itemList;
}

void SAKDebugPageCommonDatabaseInterface::updateRecord(QString tableName, QString columnName, QVariant value, QString recordID, bool valueIsString)
{
#ifdef SAK_IMPORT_SQL_MODULE
    const QString queryString = QString("UPDATE %1 SET %2=%3 WHERE ID=%4")
            .arg(tableName)
            .arg(columnName)
            .arg(valueIsString ? QString("'%1'").arg(value.toString()) : QString("%1").arg(value.toInt()))
            .arg(recordID);
    if(!sakDatabaseQuery.exec(queryString)){
        qWarning() << __FUNCTION__ << QString("Can not update record(%1):%2").arg(columnName).arg(sakDatabaseQuery.lastError().text());
    }
#else
    Q_UNUSED(tableName);
    Q_UNUSED(columnName);
    Q_UNUSED(value);
    Q_UNUSED(recordID);
    Q_UNUSED(valueIsString);
#endif
}

bool SAKDebugPageCommonDatabaseInterface::isTableExist(QString tableName)
{
   bool ret = sakDatabaseQuery.exec(QString("SELECT * FROM %1").arg(tableName));
   return ret;
}

void SAKDebugPageCommonDatabaseInterface::initDatabase()
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

void SAKDebugPageCommonDatabaseInterface::createTables()
{
    createAutoResponseTables();
    createTimingSendingTables();
    createPresettingDataTables();
}

void SAKDebugPageCommonDatabaseInterface::createAutoResponseTables()
{
    /// @brief 自动回复数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    AutoResponseTable autoResponseTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        autoResponseTable.tableName = SAKDataStruct::autoResponseTableName(i);
        autoResponseTable.columns.id = QString("ID");
        autoResponseTable.columns.name = QString("Name");
        autoResponseTable.columns.referenceData = QString("ReferenceData");
        autoResponseTable.columns.responseData = QString("ResponseData");
        autoResponseTable.columns.enable = QString("Enable");
        autoResponseTable.columns.referenceFormat = QString("ReferenceFormat");
        autoResponseTable.columns.responseFormat = QString("ResponseFormat");
        autoResponseTable.columns.option = QString("Option");
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

bool SAKDebugPageCommonDatabaseInterface::createAutoResponseTable(const AutoResponseTable &table)
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
                                             .arg(table.columns.id)
                                             .arg(table.columns.name)
                                             .arg(table.columns.referenceData)
                                             .arg(table.columns.responseData)
                                             .arg(table.columns.enable)
                                             .arg(table.columns.referenceFormat)
                                             .arg(table.columns.responseFormat)
                                             .arg(table.columns.option));
    return ret;
}

void SAKDebugPageCommonDatabaseInterface::createTimingSendingTables()
{
    /// @brief 定时发送数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    TimingSendingTable timingSendingTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        timingSendingTable.tableName = SAKDataStruct::timingSendingTableName(i);
        timingSendingTable.columns.id = QString("ID");
        timingSendingTable.columns.interval = QString("Interval");
        timingSendingTable.columns.format = QString("Format");
        timingSendingTable.columns.comment = QString("Comment");
        timingSendingTable.columns.data = QString("Data");

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

bool SAKDebugPageCommonDatabaseInterface::createTimingSendingTable(const TimingSendingTable &table)
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
                                             .arg(table.columns.id)
                                             .arg(table.columns.interval)
                                             .arg(table.columns.format)
                                             .arg(table.columns.comment)
                                             .arg(table.columns.data));
    return ret;
}

void SAKDebugPageCommonDatabaseInterface::createPresettingDataTables()
{
    /// @brief 定时发送数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    DataPresetItemTable presettingDataTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        presettingDataTable.tableName = SAKDataStruct::presettingDataTableName(i);
        presettingDataTable.columns.id = QString("ID");
        presettingDataTable.columns.format = QString("Format");
        presettingDataTable.columns.description = QString("Comment");
        presettingDataTable.columns.classify = QString("Classify");
        presettingDataTable.columns.text = QString("Data");

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

bool SAKDebugPageCommonDatabaseInterface::createPresettingDataTable(const DataPresetItemTable &table)
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
                                             .arg(table.columns.id)
                                             .arg(table.columns.format)
                                             .arg(table.columns.description)
                                             .arg(table.columns.classify)
                                             .arg(table.columns.text));
    return ret;
}

void SAKDebugPageCommonDatabaseInterface::deleteItemFromTable(QString tableName, quint64 id)
{
    bool ret = sakDatabaseQuery.exec(QString("DELETE FROM %1 WHERE ID=%2")
                                     .arg(tableName)
                                     .arg(id));
    if (!ret){
        qWarning() << __FUNCTION__ << "delete record form " << tableName << " table failed: " << sakDatabaseQuery.lastError().text();
    }
}

SAKDebugPageCommonDatabaseInterface::AutoResponseTable SAKDebugPageCommonDatabaseInterface::tableNmaeToAutoResponseTable(QString tableName)
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

SAKDebugPageCommonDatabaseInterface::TimingSendingTable SAKDebugPageCommonDatabaseInterface::tableNameToTimingSendingTable(QString tableName)
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

SAKDebugPageCommonDatabaseInterface::DataPresetItemTable SAKDebugPageCommonDatabaseInterface::tableNameToPresettingDataTable(QString tableName)
{
    DataPresetItemTable table;
    for(auto var : presettingDataTableList){
        if (tableName.compare(var.tableName) == 0){
            table = var;
            break;
        }
    }
    return table;
}
