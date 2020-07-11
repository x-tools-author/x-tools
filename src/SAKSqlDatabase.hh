/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSQLDATABASE_HH
#define SAKSQLDATABASE_HH

#include <QMutex>
#include <QSqlDatabase>

/// @brief 数据库类，该类是一个单实例类
class SAKSqlDatabase : public QSqlDatabase
{
private:
    SAKSqlDatabase();
    ~SAKSqlDatabase();
public:
    /**
     * @brief instance 获取实力指针
     * @return 实例指针
     */
    static SAKSqlDatabase *instance();
private:
    static SAKSqlDatabase *instancePtr;
    static QMutex *instanceMutex;
};

#endif
