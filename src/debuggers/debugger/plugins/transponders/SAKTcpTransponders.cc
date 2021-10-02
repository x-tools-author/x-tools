/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKTcpTransponder.hh"
#include "SAKTcpTransponders.hh"

SAKTcpTransponders::SAKTcpTransponders(QSqlDatabase *sqlDatabase,
                                       QSettings *settings,
                                       QString settingsGroup,
                                       QString tableNameSuffix,
                                       QWidget *parent)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix, parent)
{
    mTableCtx.tableName = mTableName;
    initialize();
}

QString SAKTcpTransponders::sqlCreate(const QString &tableName)
{
    QString sqlString = QString("CREATE TABLE %1(").arg(tableName);
    sqlString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                     .arg(mTableCtx.columns.id));
    sqlString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.serverHost));
    sqlString.append(QString("%1 INTEGER NOT NULL)")
                       .arg(mTableCtx.columns.serverPort));;
    return sqlString;
}

QString SAKTcpTransponders::sqlInsert(const QString &tableName, QWidget *itemWidget)
{
    auto cookedItemWidget
            = qobject_cast<SAKTcpTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QString();
    }

    auto itemCtx = cookedItemWidget->parametersContext()
            .value<SAKTcpClientParametersContext>();
    auto tableCtx = mTableCtx;
    QString queryString = QString("INSERT INTO %1(").arg(tableName);
    queryString.append(tableCtx.columns.id).append(",");
    queryString.append(tableCtx.columns.serverHost).append(",");
    queryString.append(tableCtx.columns.serverPort).append(")");
    queryString.append(" VALUES(");
    queryString.append(QString("%1,").arg(cookedItemWidget->id()));
    queryString.append(QString("'%1',").arg(itemCtx.serverHost));
    queryString.append(QString("%1)").arg(itemCtx.serverPort));

    return queryString;
}

QJsonObject SAKTcpTransponders::toJsonObject(QWidget *itemWidget)
{
    QJsonObject jsonObj;
    auto cookedItemWidget
            = qobject_cast<SAKTcpTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QJsonObject();
    } else {
        auto parasCtx = cookedItemWidget->parametersContext()
                .value<SAKTcpClientParametersContext>();
        jsonObj.insert(mTableCtx.columns.id, qint64(cookedItemWidget->id()));
        jsonObj.insert(mTableCtx.columns.serverHost, parasCtx.serverHost);
        jsonObj.insert(mTableCtx.columns.serverPort, parasCtx.serverPort);
    }
    return jsonObj;
}

QJsonObject SAKTcpTransponders::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    QString column;
    QVariant valueVariant;

    column = mTableCtx.columns.id;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toLongLong());

    column = mTableCtx.columns.serverHost;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.serverPort;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    return jsonObj;
}

QWidget *SAKTcpTransponders::createItemFromParameters(const QJsonObject &jsonObj)
{
    if (jsonObj.isEmpty()) {
        return new SAKTcpTransponder();
    } else {
        SAKTcpClientParametersContext parasCtx;
        parasCtx.serverHost = jsonObj.value(mTableCtx.columns.serverHost).toString();
        parasCtx.serverPort = jsonObj.value(mTableCtx.columns.serverPort).toInt();
        quint64 id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
        auto itemWidget = new SAKTcpTransponder(id, parasCtx);
        return itemWidget;
    }
}

quint64 SAKTcpTransponders::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKTcpTransponder*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->id();
    }
    return 0;
}

void SAKTcpTransponders::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKTcpTransponder*>(itemWidget);
    if (cookedItemWidget) {
        quint64 id = cookedItemWidget->id();
        connect(cookedItemWidget, &SAKTcpTransponder::parametersContextChanged,
                this, [=](){
            auto parasCtx = cookedItemWidget->parametersContext()
                    .value<SAKTcpClientParametersContext>();
            updateRecord(id, mTableCtx.columns.serverHost, parasCtx.serverHost);
            updateRecord(id, mTableCtx.columns.serverPort, parasCtx.serverHost);
        });
    }
}
