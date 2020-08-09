/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QCoreApplication>

#include "SAKGlobal.hh"
#include "SAKSqlDatabase.hh"

SAKSqlDatabase* SAKSqlDatabase::instancePtr = Q_NULLPTR;
SAKSqlDatabase* SAKSqlDatabase::instance()
{
    return Q_NULLPTR;
}

SAKSqlDatabase::SAKSqlDatabase()
{
    instancePtr = this;
}

SAKSqlDatabase::~SAKSqlDatabase()
{
    instancePtr = Q_NULLPTR;
}
