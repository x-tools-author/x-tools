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
#include "SAKDataStruct.hh"
#include "SAKDebugPageDatabaseInterface.hh"

SAKDebugPageDatabaseInterface *SAKDebugPageDatabaseInterface::instancePtr = Q_NULLPTR;
SAKDebugPageDatabaseInterface::SAKDebugPageDatabaseInterface(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;

    /// @brief 初始化数据库
    QString databaseName = SAKSettings::instance()->fileName();
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
}

void SAKDebugPageDatabaseInterface::createAutoResponseTables()
{
    /// @brief 自动回复数据表名称
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    AutoResponseTable autoResponseTable;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        autoResponseTable.tableName = QString(metaEnum.valueToKey(i));
        autoResponseTable.tableName.prepend(QString("AutoResponseTable_"));
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
