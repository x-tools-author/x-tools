/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKUdpTransponders.hh"

SAKUdpTransponders::SAKUdpTransponders(QSqlDatabase *sqlDatabase,
                                       QSettings *settings,
                                       QString settingsGroup,
                                       QString tableNameSuffix,
                                       QWidget *parent)
    :SAKBaseListWidget(sqlDatabase, settings, settingsGroup, tableNameSuffix, parent)
{

}

QString SAKUdpTransponders::sqlCreate(const QString &tableName)
{
    return tableName;
}

QString SAKUdpTransponders::sqlInsert(const QString &tableName, QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
    return tableName;
}

QJsonObject SAKUdpTransponders::toJsonObject(QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
    return QJsonObject();
}

QJsonObject SAKUdpTransponders::toJsonObject(const QSqlQuery &sqlQuery)
{
    Q_UNUSED(sqlQuery);
    return QJsonObject();
}

QWidget *SAKUdpTransponders::createItemFromParameters(const QJsonObject &jsonObj)
{
    Q_UNUSED(jsonObj);
    return Q_NULLPTR;
}

quint64 SAKUdpTransponders::itemId(QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
    return 0;
}

void SAKUdpTransponders::connectSignalsToSlots(QWidget *itemWidget)
{
    Q_UNUSED(itemWidget);
}
