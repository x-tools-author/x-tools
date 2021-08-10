/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
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
};

#endif // SAKTRANSPONDERSERIALPORT_HH
