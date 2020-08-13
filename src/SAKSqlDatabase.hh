/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSQLDATABASE_HH
#define SAKSQLDATABASE_HH

#include <QObject>
#include <QSqlDatabase>

class SAKSqlDatabase:public QObject
{
    Q_OBJECT
private:
    SAKSqlDatabase(QObject *parent = Q_NULLPTR);
    ~SAKSqlDatabase();
public:
    /**
     * @brief instance: Get the instance of the calss
     * @return Instance of the class
     */
    static SAKSqlDatabase *instance();

    /**
     * @brief sqlDatabase: Get the instance of sql database
     * @return The instance of sql database
     */
    QSqlDatabase *sqlDatabase();

    /**
     * @brief fullPath: Get the full path of database
     * @return The full path of database
     */
    static const QString fullPath();
private:
    static SAKSqlDatabase *instancePtr;
    QSqlDatabase mSqlDatabase;
};

#endif
