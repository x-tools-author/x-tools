/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKUdpTransponder.hh"
#include "SAKUdpTransponders.hh"
#include "SAKCommonDataStructure.hh"

SAKUdpTransponders::SAKUdpTransponders(QSqlDatabase *sqlDatabase,
                                       QSettings *settings,
                                       QString settingsGroup,
                                       QString tableNameSuffix,
                                       QWidget *parent)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix, parent)
{
    mTableCtx.tableName = mTableName;
    initialize();
}

QString SAKUdpTransponders::sqlCreate(const QString &tableName)
{
    QString sqlString = QString("CREATE TABLE %1(").arg(tableName);
    sqlString.append(QString("%1 INTEGER PRIMARY KEY NOT NULL,")
                     .arg(mTableCtx.columns.id));
    sqlString.append(QString("%1 TEXT NOT NULL,")
                       .arg(mTableCtx.columns.peerHost));
    sqlString.append(QString("%1 INTEGER NOT NULL)")
                       .arg(mTableCtx.columns.peerPort));;
    return sqlString;
}

QString SAKUdpTransponders::sqlInsert(const QString &tableName, QWidget *itemWidget)
{
    auto cookedItemWidget
            = qobject_cast<SAKUdpTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QString();
    }

    auto itemCtx = cookedItemWidget->parametersContext()
            .value<SAKUdpClientParametersContext>();
    auto tableCtx = mTableCtx;
    QString queryString = QString("INSERT INTO %1(").arg(tableName);
    queryString.append(tableCtx.columns.id).append(",");
    queryString.append(tableCtx.columns.peerHost).append(",");
    queryString.append(tableCtx.columns.peerPort).append(")");
    queryString.append(" VALUES(");
    queryString.append(QString("%1,").arg(cookedItemWidget->id()));
    queryString.append(QString("'%1',").arg(itemCtx.peerHost));
    queryString.append(QString("%1)").arg(itemCtx.peerPort));

    return queryString;
}

QJsonObject SAKUdpTransponders::toJsonObject(QWidget *itemWidget)
{
    QJsonObject jsonObj;
    auto cookedItemWidget
            = qobject_cast<SAKUdpTransponder*>(itemWidget);
    if (!cookedItemWidget) {
        return QJsonObject();
    } else {
        auto parasCtx = cookedItemWidget->parametersContext()
                .value<SAKUdpClientParametersContext>();
        jsonObj.insert(mTableCtx.columns.id, qint64(cookedItemWidget->id()));
        jsonObj.insert(mTableCtx.columns.peerHost, parasCtx.peerPort);
        jsonObj.insert(mTableCtx.columns.peerPort, parasCtx.peerPort);
    }
    return jsonObj;
}

QJsonObject SAKUdpTransponders::toJsonObject(const QSqlQuery &sqlQuery)
{
    QJsonObject jsonObj;
    QString column;
    QVariant valueVariant;

    column = mTableCtx.columns.id;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toLongLong());

    column = mTableCtx.columns.peerHost;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toString());

    column = mTableCtx.columns.peerPort;
    valueVariant = sqlQuery.value(column);
    jsonObj.insert(column, valueVariant.toInt());

    return jsonObj;
}

QWidget *SAKUdpTransponders::createItemFromParameters(const QJsonObject &jsonObj)
{
    if (jsonObj.isEmpty()) {
        return new SAKUdpTransponder();
    } else {
        SAKUdpClientParametersContext parasCtx;
        parasCtx.peerHost = jsonObj.value(mTableCtx.columns.peerHost).toString();
        parasCtx.peerPort = jsonObj.value(mTableCtx.columns.peerPort).toInt();
        quint64 id = jsonObj.value(mTableCtx.columns.id).toVariant().toULongLong();
        auto itemWidget = new SAKUdpTransponder(id, parasCtx);
        return itemWidget;
    }
}

quint64 SAKUdpTransponders::itemId(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKUdpTransponder*>(itemWidget);
    if (cookedItemWidget) {
        return cookedItemWidget->id();
    }
    return 0;
}

void SAKUdpTransponders::connectSignalsToSlots(QWidget *itemWidget)
{
    auto cookedItemWidget = qobject_cast<SAKUdpTransponder*>(itemWidget);
    if (cookedItemWidget) {
        quint64 id = cookedItemWidget->id();
        connect(cookedItemWidget, &SAKUdpTransponder::parametersContextChanged,
                this, [=](){
            auto parasCtx = cookedItemWidget->parametersContext()
                    .value<SAKUdpClientParametersContext>();
            updateRecord(id, mTableCtx.columns.peerHost, parasCtx.peerHost);
            updateRecord(id, mTableCtx.columns.peerPort, parasCtx.peerPort);
        });
    }
}
