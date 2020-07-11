/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
