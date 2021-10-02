/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKSERIALPORTTRANSPONDERS_HH
#define SAKSERIALPORTTRANSPONDERS_HH

#include <QSettings>
#include <QSqlDatabase>

#include "SAKBaseListWidget.hh"
#include "SAKSerialPortTransponder.hh"

class SAKSerialPortTransponders : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKSerialPortTransponders(QSqlDatabase *sqlDatabase,
                             QSettings *settings,
                             QString settingsGroup,
                             QString tableNameSuffix);
protected:
    QString sqlCreate(const QString &tableName) final;
    QString sqlInsert(const QString &tableName, QWidget *itemWidget) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    QJsonObject toJsonObject(const QSqlQuery &sqlQuery) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    quint64 itemId(QWidget *itemWidget) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;
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
