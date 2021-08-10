/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#include "SAKTransponderSerialPort.hh"

SAKTransponderSerialPort::SAKTransponderSerialPort(QSqlDatabase *sqlDatabase,
                                                   QSettings *settings,
                                                   QString settingsGroup,
                                                   QString tableNameSuffix)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix)
{

}

void SAKTransponderSerialPort::insertRecord(const QString &tableName,
                                            QWidget *itemWidget)
{
    Q_UNUSED(tableName);
    Q_UNUSED(itemWidget);
}

QWidget *SAKTransponderSerialPort::createItemFromParameters(const QJsonObject &jsonObj)
{
    if (jsonObj.isEmpty()) {
        return new SAKTransponderSerialPortItem();
    } else {
        return Q_NULLPTR;
    }
}

QJsonObject SAKTransponderSerialPort::toJsonObject(QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
    QJsonObject jsonObj;
    return jsonObj;
}

quint64 SAKTransponderSerialPort::itemId(QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
    return 0;
}

void SAKTransponderSerialPort::connectSignalsToSlots(QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
}

void SAKTransponderSerialPort::createDatabaseTable(QString tableName)
{
    Q_UNUSED(tableName);
}

void SAKTransponderSerialPort::readinRecords()
{

}
