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

#include <QSqlDatabase>

class SAKSqlDatabase:public QSqlDatabase
{
private:
    SAKSqlDatabase();
    ~SAKSqlDatabase();
public:
    static SAKSqlDatabase* instance();
    QSqlDatabase *sqlDatabase();
private:
    static SAKSqlDatabase *instancePtr;
    QSqlDatabase *mSqlDatabase;
};

#endif
