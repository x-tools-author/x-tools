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
#include <QStandardItemModel>
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

void SAKCommonDataStructure::setComboBoxTextOutputFormat(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKCommonDataStructure::OutputFormatBin,   QString("BIN"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatDec,   QString("DEC"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatHex,   QString("HEX"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUtf8,  QString("UTF8"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUcs4,  QString("UCS4"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatAscii, QString("ASCII"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUtf16, QString("UTF16"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatLocal, QString("SYSTEM"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKCommonDataStructure::OutputFormatHex));

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }
    }
}
