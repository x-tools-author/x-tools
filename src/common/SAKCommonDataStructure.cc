/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
    if (name.length()){
        name.prepend(QString("AutoResponseTable_"));
    }

    return name;
}

QString SAKDataStruct::timingSendingTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    if (name.length()){
        name.prepend(QString("TimingSendingTable_"));
    }

    return name;
}

QString SAKDataStruct::dataPresetTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    if (name.length()){
        name.prepend(QString("DataPresetTable_"));
    }

    return name;
}
