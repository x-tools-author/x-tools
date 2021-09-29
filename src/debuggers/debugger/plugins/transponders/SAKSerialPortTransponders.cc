/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#include "SAKCommonDataStructure.hh"
#include "SAKSerialPortTransponders.hh"

SAKSerialPortTransponders::SAKSerialPortTransponders(QSqlDatabase *sqlDatabase,
                                                     QSettings *settings,
                                                     QString settingsGroup,
                                                     QString tableNameSuffix)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix)
{
    mTableCtx.tableName = mTableName;
    initialize();
}

QString SAKSerialPortTransponders::sqlCreate(const QString &tableName)
{
    QString sqlString = QString("CREATE TABLE %1(").arg(tableName);
    sqlString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                     .arg(mTableCtx.columns.id));
    sqlString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.portName));

    sqlString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.baudRate));

    sqlString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.dataBits));

    sqlString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.parity));

    sqlString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.stopBits));

    sqlString.append(QString("%1 INTEGER NOT NULL,")
                       .arg(mTableCtx.columns.flowControl));

    sqlString.append(QString("%1 INTEGER NOT NULL)")
                       .arg(mTableCtx.columns.frameIntervel));
    return sqlString;
}

QString SAKSerialPortTransponders::sqlInsert(const QString &tableName,
                                            QWidget *itemWidget)
{
    auto cookedItemWidget
            = qobject_cast<SAKSerialPortTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QString();
    }

    auto itemCtx = cookedItemWidget->parametersContext()
            .value<SAKSerialPortParametersContext>();
    auto tableCtx = mTableCtx;
    QString queryString = QString("INSERT INTO %1(").arg(tableName);
    queryString.append(tableCtx.columns.id).append(",");
    queryString.append(tableCtx.columns.portName).append(",");
    queryString.append(tableCtx.columns.baudRate).append(",");
    queryString.append(tableCtx.columns.dataBits).append(",");
    queryString.append(tableCtx.columns.parity).append(",");
    queryString.append(tableCtx.columns.stopBits).append(",");
    queryString.append(tableCtx.columns.flowControl).append(",");
    queryString.append(tableCtx.columns.frameIntervel).append(")");
    queryString.append(" VALUES(");
    queryString.append(QString("%1,").arg(cookedItemWidget->id()));
    queryString.append(QString("'%1',").arg(itemCtx.portName));
    queryString.append(QString("%1,").arg(itemCtx.baudRate));
    queryString.append(QString("%1,").arg(itemCtx.dataBits));
    queryString.append(QString("%1,").arg(itemCtx.parity));
    queryString.append(QString("%1,").arg(itemCtx.stopBits));
    queryString.append(QString("%1,").arg(itemCtx.flowControl));
    queryString.append(QString("%1)").arg(itemCtx.frameIntervel));

    return queryString;
}

QJsonObject SAKSerialPortTransponders::toJsonObject(QWidget *itemWidget)
{
    QJsonObject jsonObj;
    auto cookedItemWidget = qobject_cast<SAKSerialPortTransponder*>(itemWidget);
    if (cookedItemWidget) {
        auto parasCtx = cookedItemWidget->parametersContext()
                .value<SAKSerialPortParametersContext>();
        jsonObj.insert(mTableCtx.columns.id, qint64(cookedItemWidget->id()));
        jsonObj.insert(mTableCtx.columns.portName, parasCtx.portName);
        jsonObj.insert(mTableCtx.columns.baudRate, parasCtx.baudRate);
        jsonObj.insert(mTableCtx.columns.dataBits, parasCtx.dataBits);
        jsonObj.insert(mTableCtx.columns.parity, parasCtx.parity);
        jsonObj.insert(mTableCtx.columns.stopBits, parasCtx.stopBits);
        jsonObj.insert(mTableCtx.columns.flowControl, parasCtx.flowControl);
        jsonObj.insert(mTableCtx.columns.frameIntervel, parasCtx.frameIntervel);
    }
    return jsonObj;
}

QJsonObject SAKSerialPortTransponders::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    QString column;
    QVariant valueVariant;

    column = mTableCtx.columns.id;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toLongLong());

    column = mTableCtx.columns.portName;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.baudRate;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.dataBits;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.parity;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.stopBits;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.flowControl;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    column = mTableCtx.columns.frameIntervel;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    return jsonObj;
}

QWidget *SAKSerialPortTransponders::createItemFromParameters(const QJsonObject &jsonObj)
{
    if (jsonObj.isEmpty()) {
        return new SAKSerialPortTransponder();
    } else {
        SAKCommonDataStructure::SAKStructSerialPortParametersContext parasCtx;
        parasCtx.portName = jsonObj.value(mTableCtx.columns.portName).toString();
        parasCtx.baudRate = jsonObj.value(mTableCtx.columns.baudRate).toInt();
        parasCtx.dataBits = static_cast<QSerialPort::DataBits>(
                    jsonObj.value(mTableCtx.columns.dataBits).toInt()
                    );
        parasCtx.parity = static_cast<QSerialPort::Parity>(
                    jsonObj.value(mTableCtx.columns.parity).toInt()
                    );
        parasCtx.stopBits = static_cast<QSerialPort::StopBits>(
                    jsonObj.value(mTableCtx.columns.stopBits).toInt()
                    );
        parasCtx.flowControl = static_cast<QSerialPort::FlowControl>(
                    jsonObj.value(mTableCtx.columns.dataBits).toInt()
                    );
        parasCtx.frameIntervel = jsonObj.value(mTableCtx.columns.frameIntervel).toInt();


        quint64 id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
        auto itemWidget = new SAKSerialPortTransponder(id, parasCtx);
        return itemWidget;
    }
}

quint64 SAKSerialPortTransponders::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKSerialPortTransponder*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->id();
    }
    return 0;
}

void SAKSerialPortTransponders::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKSerialPortTransponder*>(itemWidget);
    quint64 id = cookedItemWidget->id();
    if (cookedItemWidget) {
        connect(cookedItemWidget, &SAKSerialPortTransponder::portNameChanged,
                this, [=](QString portName){
            updateRecord(id, mTableCtx.columns.portName, portName);
        });
        connect(cookedItemWidget, &SAKSerialPortTransponder::baudRateChanged,
                this, [=](int value){
            updateRecord(id, mTableCtx.columns.baudRate, value);
        });
        connect(cookedItemWidget, &SAKSerialPortTransponder::dataBitsChanged,
                this, [=](int value){
            updateRecord(id, mTableCtx.columns.dataBits, value);
        });
        connect(cookedItemWidget, &SAKSerialPortTransponder::parityChanged,
                this, [=](int value){
            updateRecord(id, mTableCtx.columns.parity, value);
        });
        connect(cookedItemWidget, &SAKSerialPortTransponder::stopBitsChanged,
                this, [=](int value){
            updateRecord(id, mTableCtx.columns.stopBits, value);
        });
        connect(cookedItemWidget, &SAKSerialPortTransponder::flowControlChanged,
                this, [=](int value){
            updateRecord(id, mTableCtx.columns.flowControl, value);
        });
        connect(cookedItemWidget, &SAKSerialPortTransponder::frameIntervalChanged,
                this, [=](int value){
            updateRecord(id, mTableCtx.columns.frameIntervel, value);
        });
    }
}
