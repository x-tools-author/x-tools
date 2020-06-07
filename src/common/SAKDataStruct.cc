/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QMetaEnum>
#include "SAKDataStruct.hh"

SAKDataStruct::SAKDataStruct(QObject* parent)
    :QObject (parent)
{

}

QString SAKDataStruct::autoResponseTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    name.prepend(QString("AutoResponseTable_"));

    return name;
}

QString SAKDataStruct::timingSendingTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    name.prepend(QString("TimingSendingTable_"));

    return name;
}

QString SAKDataStruct::presettingDataTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    name.prepend(QString("PresettingDataTable_"));

    return name;
}
