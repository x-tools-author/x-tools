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

#include "SAKCRCInterface.hh"
#include "SAKDebuggerInputSettings.hh"

SAKDebuggerInputSettings::SAKDebuggerInputSettings(QObject *parent)
    :QObject (parent)
{

}

SAKDebuggerInputSettings::~SAKDebuggerInputSettings()
{

}

QStringList SAKDebuggerInputSettings::avalidCRCParameterModel()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}


QStringList SAKDebuggerInputSettings::crcParameterModel()
{
    return avalidCRCParameterModel();
}

QStringList SAKDebuggerInputSettings::textFormats()
{
    QStringList list;
    QMetaEnum metaEnum = QMetaEnum::fromType<InputTextFormat>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QString str = QString(metaEnum.key(i));
        list.append(str);
    }

    return list;
}
