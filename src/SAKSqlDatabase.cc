/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QSqlError>

#include "SAKSettings.hh"
#include "SAKSqlDatabase.hh"

SAKSqlDatabase *SAKSqlDatabase::instancePtr = Q_NULLPTR;
QMutex *SAKSqlDatabase::instanceMutex = new QMutex;
SAKSqlDatabase::SAKSqlDatabase()
{
    instancePtr = this;

    /// @brief 路径设置
    QString databaseName = SAKSettings::instance()->fileName();
    QStringList strList = databaseName.split('/');
    databaseName = databaseName.remove(strList.last());
    databaseName.append(QString("QSAKDatabase.sqlite3"));
    /// @brief 连接数据库
    addDatabase("QSQLITE");
    setDatabaseName(databaseName);
    /// @brief 以下是可选设置选项，对sqlite数据库来说,以下选项是无效的
    setHostName("localhost");
    setUserName("Qter");
    setPassword("QterPassword");

    if (!open()){
        Q_ASSERT_X(false, __FUNCTION__, lastError().text().toUtf8().data());
    }
}

SAKSqlDatabase::~SAKSqlDatabase()
{
    instancePtr = Q_NULLPTR;
    if (isOpen()){
        close();
    }
}

SAKSqlDatabase *SAKSqlDatabase::instance()
{
    instanceMutex->lock();
    if (!instancePtr){
        new SAKSqlDatabase;
    }
    instanceMutex->unlock();
    Q_ASSERT_X(instancePtr, __FUNCTION__, "Initializng database failed!");

    return instancePtr;
}
