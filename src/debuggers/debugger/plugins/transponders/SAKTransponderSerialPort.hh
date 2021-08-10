/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKTRANSPONDERSERIALPORT_HH
#define SAKTRANSPONDERSERIALPORT_HH

#include <QSettings>
#include <QSqlDatabase>

#include "SAKBaseListWidget.hh"
#include "SAKTransponderSerialPortItem.hh"

class SAKTransponderSerialPort : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKTransponderSerialPort(QSqlDatabase *sqlDatabase,
                             QSettings *settings,
                             QString settingsGroup,
                             QString tableNameSuffix);


protected:
    void insertRecord(const QString &tableName, QWidget *itemWidget) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    quint64 itemId(QWidget *itemWidget) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;
    void createDatabaseTable(QString tableName) final;
    void readinRecords() final;


private:
    struct SAKStructSAKTransponderSerialPortTableContext {
        QString tableName;
        struct {
            const QString id = QString("id");

            const QString portName = QString("portName");
            const QString baudRate = QString("baudRate");
            const QString dataBits = QString("dataBits");
            const QString parity = QString("parity");
            const QString stopBits = QString("stopBits");
            const QString flowControl = QString("flowControl");
            const QString frameIntervel = QString("frameIntervel");
        }columns;
    }mTableCtx;
};

#endif // SAKTRANSPONDERSERIALPORT_HH
