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

#include "SAKCommonDataStructure.hh"

class SAKDebugPage;
/// @brief If Qt has sql module, some parameters will be written to database
class SAKDebugPageCommonDatabaseInterface : public QObject
{
    Q_OBJECT
public:
    SAKDebugPageCommonDatabaseInterface(SAKDebugPage *debugPage, QSqlDatabase *sqlDatabase, QObject *parent = Q_NULLPTR);
    ~SAKDebugPageCommonDatabaseInterface();

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

    struct SAKStructAutoResponseItem {
        quint64 id; // The id of response item
        QString name; //  Response item name
        QString referenceData;
        QString responseData;
        bool enable; // true-enable the response item
        quint32 referenceFormat; // see the SAKEnumTextInputFormat enum
        quint32 responseFormat; // See the SAKEnumTextInputFormat enum
        quint32 option; // response option, see the SAKEnumAutoResponseOption enum
        bool delay; // true-response delayly
        quint32 interval; // delay interval
    };

    struct SAKStructTimingSentItem {
        quint64 id; // The id of timing sent item
        quint32 interval; // Timing interval
        quint32 format; // The format of data that will be sent later
        QString comment; // The item comment
        QString data; // The data that will be sent later
    };

    struct SAKStructPresettingDataItem{
        quint64 id; // The id of the item
        quint32 format; // Data format, see the SAKEnumTextInputFormat enum
        QString description; // The description of item
        quint32 classify; // (The parameter is reserved)
        QString text; // Raw data that will be sent later
    };

    /**
     * @brief insertAutoResponseItem: Insert record
     * @param item: Item that contains record info
     */
    void insertAutoResponseItem(SAKStructAutoResponseItem item);

    /**
     * @brief selectAutoResponseItem: Query records
     * @return records
     */
    QList<SAKStructAutoResponseItem> selectAutoResponseItem();

    /**
     * @brief insertTimingSendingItem: Insert record to database
     * @param item: Record item
     */
    void insertTimingSentItem(SAKStructTimingSentItem item);

    /**
     * @brief selectTimingSendingItem: Query records
     * @return records
     */
    QList<SAKStructTimingSentItem> selectTimingSentItem();

    /**
     * @brief insertDataPresetItem: Insert record to database
     * @param item: item that contains information of record
     */
    void insertDataPresetItem(SAKStructPresettingDataItem item);

    /**
     * @brief selectDataPresetItem: Select records
     * @return records
     */
    QList<SAKStructPresettingDataItem> selectDataPresetItem();

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
    QSqlDatabase *mSqlDatabase;
    QSqlQuery mSqlQuery;
    SAKDebugPage *mDebugPage;
    QString mTableNameAutoResponseTable;
    QString mTableNameTimingSendingTable;
    QString mTableNamePresettingDataTable;
private:
    bool isTableExist(QString tableName);
    void createAutoResponseTable(QString &tableName);
    bool createAutoResponseTable(const AutoResponseTable &table);
    void createTimingSendingTable(QString &tableName);
    bool createTimingSendingTable(const TimingSendingTable &table);
    void createPresettingDataTable(QString &tableName);
    bool createPresettingDataTable(const DataPresetItemTable &table);
};

#endif
