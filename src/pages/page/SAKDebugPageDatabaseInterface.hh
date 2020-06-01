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
#ifndef SAKDEBUGPAGEDATABASEMANAGER_HH
#define SAKDEBUGPAGEDATABASEMANAGER_HH

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

class SAKDebugPage;
/// @brief 为数据库读写提供了一套接口，主要是用户配置内容的读写
class SAKDebugPageDatabaseInterface : public QObject
{
    Q_OBJECT
public:
    /// @brief 自动应答数据表
    struct AutoResponseTable {
        bool enable;
        int format;
    };

    /// @brief 定时发送数据表
    struct TimingSendingTable {

    };

    /// @brief 数据预设数据表
    struct PresettingDataTable{

    };

    friend SAKDebugPage;
private:
    SAKDebugPageDatabaseInterface(QObject *parent = Q_NULLPTR);
    ~SAKDebugPageDatabaseInterface();
public:
    static SAKDebugPageDatabaseInterface *instance();
private:
    static SAKDebugPageDatabaseInterface *instancePtr;
    QSqlDatabase sakDatabase;
    QSqlQuery sakDatabaseQuery;
private:
    void initDatabase();
};

#endif
