/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKDEBUGPAGEDATABASEMANAGER_HH
#define SAKDEBUGPAGEDATABASEMANAGER_HH

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "SAKDataStruct.hh"

class SAKDebugPage;
/// @brief 为数据库读写提供了一套接口，主要是用户配置内容的读写
class SAKDebugPageDatabaseInterface : public QObject
{
    Q_OBJECT
private:
    SAKDebugPageDatabaseInterface(QObject *parent = Q_NULLPTR);
    ~SAKDebugPageDatabaseInterface();
public:
    friend SAKDebugPage;
    static SAKDebugPageDatabaseInterface *instance();

    /// @brief 自动应答数据表
    struct AutoResponseTable {
        QString tableName;          /// 自动回复表名称
        struct Column {
            QString id;             /// 自动回复条目ID
            QString name;           /// 自动回复条目名称
            QString referenceData;  /// 自动回复条目参考数据
            QString responseData;   /// 自动回复条目回复数据
            QString enable;         /// 是否使能该自动回复条目
            QString referenceFormat;/// 自动回复条目参考数据格式
            QString responseFormat; /// 自动回复条目应答数据格式
            QString option;         /// 自动回复条目应答选项
        }column;
    };

    /// @brief 定时发送数据表
    struct TimingSendingTable {
        QString tableName;      /// 表格名称
        struct Column {
            QString id;
            QString interval;   /// 定时间隔
            QString format;     /// 文本格式
            QString comment;    /// 备注
            QString data;       /// 定时发送数据
        }column;
    };

    /// @brief 数据预设数据表
    struct PresettingDataTable{
        QString tableName;
        struct Column {
            QString id;
            QString format;
            QString comment;
            QString classify;
            QString data;
        }column;
    };

    /**
     * @brief insertAutoResponseItem 添加自动回复条目
     * @param tableName 表格名称
     * @param item 自动回复参数
     */
    void insertAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item);

    /**
     * @brief deleteAutoResponseItem 删除自动回复条目
     * @param tableName 表格名称
     * @param item 自动回复信息（参数中ID字段有效即可）
     */
    void deleteAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item);

    /**
     * @brief updateAutoResponseItem 更新自动回复条目
     * @param tableName 表格名称
     * @param item 自动回复信息
     */
    void updateAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item);

    /**
     * @brief selectAutoResponseItem 从数据库中读出所有自动回复条目
     * @param tableName 表格名称
     * @return 自动回复条目参数信息列表
     */
    QList<SAKDataStruct::SAKStructAutoResponseItem> selectAutoResponseItem(QString tableName);

    /**
     * @brief insertTimingSendingItem 插入定时发送条目
     * @param tableName 数据表名称
     * @param item 数据信息
     */
    void insertTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item);

    /**
     * @brief deleteTimingSendingItem 删除定时发送条目
     * @param tableName 定时发送条目数据表名称
     * @param item 数据信息
     */
    void deleteTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item);

    /**
     * @brief updateTimingSendingItem 更新数据表中相应数据条目的数据
     * @param tableName 定时发送条目所在数据表的数据表名称
     * @param item 数据表名称
     */
    void updateTimingSendingItem(QString tableName, SAKDataStruct::SAKStructTimingSendingItem item);

    /**
     * @brief selectTimingSendingItem 提取数据数据库中保存的定时发送条目的数据信息
     * @param tableName 数据表名称
     * @return 定时发送条目数据信息列表
     */
    QList<SAKDataStruct::SAKStructTimingSendingItem> selectTimingSendingItem(QString tableName);

    /**
     * @brief insertPresettingDataItem 插入预设数据条目
     * @param tableName 预设数据表名称
     * @param item 数据信息
     */
    void insertPresettingDataItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item);

    /**
     * @brief deletePresettingDataItem 删除预设数据条目
     * @param tableName 预设数据条目表名称
     * @param item 数据信息
     */
    void deletePresettingDataItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item);

    /**
     * @brief updatePresettingDataItem 更新预设数据条目的数据信息
     * @param tableName 预设数据条目表名称
     * @param item 数据信息
     */
    void updatePresettingDataItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item);

    /**
     * @brief selectPresettingDataItem 获取预设数据条目数据表中保存的预设数据条目
     * @param tableName 预设数据条目表名称
     * @return 预设数据条目列表
     */
    QList<SAKDataStruct::SAKStructPresettingDataItem> selectPresettingDataItem(QString tableName);
private:
    static SAKDebugPageDatabaseInterface *instancePtr;
    QSqlDatabase sakDatabase;
    QSqlQuery sakDatabaseQuery;
    QList<AutoResponseTable> autoResponseTableList;
    QList<TimingSendingTable> timingSendingTableList;
    QList<PresettingDataTable> presettingDataTableList;
    QString databaseName;
private:
    bool isTableExist(QString tableName);
    void initDatabase();
    void createTables();
    void createAutoResponseTables();
    bool createAutoResponseTable(const AutoResponseTable &table);
    void createTimingSendingTables();
    bool createTimingSendingTable(const TimingSendingTable &table);
    void createPresettingDataTables();
    bool createPresettingDataTable(const PresettingDataTable &table);

    void deleteItemFromTable(QString tableName, quint64 id);
    AutoResponseTable tableNmaeToAutoResponseTable(QString tableName);
    TimingSendingTable tableNameToTimingSendingTable(QString tableName);
    PresettingDataTable tableNameToPresettingDataTable(QString tableName);
};

#endif
