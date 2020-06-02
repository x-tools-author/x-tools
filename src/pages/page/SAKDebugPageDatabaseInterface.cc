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
#include <QSqlError>
#include <QCoreApplication>

#include "SAKGlobal.hh"
#include "SAKDebugPageDatabaseInterface.hh"

SAKDebugPageDatabaseInterface *SAKDebugPageDatabaseInterface::instancePtr = Q_NULLPTR;
SAKDebugPageDatabaseInterface::SAKDebugPageDatabaseInterface(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;
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
    sakDatabase.setDatabaseName(SAKGlobal::dataPath().append(QString("/QSAKDatabase.sqlite3")));
    /// @brief 以下是可选设置选项，对sqlite数据库来说,以下选项是无效的
    sakDatabase.setHostName("localhost");
    sakDatabase.setUserName("Qter");
    sakDatabase.setPassword("QterPassword");

    /// @brief 关联查询，设置写同步
    sakDatabaseQuery = QSqlQuery(sakDatabase);
    if (sakDatabase.open()){
        /// @brief 以下语句据说能提高插入速度
#if 0
        spinqQuery.exec("PRAGMA synchronous = OFF");
#endif
    }else{
        qWarning() << __FUNCTION__ << "QSAKDatabase.sqlite3 open failed: " << sakDatabase.lastError().text();
    }
}
