/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QWidget>
#include <QMetaEnum>
#include <QMapIterator>

#include "SAKDataStruct.hh"
#include "SAKToolsManager.hh"
#include "SAKToolCRCCalculator.hh"
#ifdef SAK_IMPORT_QRCODE_MODULE
#include "SAKToolQRCodeCreator.hh"
#endif
#ifdef SAK_IMPORT_FILECHECKER_MODULE
#include "SAKToolFileChecker.hh"
#endif

SAKToolsManager* SAKToolsManager::instancePtr = Q_NULLPTR;
SAKToolsManager::SAKToolsManager(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;

    /// @brief 创建工具
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKDataStruct::SAKEnumToolType>();
    for (int i = 0; i < metaEnum.keyCount(); i++){
        QWidget *toolWidget = toolWidgetFromType(metaEnum.value(i));
        if (toolWidget){
            mToolsMap.insert(metaEnum.value(i), toolWidget);
        }
    }
}

SAKToolsManager::~SAKToolsManager()
{
    instancePtr = Q_NULLPTR;
    QMapIterator<int, QWidget *> i(mToolsMap);
    while (i.hasNext()) {
        QWidget *w = i.value();
        if (w){
            w->close();
            w->deleteLater();
            w = Q_NULLPTR;
        }
    }
}

SAKToolsManager* SAKToolsManager::instance()
{
    if (!instancePtr){
        new SAKToolsManager;
    }
    Q_ASSERT_X(instancePtr, __FUNCTION__, "Can not instance the class, which name is SAKToolsManager.");

    return instancePtr;
}

void SAKToolsManager::showToolWidget(int type)
{
    QWidget *toolWidget = mToolsMap.value(type);
    if (toolWidget){
        if (toolWidget->isHidden()){
            toolWidget->show();
        }else{
            toolWidget->activateWindow();
        }
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Unknow tool type!");
    }
}

QWidget *SAKToolsManager::toolWidgetFromType(int type)
{
    QWidget *toolWidget = Q_NULLPTR;
    switch (type) {
#ifdef SAK_IMPORT_FILECHECKER_MODULE
    case SAKDataStruct::ToolTypeFileChecker:
        toolWidget = new SAKToolFileChecker;
        break;
#endif
    case SAKDataStruct::ToolTypeCRCCalculator:
        toolWidget = new SAKToolCRCCalculator;
        break;
#ifdef SAK_IMPORT_QRCODE_MODULE
    case SAKDataStruct::ToolTypeQRCodeCreator:
        toolWidget = new SAKToolQRCodeCreator;
        break;
#endif
    default:
        break;
    }

    return toolWidget;
}
