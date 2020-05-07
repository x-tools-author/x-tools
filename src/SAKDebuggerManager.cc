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
#include <QQmlEngine>
#include <QGuiApplication>

#include "SAKDebugger.hh"
#include "SAKDebuggerManager.hh"

SAKDebuggerManager *SAKDebuggerManager::instancePtr = Q_NULLPTR;
SAKDebuggerManager::SAKDebuggerManager(QObject *parent)
    :QObject (parent)
{
    instancePtr = this;
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SAKDebuggerManager::~SAKDebuggerManager()
{

}

SAKDebuggerManager *SAKDebuggerManager::instance()
{
    if (!instancePtr){
        new SAKDebuggerManager(qApp);
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Can not instance the class(SAKDebuggerManager)!");
    return instancePtr;
}

SAKDebugger *SAKDebuggerManager::createDebugger(int type)
{
    return new SAKDebugger(type, qApp);
}
