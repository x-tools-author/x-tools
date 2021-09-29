/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKWebSocketTransponder.hh"
#include "SAKWebSocketTransponders.hh"

SAKWebSocketTransponders::SAKWebSocketTransponders(QSqlDatabase *sqlDatabase,
                                                   QSettings *settings,
                                                   QString settingsGroup,
                                                   QString tableNameSuffix,
                                                   QWidget *parent)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix, parent)
{
    mTableCtx.tableName = mTableName;
    initialize();
}

QString SAKWebSocketTransponders::sqlCreate(const QString &tableName)
{
    QString sqlString = QString("CREATE TABLE %1(").arg(tableName);
    sqlString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                     .arg(mTableCtx.columns.id));
    sqlString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.serverAddress));
    sqlString.append(QString("%1 INTEGER NOT NULL)")
                       .arg(mTableCtx.columns.sendingType));;
    return sqlString;
}

QString SAKWebSocketTransponders::sqlInsert(const QString &tableName, QWidget *itemWidget)
{
    auto cookedItemWidget
            = qobject_cast<SAKWebSocketTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QString();
    }

    auto itemCtx = cookedItemWidget->parametersContext()
            .value<SAKWSClientParametersContext>();
    auto tableCtx = mTableCtx;
    QString queryString = QString("INSERT INTO %1(").arg(tableName);
    queryString.append(tableCtx.columns.id).append(",");
    queryString.append(tableCtx.columns.serverAddress).append(",");
    queryString.append(tableCtx.columns.sendingType).append(")");
    queryString.append(" VALUES(");
    queryString.append(QString("%1,").arg(cookedItemWidget->id()));
    queryString.append(QString("'%1',").arg(itemCtx.serverAddress));
    queryString.append(QString("%1)").arg(itemCtx.sendingType));

    return queryString;
}

QJsonObject SAKWebSocketTransponders::toJsonObject(QWidget *itemWidget)
{
    QJsonObject jsonObj;
    auto cookedItemWidget
            = qobject_cast<SAKWebSocketTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QJsonObject();
    } else {
        auto parasCtx = cookedItemWidget->parametersContext()
                .value<SAKWSClientParametersContext>();
        jsonObj.insert(mTableCtx.columns.id, qint64(cookedItemWidget->id()));
        jsonObj.insert(mTableCtx.columns.serverAddress, parasCtx.serverAddress);
        jsonObj.insert(mTableCtx.columns.sendingType, int(parasCtx.sendingType));
    }
    return jsonObj;
}

QJsonObject SAKWebSocketTransponders::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    QString column;
    QVariant valueVariant;

    column = mTableCtx.columns.id;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toLongLong());

    column = mTableCtx.columns.serverAddress;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.sendingType;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    return jsonObj;
}

QWidget *SAKWebSocketTransponders::createItemFromParameters(const QJsonObject &jsonObj)
{
    if (jsonObj.isEmpty()) {
        return new SAKWebSocketTransponder();
    } else {
        SAKWSClientParametersContext parasCtx;
        parasCtx.serverAddress
                = jsonObj.value(mTableCtx.columns.serverAddress).toString();
        parasCtx.sendingType = jsonObj.value(mTableCtx.columns.sendingType).toInt();
        quint64 id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
        auto itemWidget = new SAKWebSocketTransponder(id, parasCtx);
        return itemWidget;
    }
}

quint64 SAKWebSocketTransponders::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKWebSocketTransponder*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->id();
    }
    return 0;
}

void SAKWebSocketTransponders::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKWebSocketTransponder*>(itemWidget);
    if (cookedItemWidget) {
        quint64 id = cookedItemWidget->id();
        connect(cookedItemWidget, &SAKWebSocketTransponder::parametersContextChanged,
                this, [=](){
            auto parasCtx = cookedItemWidget->parametersContext()
                    .value<SAKWSClientParametersContext>();
            updateRecord(id, mTableCtx.columns.serverAddress, parasCtx.serverAddress);
            updateRecord(id, mTableCtx.columns.sendingType, parasCtx.sendingType);
        });
    }
}
