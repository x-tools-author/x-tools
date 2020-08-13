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
#include <QSqlError>
#include <QCoreApplication>

#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKSqlDatabase.hh"

SAKSqlDatabase* SAKSqlDatabase::instancePtr = Q_NULLPTR;
SAKSqlDatabase* SAKSqlDatabase::instance()
{
    if (!instancePtr){
        new SAKSqlDatabase(qApp);
    }
    Q_ASSERT_X(instancePtr, __FUNCTION__, "A null pointer!");

    return instancePtr;
}

SAKSqlDatabase::SAKSqlDatabase(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;

    mSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mSqlDatabase.setDatabaseName(fullPath());
    // Do something useless
    mSqlDatabase.setHostName("localhost");
    mSqlDatabase.setUserName("Qter");
    mSqlDatabase.setPassword("QterPassword");

    if (!mSqlDatabase.open()){
        qWarning() << __FUNCTION__ << "QSAKDatabase.sqlite3 open failed: " << mSqlDatabase.lastError().text();
        Q_ASSERT_X(false, __FUNCTION__, "Open database failed!");
    }
}

SAKSqlDatabase::~SAKSqlDatabase()
{
    instancePtr = Q_NULLPTR;
    if (mSqlDatabase.isOpen()){
        mSqlDatabase.close();
    }
}

QSqlDatabase *SAKSqlDatabase::sqlDatabase()
{
    return &mSqlDatabase;
}

const QString SAKSqlDatabase::fullPath()
{
    QString databaseName = SAKSettings::instance()->fileName();
    QStringList strList = databaseName.split('/');
    databaseName = databaseName.remove(strList.last());
    databaseName.append(QString("QSAKDatabase.sqlite3"));

    return databaseName;
}
