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
#include "SAKCommonDataStructure.hh"

SAKCommonDataStructure::SAKCommonDataStructure(QObject* parent)
    :QObject (parent)
{

}

QString SAKCommonDataStructure::autoResponseTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    if (name.length()){
        name.prepend(QString("AutoResponseTable_"));
    }

    return name;
}

QString SAKCommonDataStructure::timingSendingTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    if (name.length()){
        name.prepend(QString("TimingSendingTable_"));
    }

    return name;
}

QString SAKCommonDataStructure::dataPresetTableName(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumDebugPageType>();
    QString name = QString(metaEnum.valueToKey(type));
    if (name.length()){
        name.prepend(QString("DataPresetTable_"));
    }

    return name;
}
