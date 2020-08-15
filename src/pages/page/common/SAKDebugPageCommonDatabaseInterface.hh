/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGECOMMONDATABASEMANAGER_HH
#define SAKDEBUGPAGECOMMONDATABASEMANAGER_HH

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "SAKDataStruct.hh"

class SAKDebugPage;
/// @brief If Qt has sql module, some parameters will be written to database
class SAKDebugPageCommonDatabaseInterface : public QObject
{
    Q_OBJECT
private:
    SAKDebugPageCommonDatabaseInterface(QSqlDatabase *sqlDatabase, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageCommonDatabaseInterface();
public:
    friend SAKDebugPage;
    static SAKDebugPageCommonDatabaseInterface *instance();

    /// @brief Auto response item record
    struct AutoResponseTable {
        QString tableName;
        struct Columns{
            QString id;
            QString description;
            QString referenceText;
            QString responseText;
            QString enable;
            QString referenceFormat;
            QString responseFormat;
            QString option;
            QString delay;
            QString interval;
        }columns;

        AutoResponseTable(){
            columns.id = QString("ID");
            columns.description = QString("Description");
            columns.referenceText = QString("ReferenceText");
            columns.responseText = QString("ResponseText");
            columns.enable = QString("Enable");
            columns.referenceFormat = QString("ReferenceFormat");
            columns.responseFormat = QString("ResponseFormat");
            columns.option = QString("Option");
            columns.delay = QString("Delay");
            columns.interval = QString("Interval");
        }
    };

    // timing sent table context
    struct TimingSendingTable {
        QString tableName;
        struct Columns {
            QString id;
            QString interval;
            QString format;
            QString description;
            QString text;
        }columns;

        TimingSendingTable(){
            columns.id = QString("ID");
            columns.interval = QString("Interval");
            columns.format = QString("Format");
            columns.description = QString("Description");
            columns.text = QString("Text");
        }
    };

    // data preset table context
    struct DataPresetItemTable{
        QString tableName;
        struct Columns {
            QString id;
            QString format;
            QString description;
            QString text;
        }columns;

        DataPresetItemTable(){
            columns.id = QString("ID");
            columns.format = QString("Format");
            columns.description = QString("Description");
            columns.text = QString("Text");
        }
    };

    /**
     * @brief insertAutoResponseItem: Insert record
     * @param tableName: table name of database
     * @param item: Item that contains record info
     */
    void insertAutoResponseItem(QString tableName, SAKDataStruct::SAKStructAutoResponseItem item);

    /**
     * @brief selectAutoResponseItem: Query records
     * @param tableName: table name
     * @return records
     */
    QList<SAKDataStruct::SAKStructAutoResponseItem> selectAutoResponseItem(QString tableName);

    /**
     * @brief insertTimingSendingItem: Insert record to database
     * @param tableName: Target table name
     * @param item: Record item
     */
    void insertTimingSentItem(QString tableName, SAKDataStruct::SAKStructTimingSentItem item);

    /**
     * @brief selectTimingSendingItem: Query records
     * @param tableName: Table name
     * @return records
     */
    QList<SAKDataStruct::SAKStructTimingSentItem> selectTimingSentItem(QString tableName);

    /**
     * @brief insertDataPresetItem: Insert record to database
     * @param tableName: table name of database
     * @param item: item that contains information of record
     */
    void insertDataPresetItem(QString tableName, SAKDataStruct::SAKStructPresettingDataItem item);

    /**
     * @brief selectDataPresetItem: Select records
     * @param tableName: Table name
     * @return records
     */
    QList<SAKDataStruct::SAKStructPresettingDataItem> selectDataPresetItem(QString tableName);

    /**
     * @brief updateRecord: Update the record of table
     * @param tableName: The table name that need to be updated
     * @param columnName: The column that need to be updated
     * @param value: the new value of record
     * @param recordID: The ID of record that need to be update
     * @param valueIsString: ture-the value type is string, false-the value type is not string
     */
    void updateRecord(QString tableName, QString columnName, QVariant value, quint64 recordID, bool valueIsString);

    /**
     * @brief deleteRecord: Delete record from database
     * @param tableName: table name
     * @param recordID: id of the record that need to be delete
     */
    void deleteRecord(QString tableName, quint64 recordID);
private:
    static SAKDebugPageCommonDatabaseInterface *instancePtr;
    QSqlDatabase *mSqlDatabase;
    QSqlQuery mSqlQuery;
    QList<AutoResponseTable> mAutoResponseTableList;
    QList<TimingSendingTable> mTimingSendingTableList;
    QList<DataPresetItemTable> mPresettingDataTableList;
private:
    bool isTableExist(QString tableName);
    void createTables();
    void createAutoResponseTables();
    bool createAutoResponseTable(const AutoResponseTable &table);
    void createTimingSendingTables();
    bool createTimingSendingTable(const TimingSendingTable &table);
    void createPresettingDataTables();
    bool createPresettingDataTable(const DataPresetItemTable &table);

    AutoResponseTable tableNmaeToAutoResponseTable(QString tableName);
    TimingSendingTable tableNameToTimingSendingTable(QString tableName);
    DataPresetItemTable tableNameToPresettingDataTable(QString tableName);
};

#endif
