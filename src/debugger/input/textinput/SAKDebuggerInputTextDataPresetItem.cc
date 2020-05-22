/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QMetaEnum>
#include <QDateTime>

#include "SAKDebuggerInputSettings.hh"
#include "SAKDebuggerInputTextDataPresetItem.hh"

SAKDebuggerInputTextDataPresetItem::SAKDebuggerInputTextDataPresetItem(QObject *parent)
    :QObject(parent)
{
    _itemID = QString::number(QDateTime::currentMSecsSinceEpoch());
    _itemName = _itemID;

    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDebuggerInputSettings::InputTextFormat>();
    _itemFormat = QString(metaEnum.valueToKey(SAKDebuggerInputSettings::Ascii));

    _itemText = QString("QQ:952218522");
}

SAKDebuggerInputTextDataPresetItem::SAKDebuggerInputTextDataPresetItem(QString id,
                                                                       QString name,
                                                                       QString format,
                                                                       QString text,
                                                                       QString group,
                                                                       QObject *parent)
    :QObject(parent)
    ,_itemID(id)
    ,_itemName(name)
    ,_itemFormat(format)
    ,_itemText(text)
    ,_itemGroup(group)
{

}

SAKDebuggerInputTextDataPresetItem::~SAKDebuggerInputTextDataPresetItem()
{

}

QString SAKDebuggerInputTextDataPresetItem::itemID()
{
    return _itemID;
}

QString SAKDebuggerInputTextDataPresetItem::itemName()
{
    return _itemName;
}

void SAKDebuggerInputTextDataPresetItem::setItemName(QString name)
{
    _itemName = name;
    emit itemNameChanged();
}

QString SAKDebuggerInputTextDataPresetItem::itemFormat()
{
    return _itemFormat;
}

void SAKDebuggerInputTextDataPresetItem::setItemFormat(QString format)
{
    _itemFormat = format;
    emit itemFormatChanged();
}

QString SAKDebuggerInputTextDataPresetItem::itemText()
{
    return _itemText;
}

void SAKDebuggerInputTextDataPresetItem::setItemText(QString text)
{
    _itemText = text;
    emit itemTextChanged();
}

QString SAKDebuggerInputTextDataPresetItem::itemGroup()
{
    return _itemGroup;
}

void SAKDebuggerInputTextDataPresetItem::setItemGroup(QString group)
{
    _itemGroup = group;
    emit itemGroupChanged();
}

QStringList SAKDebuggerInputTextDataPresetItem::formats()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDebuggerInputSettings::InputTextFormat>();
    QStringList stringList;
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        stringList.append(str);
    }

    return  stringList;
}
